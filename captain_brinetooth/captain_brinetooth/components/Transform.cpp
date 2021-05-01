#include "Transform.h"

Transform::Transform() :
	pos_(), vel_(), width_(), height_(), rotation_() {
}

Transform::Transform(Vector2D pos, Vector2D vel, float width, float height, float rotation) :
	pos_(pos), //
	vel_(vel), //
	width_(width), //
	height_(height), //
	rotation_(rotation)
{
}

Transform::~Transform()
{
}

Vector2D& Transform::getPos() {
	return pos_;
}

Vector2D& Transform::getVel() {
	return vel_;
}

float Transform::getH() const {
	return height_;
}

void Transform::setH(float height) {
	height_ = height;
}

float Transform::getW() const {
	return width_;
}

void Transform::setW(float width) {
	width_ = width;
}

float Transform::getRot() const {
	return rotation_;
}

void Transform::setRot(float rot) {
	rotation_ = rot;
}

void Transform::update() {
	pos_ = pos_ + vel_;
}
