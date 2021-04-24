#include "VideoPlayer.h"

VideoPlayer::VideoPlayer(const char* file, bool loop, const Vector2D& size){
	files.push_back({file, loop});

	window_w = size.getX();
	window_h = size.getY();
}

VideoPlayer::~VideoPlayer(){

}

void VideoPlayer::init()
{
}

void VideoPlayer::update()
{
}

void VideoPlayer::render()
{
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
	int videoIndex = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1,
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


	// Create the SDL Rendered
	sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);

	// Create the Texture to be used to render the frame
	sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_IYUV,
		SDL_TEXTUREACCESS_STREAMING, pCodecCtx->width, pCodecCtx->height);

	// the texture occupies the whole window
	sdlRect.x = window_w / 4;
	sdlRect.y = window_h / 4;

	sdlRect.w = window_w / 2;
	sdlRect.h = window_h / 2;

	// some auxiliary variables to track frame update, pause state, etc.
	lastUpdate = SDL_GetTicks();
	done = 0;
	paused = 0;
	ret = 0;
	available = false;
}
