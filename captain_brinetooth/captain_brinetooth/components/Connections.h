#pragma once
#include "..//ecs/Component.h"
#include <string>

class Connections : public Component
{
public:
	Connections(const std::string& d);
	virtual ~Connections();


	std::string getDir() { return dir; }

private:
	

protected:
	std::string dir;

};

