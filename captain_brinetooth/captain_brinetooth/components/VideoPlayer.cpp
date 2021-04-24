#include "VideoPlayer.h"

VideoPlayer::VideoPlayer(const std::string file, bool loop){
	files.push_back({file, loop});
}

VideoPlayer::~VideoPlayer(){
}

void VideoPlayer::queueVideo(const std::string file, bool loop){
	files.push_back({ file, loop });
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
