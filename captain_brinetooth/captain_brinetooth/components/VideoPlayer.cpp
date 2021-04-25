#include "VideoPlayer.h"

VideoPlayer::VideoPlayer(const char* file, bool loop, const Vector2D& size){
	files.push_back({file, loop});

	window_w = size.getX();
	window_h = size.getY();
}

VideoPlayer::~VideoPlayer(){
	SDL_DestroyTexture(sdlTexture);

	// free all ffmpeg allocated data, etc.
	sws_freeContext(img_convert_ctx);
	av_frame_free(&pFrameYUV);
	av_frame_free(&pFrame);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);
}

void VideoPlayer::init(){
	loadVideo();
}

void VideoPlayer::update(){
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_SPACE) {
				paused = !paused;
			}
			else if (event.key.keysym.sym == SDLK_ESCAPE) {
				done = 1;
				break;
			}
		}
		else if (event.type == SDL_QUIT) {
			done = 1;
			break;
		}
	}

	// read (and decode) a frame if no one is available, it is separated
		// from rendering so it can be done in a thread later
	int n;
	if (!done && !available) {
		// read a frame
		do {
			n = av_read_frame(pFormatCtx, packet);
			if (n < 0) {
				//Verificamos si se quiere loopear
				if (files.back().second){
					if (n == AVERROR_EOF) {
						auto stream = pFormatCtx->streams[videoIndex];
						avio_seek(pFormatCtx->pb, 0, SEEK_SET);
						avformat_seek_file(pFormatCtx, videoIndex, 0, 0, stream->duration, 0);
						continue;
					}
				}
				else{
					done = 1;
					break;
				}
			}
		} while (packet->stream_index != videoIndex); // we just take video, we ignore audio

		// exit if there are no more frame
		if (done)
			return;

		if (n == AVERROR_EOF)
			return;

		// decode
		avcodec_send_packet(pCodecCtx, packet);
		if (ret < 0) {
			std::cout << "Error submitting a packet for decoding "
				// << av_err2str(ret) 
				<< std::endl;
			done = 1;
			return;
		}

		ret = avcodec_receive_frame(pCodecCtx, pFrame);
		if (ret < 0) {
			// those two return values are special and mean there is no output
			// frame available, but there were no errors during decoding
			if (ret == AVERROR_EOF || ret == AVERROR(EAGAIN)) {
				ret = 0;
			}
			else {
				std::cout << "Error during decoding "
					//<< av_err2str(ret)
					<< std::endl;
			}
			done = 1;
			return;
		}

		// scale the image
		sws_scale(img_convert_ctx,
			static_cast<const unsigned char* const*>(pFrame->data),
			pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data,
			pFrameYUV->linesize);

		available = true;
	}
}

void VideoPlayer::render(){
	if (sdlutils().getDebug()){
		SDL_SetRenderDrawColor(sdlutils().renderer(), 0, 255, 0, 255);
		SDL_RenderDrawRect(sdlutils().renderer(), &sdlRect);
	}

	// show a frame if timePerFrame milliseconds have passed since last update
	if (!done && !paused && available
		&& SDL_GetTicks() - lastUpdate >= timePerFrame) {
		lastUpdate = SDL_GetTicks();
		available = false;
		// render the frame
		SDL_UpdateTexture(sdlTexture, NULL, pFrameYUV->data[0],
			pFrameYUV->linesize[0]);

		av_frame_unref(pFrame);
		av_packet_unref(packet);
	}

	SDL_RenderCopy(sdlutils().renderer(), sdlTexture, NULL, &sdlRect);
}


void VideoPlayer::queueVideo(const char *file, bool loop){
	files.push_back({ file, loop });
}

int VideoPlayer::loadVideo(){

	if (files.size() == 0) return 0;

	pFormatCtx = avformat_alloc_context();

	// load the headers
	if (avformat_open_input(&pFormatCtx, files.back().first, NULL, NULL) != 0) {
		std::cout << "Couldn't open input stream.\n" << std::endl;
		return -1;
	}

	// find the stream
	if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
		std::cout << "Couldn't find stream information.\n" << std::endl;
		return -1;
	}
	videoIndex = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1,
		nullptr, 0);
	if (videoIndex < 0) {
		std::cout << "Didn't find a video stream.\n" << std::endl;
		return -1;
	}

	// find the decoder
	pCodec = avcodec_find_decoder(
		pFormatCtx->streams[videoIndex]->codecpar->codec_id);
	if (pCodec == nullptr) {
		std::cout << "Codec not found." << std::endl;
		return -1;
	}

	// allocate and initialise the codec context
	pCodecCtx = avcodec_alloc_context3(pCodec);
	if (avcodec_parameters_to_context(pCodecCtx,
		pFormatCtx->streams[videoIndex]->codecpar) < 0) {
		std::cout << "Could not fill the codec conetxt." << std::endl;
		return -1;
	}
	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
		std::cout << "Could not open codec." << std::endl;
		return -1;
	}

	//// allocate and initialise the output buffer
	pFrame = av_frame_alloc();
	pFrameYUV = av_frame_alloc();

	out_buffer =
		static_cast<unsigned char*>(av_malloc(
			static_cast<std::size_t>(av_image_get_buffer_size(
				AV_PIX_FMT_YUV420P, pCodecCtx->width,
				pCodecCtx->height, 1))));

	av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize, out_buffer,
		AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1);

	// allocate the memory where the frame (before decoding) is loaded
	packet = static_cast<AVPacket*>(av_malloc(sizeof(AVPacket)));

	//Output file information
	//av_dump_format(pFormatCtx, 0, files.back().first, 0);

	// allocate scaling/conversion context
	img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height,
		pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height,
		AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

	//// calculate frame rate
	timePerFrame = static_cast<Uint32>(1000.0
		/ av_q2d(pFormatCtx->streams[videoIndex]->r_frame_rate));

	// Create the Texture to be used to render the frame
	sdlTexture = SDL_CreateTexture(sdlutils().renderer(), SDL_PIXELFORMAT_IYUV,
		SDL_TEXTUREACCESS_STREAMING, pCodecCtx->width, pCodecCtx->height);

	// the texture occupies the whole window
	sdlRect.x = 0;
	sdlRect.y = 0;

	sdlRect.w = window_w / 1;
	sdlRect.h = window_h / 1;

	// some auxiliary variables to track frame update, pause state, etc.
	lastUpdate = SDL_GetTicks();
	done = 0;
	paused = 0;
	ret = 0;
	available = false;
}

SDL_Rect& VideoPlayer::getRect(){
	return sdlRect;
}
