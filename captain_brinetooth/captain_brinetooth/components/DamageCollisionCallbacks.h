#include "box2d.h"
#include "iostream"

class DamageCollisionCallbacks : public b2ContactListener
{
    void BeginContact(b2Contact* contact) {

        //Debugg
        std::cout << "TOUCHED WITH BEGIN CONTACT\n";

    }

    void EndContact(b2Contact* contact) {

        //Debugg
        std::cout << "STOPPED TOUCHING\n";

    }
};