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

struct Video {
	//Valores para leer los frames del video
	AVFormatContext* pFormatCtx = nullptr;
	AVCodecContext* pCodecCtx = nullptr;
	AVCodec* pCodec = nullptr;
	AVFrame* pFrame = nullptr, * pFrameYUV = nullptr;
	unsigned char* out_buffer = nullptr;
	AVPacket* packet = nullptr;
	struct SwsContext* img_convert_ctx = nullptr;
	//Controlar si el video ha acabado
	int done = 0;
	bool loop = false;
	int videoIndex;
	const char* filename;
	Uint32 timePerFrame;

	void actTexture(SDL_Texture* text){
		text = SDL_CreateTexture(sdlutils().renderer(), SDL_PIXELFORMAT_IYUV,
			SDL_TEXTUREACCESS_STREAMING, pCodecCtx->width, pCodecCtx->height);
	}
};

class VideoPlayer : public Component
{
public:
	VideoPlayer(std::vector<std::pair<const char*, bool>>& file, const Vector2D& size = Vector2D(App::camera.w * App::camera_Zoom_Out, App::camera.h* App::camera_Zoom_Out));
	~VideoPlayer();

	void init() override;
	
	void update() override;
	void render() override;

	int createVideo(Video& video_);

	void queueVideo(const char* file, bool loop);

	int prepareVideos(std::vector<std::pair<const char*, bool>>& files);

	SDL_Rect& getRect();

private:
	//Cola de videos
	std::vector<Video> queueVideos;	
	// sdl stuff
	int window_w, window_h;
	SDL_Texture* sdlTexture = nullptr;
	SDL_Rect sdlRect;
	SDL_Event event;
	
	//Tiempo entre frame y frame del video
	Uint32 lastUpdate;
	//Controlares de decodificación del video
	int done = 0;
	int paused = 0;
	int ret = 0;
	bool available = false;
};

