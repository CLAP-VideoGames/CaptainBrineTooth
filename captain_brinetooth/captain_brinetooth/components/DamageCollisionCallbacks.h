#include "box2d.h"
#include "iostream"

class DamageCollisionCallbacks : public b2ContactListener
{
    void BeginContact(b2Contact* contact) {
        //Debugg
        std::cout << "TOUCHED WITH BEGIN CONTACT\n";

        Entity* cosaA = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
        cosaA->playCollisionMethod();

        Entity* cosaB = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
        cosaB->playCollisionMethod();
    }

    void EndContact(b2Contact* contact) {
        //Debugg
        std::cout << "STOPPED TOUCHING\n";


    }
};