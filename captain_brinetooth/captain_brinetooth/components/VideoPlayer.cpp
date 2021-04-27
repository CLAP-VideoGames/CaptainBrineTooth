#include "VideoPlayer.h"

VideoPlayer::VideoPlayer(std::vector<std::pair<const char*, std::pair<bool, int>>>& file, const Vector2D& size){

	prepareVideos(file);

	window_w = size.getX();
	window_h = size.getY();
}

VideoPlayer::~VideoPlayer(){
	SDL_DestroyTexture(sdlTexture);

	// free all ffmpeg allocated data, etc.

	for (Video& video_ : queueVideos){
		sws_freeContext(video_.img_convert_ctx);
		av_frame_free(&video_.pFrameYUV);
		av_frame_free(&video_.pFrame);
		avcodec_close(video_.pCodecCtx);
		avformat_close_input(&video_.pFormatCtx);
	}
}

void VideoPlayer::init(){
	// Create the Texture to be used to render the frame
	changeTexture();

	// the texture occupies the whole window
	sdlRect.x = 0;
	sdlRect.y = 0;
	sdlRect.w = window_w;
	sdlRect.h = window_h;

	// some auxiliary variables to track frame update, pause state, etc.
	lastUpdate = SDL_GetTicks();
	//done = 0;
	paused = 0;
	ret = 0;
	available = false;
}

void VideoPlayer::update(){
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_SPACE) {
				paused = !paused;
			}
			else if (event.key.keysym.sym == SDLK_ESCAPE) {
				queueVideos.back().done = 1;
				break;
			}
		}
		else if (event.type == SDL_QUIT) {
			queueVideos.back().done = 1;
			break;
		}
	}

	// read (and decode) a frame if no one is available, it is separated
		// from rendering so it can be done in a thread later
	int n;
	if (!queueVideos.empty() && !queueVideos.back().done && !available) {
		// read a frame
		do {
			n = av_read_frame(queueVideos.back().pFormatCtx, queueVideos.back().packet);
			if (n < 0) {
				//Verificamos si se quiere loopear
				if (queueVideos.back().loop){
					if (n == AVERROR_EOF) {
						auto stream = queueVideos.back().pFormatCtx->streams[queueVideos.back().videoIndex];
						avio_seek(queueVideos.back().pFormatCtx->pb, 0, SEEK_SET);
						avformat_seek_file(queueVideos.back().pFormatCtx, queueVideos.back().videoIndex, 0, 0, stream->duration, 0);
						continue;
					}
				}
				else{
					queueVideos.back().done = 1;
					break;
				}
			}
		} while (queueVideos.back().packet->stream_index != queueVideos.back().videoIndex); // we just take video, we ignore audio

		// exit if there are no more frame
		if (queueVideos.back().done)
			return;

		if (n == AVERROR_EOF)
			return;

		// decode
		avcodec_send_packet(queueVideos.back().pCodecCtx, queueVideos.back().packet);
		if (ret < 0) {
			std::cout << "Error submitting a packet for decoding "
				// << av_err2str(ret) 
				<< std::endl;
			queueVideos.back().done = 1;
			return;
		}

		ret = avcodec_receive_frame(queueVideos.back().pCodecCtx, queueVideos.back().pFrame);
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

			//queueVideos.back().done = 1;
			return;
		}

		// scale the image
		sws_scale(queueVideos.back().img_convert_ctx,
			static_cast<const unsigned char* const*>(queueVideos.back().pFrame->data),
			queueVideos.back().pFrame->linesize, 0, queueVideos.back().pCodecCtx->height, queueVideos.back().pFrameYUV->data,
			queueVideos.back().pFrameYUV->linesize);

		available = true;
	}

	if (queueVideos.back().done){
		popVideo();
	}

}

void VideoPlayer::render(){
	if (sdlutils().getDebug()){
		SDL_SetRenderDrawColor(sdlutils().renderer(), 255, 255, 0, 255);
		SDL_RenderDrawRect(sdlutils().renderer(), &sdlRect);
	}

	// show a frame if timePerFrame milliseconds have passed since last update
	if (!queueVideos.empty() && !queueVideos.back().done && !paused && available
		&& SDL_GetTicks() - lastUpdate >= queueVideos.back().timePerFrame) {
		lastUpdate = SDL_GetTicks();
		available = false;
		// render the frame
		SDL_UpdateTexture(sdlTexture, NULL, queueVideos.back().pFrameYUV->data[0],
			queueVideos.back().pFrameYUV->linesize[0]);

		av_frame_unref(queueVideos.back().pFrame);
		av_packet_unref(queueVideos.back().packet);
	}

	SDL_RenderCopy(sdlutils().renderer(), sdlTexture, NULL, &sdlRect);
}

