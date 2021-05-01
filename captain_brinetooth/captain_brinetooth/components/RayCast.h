//Defines how a RayCast functions in our world
#include <iostream>

#include "box2d.h"

/// <summary>
/// This class functions as a Box2D RayCast. When this RayCast touches an object it calls ReportFixture
/// 
/// To use this RayCast, it must be called and used as a Callback in a different script
/// 
/// Example of RayCast callback in a different script:
/// 
/// RayCast callback;
/// b2Vec2 startingPoint = b2Vec2(0.0f, 0.0f);
/// b2Vec2 endingPoint = b2Vec2(600.0f, 600.0f);
/// world_->RayCast(&callback, startingPoint, endingPoint);
/// 
/// </summary>
class RayCast : public b2RayCastCallback {
public:
	RayCast();

	float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction);

private:
	b2Fixture* m_fixture;
	b2Vec2 m_point, m_normal;
	float m_fraction;
};