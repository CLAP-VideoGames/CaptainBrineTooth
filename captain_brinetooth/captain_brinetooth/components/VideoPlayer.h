#pragma once

#include "../ecs/Component.h"
#include <string>
#include <vector>
#include "SDL.h"
#include <iostream>
#include "../game/App.h"
#include "../sdlutils/SDLUtils.h"

//Librerias de ffmpeg
extern "C" {
	#include <libavformat/avformat.h>
	#include <libswscale/swscale.h>
	#include <libavutil/imgutils.h>
	#include <libavcodec/avcodec.h>
}


class VideoPlayer : public Component
{
public:
	VideoPlayer(const char* file, bool loop, const Vector2D& size = Vector2D(App::camera.w, App::camera.h));
	~VideoPlayer();

	void init() override;
	
	void update() override;
	void render() override;

	void queueVideo(const char* file, bool loop);

	int loadVideo();

private:

	std::vector<std::pair<const char*, bool>> files;
	//Valores para leer los frames del video
	AVFormatContext* pFormatCtx = nullptr;
	AVCodecContext* pCodecCtx = nullptr;
	AVCodec* pCodec = nullptr;
	AVFrame* pFrame = nullptr, * pFrameYUV = nullptr;
	unsigned char* out_buffer = nullptr;
	AVPacket* packet = nullptr;
	struct SwsContext* img_convert_ctx = nullptr;

	// sdl stuff
	int window_w, window_h;
	SDL_Texture* sdlTexture = nullptr;
	SDL_Rect sdlRect;
	SDL_Event event;

	int videoIndex;
	//Tiempo entre frame y frame del video
	Uint32 timePerFrame;
	Uint32 lastUpdate;
	//Controlares de decodificación del video
	int done = 0;
	int paused = 0;
	int ret = 0;
	bool available = false;
};

