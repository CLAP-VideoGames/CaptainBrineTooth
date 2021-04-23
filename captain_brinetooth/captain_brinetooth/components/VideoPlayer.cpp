#include "VideoPlayer.h"

VideoPlayer::VideoPlayer(const std::string file){
	files.push_back(file);
}

VideoPlayer::~VideoPlayer(){
}

void VideoPlayer::queueVideo(const std::string file){
	files.push_back(file);
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
