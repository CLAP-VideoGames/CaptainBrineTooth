// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

// components -- forward declaration, i.e., assume they are defined somewhere
class Transform;
class Rectangle;
class KeyBoardCtrl;
class Bounce;
class Image;
class Animation;
class AnimBlendGraph;
class Rotate;
class FramedImage;
class BoxCollider;
class Player_Health;
class Armas_HUD;
class EnemyMovement;
class Level0;
class Chainsaw;

#define _CMPS_LIST_  \
	Transform,\
	Rectangle,\
	KeyBoardCtrl,\
	Bounce,\
	Image,\
	Animation,\
	AnimBlendGraph,\
	Rotate,\
	BoxCollider,\
	FramedImage,\
	Player_Health,\
	Armas_HUD, \
	EnemyMovement, \
	Level0, \
	Chainsaw

// groups

struct Group_1;
#define _GRPS_LIST_  Group_1

// handlers
struct Player;
#define _HDLRS_LIST_ Player