int VideoPlayer::createVideo(Video& video_){
	video_.pFormatCtx = avformat_alloc_context();
		
	// load the headers
	if (avformat_open_input(&video_.pFormatCtx, video_.filename, NULL, NULL) != 0) {
		std::cout << "Couldn't open input stream.\n" << std::endl;
		return -1;
	}

	// find the stream
	if (avformat_find_stream_info(video_.pFormatCtx, NULL) < 0) {
		std::cout << "Couldn't find stream information.\n" << std::endl;
		return -1;
	}

	video_.videoIndex = av_find_best_stream(video_.pFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1,
		nullptr, 0);

	if (video_.videoIndex < 0) {
		std::cout << "Didn't find a video stream.\n" << std::endl;
		return -1;
	}

	// find the decoder
	video_.pCodec = avcodec_find_decoder(
		video_.pFormatCtx->streams[video_.videoIndex]->codecpar->codec_id);
	if (video_.pCodec == nullptr) {
		std::cout << "Codec not found." << std::endl;
		return -1;
	}

	// allocate and initialise the codec context
	video_.pCodecCtx = avcodec_alloc_context3(video_.pCodec);
	if (avcodec_parameters_to_context(video_.pCodecCtx,
		video_.pFormatCtx->streams[video_.videoIndex]->codecpar) < 0) {
		std::cout << "Could not fill the codec conetxt." << std::endl;
		return -1;
	}
	if (avcodec_open2(video_.pCodecCtx, video_.pCodec, NULL) < 0) {
		std::cout << "Could not open codec." << std::endl;
		return -1;
	}

	//// allocate and initialise the output buffer
	video_.pFrame = av_frame_alloc();
	video_.pFrameYUV = av_frame_alloc();

	video_.out_buffer =
		static_cast<unsigned char*>(av_malloc(
			static_cast<std::size_t>(av_image_get_buffer_size(
				AV_PIX_FMT_YUV420P, video_.pCodecCtx->width,
				video_.pCodecCtx->height, 1))));

	av_image_fill_arrays(video_.pFrameYUV->data, video_.pFrameYUV->linesize, video_.out_buffer,
		AV_PIX_FMT_YUV420P, video_.pCodecCtx->width, video_.pCodecCtx->height, 1);

	// allocate the memory where the frame (before decoding) is loaded
	video_.packet = static_cast<AVPacket*>(av_malloc(sizeof(AVPacket)));

	//Output file information
	//av_dump_format(pFormatCtx, 0, files.back().first, 0);

	// allocate scaling/conversion context
	video_.img_convert_ctx = sws_getContext(video_.pCodecCtx->width, video_.pCodecCtx->height,
		video_.pCodecCtx->pix_fmt, video_.pCodecCtx->width, video_.pCodecCtx->height,
		AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

	//// calculate frame rate
	////Esto no me sirve :C
	//video_.timePerFrame = static_cast<Uint32>(1000.0
	//	/ av_q2d(video_.pFormatCtx->streams[video_.videoIndex]->r_frame_rate));

	return 0;
}

void VideoPlayer::queueVideo(const char *file, bool loop_, int frameRate){
	Video video;
	video.filename = file;
	video.loop = loop_;
	video.timePerFrame= frameRate;
	createVideo(video);
	queueVideos.push_back(video);
}

void VideoPlayer::popVideo(){
	//No queremos que borre el ultimo video ya que borraria la textura y se dejar�a de renderizar
	if (!queueVideos.empty() && queueVideos.size() != 1) {
		queueVideos.pop_back();
		changeTexture();
	}
}

/// <summary>
/// Be sure that you wont use the sdltexture after the pop
/// </summary>
void VideoPlayer::forcePopVideo() {
	if (!queueVideos.empty()) {
		queueVideos.pop_back();
		changeTexture();
	}
}

void VideoPlayer::prepareVideos(std::vector<std::pair<const char*, std::pair<bool, int>>>& files){
	int size = files.size();

	for (int i = 0; i < size; i++)
		queueVideo(files[i].first, files[i].second.first , files[i].second.second);
}

SDL_Rect& VideoPlayer::getRect(){
	return sdlRect;
}

void VideoPlayer::changeTexture(){
	if(sdlTexture != nullptr)
		SDL_DestroyTexture(sdlTexture);

	sdlTexture = queueVideos.back().actTexture();
}
