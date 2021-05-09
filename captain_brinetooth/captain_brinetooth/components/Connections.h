#pragma once
#include "..//ecs/Component.h"
#include <string>

class Connections : public Component
{
public:
	Connections(const char& d);
	virtual ~Connections();


	const char getDir() { return dir; }

private:
	

protected:
	char dir;

};

