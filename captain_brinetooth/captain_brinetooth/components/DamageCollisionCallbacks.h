#include "box2d.h"
#include "iostream"
#include "../sdlutils/SDLUtils.h"


class DamageCollisionCallbacks : public b2ContactListener{
    void BeginContact(b2Contact* contact);

    void EndContact(b2Contact* contact);
};