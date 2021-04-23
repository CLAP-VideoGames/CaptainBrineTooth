#pragma once

#include "../ecs/Component.h"
#include <string>
#include <vector>
class VideoPlayer : public Component
{
public:
	VideoPlayer(const std::string file);
	~VideoPlayer();

	void queueVideo(const std::string file);
	void init() override;
	void update() override;
	void render() override;

private:
	std::vector<std::string> files;
};

