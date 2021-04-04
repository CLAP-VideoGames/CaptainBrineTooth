// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

// components -- forward declaration, i.e., assume they are defined somewhere
class Transform;
class Rectangle;
class KeyBoardCtrl;
class Bounce;
class Animation;
class AnimBlendGraph;
class Rotate;
class BoxCollider;
class Player_Health;
class Armas_HUD;
class EnemyMovement;
class Level0;
class Chainsaw;
class ChainCollider;
class PlayerController;
class CameraFollow;
class SoundManager;
class LoseLife;
class FringeHeadAtack;
class Button;
class Sword;
class Hammer;
class ShootDetect;
class DisableOnExit;
class ContactDamage;
class Enemy_Health;
class WeaponDamageDetection;
class DestroyOnCollision;
class Button;

#define _CMPS_LIST_  \
	Transform,\
	Rectangle,\
	KeyBoardCtrl,\
	Bounce,\
	Animation,\
	AnimBlendGraph,\
	Rotate,\
	BoxCollider,\
	Player_Health,\
	Armas_HUD, \
	EnemyMovement, \
	PlayerController, \
	Level0, \
	Chainsaw,\
	ChainCollider,\
	CameraFollow,\
	SoundManager,\
	LoseLife,\
	FringeHeadAtack,\
	Button,\
	Sword,\
    ShootDetect,\
    DisableOnExit,\
    ContactDamage,\
    Enemy_Health,\
	Hammer,\
	WeaponDamageDetection,\
	DestroyOnCollision, \
	Button

// groups

struct Group_1;
#define _GRPS_LIST_  Group_1

// handlers
struct Player;
#define _HDLRS_LIST_ Player
