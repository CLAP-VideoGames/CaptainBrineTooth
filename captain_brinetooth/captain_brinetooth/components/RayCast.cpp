#include "RayCast.h"

RayCast::RayCast(){
	m_fixture = NULL;
}

float RayCast::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction){
	m_fixture = fixture;
	m_point = point;
	m_normal = normal;
	m_fraction = fraction;

	//Console logs
	std::cout << "Fraction: " << fraction << "\n";
	std::cout << "Punto de colision: " << point.x << " " << point.y << "\n";
	std::cout << "Normal: " << normal.x << " " << normal.y << "\n";

	//Add code to do something
	//fixture->GetBody()->ApplyForce(b2Vec2(0, -1000), point, true);	Example

	return fraction;
}
