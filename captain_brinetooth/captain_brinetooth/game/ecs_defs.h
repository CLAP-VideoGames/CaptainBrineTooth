// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

// components -- forward declaration, i.e., assume they are defined somewhere
class Transform;
class Rectangle;
class KeyBoardCtrl;
class Bounce;
class Animation;
class AnimBlendGraph;
class ParallaxScroll;
class Rotate;
class BoxCollider;
class TriggerCollider;
class Player_Health;
class Armas_HUD;
class EnemyMovement;
class Level0;
class Chainsaw;
class MapCollider;
class PlayerController;
class CameraFollow;
class SoundManager;
class LoseLife;
class FringeHeadAtack;
class ElfSharkAttack;
class PompeyWormAttack;
class Button;
class Sword;
class Hammer;
class ShootDetect;
class DisableOnExit;
class ContactDamage;
class Enemy_Health;
class EnemyTrigger;
class WeaponDamageDetection;
class DestroyOnCollision;
class Button;
class MapProcedural;
class Image;
class Slider;
class Reward;
class Gancho;
class Crab;
class MachineGun;
class ElectricWhip;
class Bellow;
class VideoPlayer;
class Fade;
class JellyHatBehavior;
class Connections;
class Inventory;
class GetAWeaponOnCollision;
class Cuerda;
class PescaController;
class EnemyGenerator;

#define _CMPS_LIST_  \
	Transform,\
	Rectangle,\
	KeyBoardCtrl,\
	Bounce,\
	Animation,\
	AnimBlendGraph,\
	ParallaxScroll,\
	Rotate,\
	BoxCollider,\
	TriggerCollider,\
	Player_Health,\
	Armas_HUD, \
	EnemyMovement, \
	PlayerController, \
	Level0, \
	Chainsaw,\
	MapCollider,\
	CameraFollow,\
	Reward,\
    Gancho,\
    SoundManager,\
	LoseLife,\
	Fade,\
	EnemyTrigger,\
	FringeHeadAtack,\
	ElfSharkAttack,\
	PompeyWormAttack,\
	Button,\
	Sword,\
    ShootDetect,\
    Slider,\
    DisableOnExit,\
    ContactDamage,\
    Enemy_Health,\
	Hammer,\
	WeaponDamageDetection,\
	DestroyOnCollision, \
	MapProcedural, \
	Button ,\
	Image,\
	Crab,\
	MachineGun,\
	ElectricWhip,\
	Bellow,\
	Connections,\
	VideoPlayer,\
	JellyHatBehavior,\
	Inventory,\
	GetAWeaponOnCollision,\
	Cuerda,\
	EnemyGenerator,\
	PescaController

// groups

struct Group_1;
#define _GRPS_LIST_  Group_1

// handlers
struct Player;
struct Rod;
struct Map;
#define _HDLRS_LIST_ Player, \
					Map,\
                    Rod
