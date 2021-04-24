#pragma once

#include "../ecs/Component.h"
#include <string>
#include <vector>
#include "SDL.h"

//Librerias de ffmpeg
	#include <iostream>
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libswscale/swscale.h>
	#include <libavutil/imgutils.h>

class VideoPlayer : public Component
{
public:
	VideoPlayer(const std::string file, bool loop);
	~VideoPlayer();

	void init() override;
	
	void update() override;
	void render() override;

	void queueVideo(const std::string file, bool loop);

private:
	std::vector<std::pair<std::string, bool>> files;

	AVFormatContext* pFormatCtx;
	AVCodecContext* pCodecCtx;
	AVCodec* pCodec;
	AVFrame* pFrame, * pFrameYUV;
	unsigned char* out_buffer;
	AVPacket* packet;
	struct SwsContext* img_convert_ctx;

	// sdl stuff
	//
	int window_w, window_h;
	SDL_Window* sdlWindow;
	SDL_Renderer* sdlRenderer;
	SDL_Texture* sdlTexture;
	SDL_Rect sdlRect;
	SDL_Event event;
};

