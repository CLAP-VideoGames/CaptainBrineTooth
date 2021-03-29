#include "box2d.h"
#include "iostream"

class DamageCollisionCallbacks : public b2ContactListener
{
    void BeginContact(b2Contact* contact) {
        //Debugg
        std::cout << "TOUCHED WITH BEGIN CONTACT\n";

        //Codigo de ejemplo para obtener la entidad de los cuerpos que colisionan
        Entity* cosaA = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;

        //Llamamos a su metodo de colision, el cual debe de asinarse mediante un componente en la entidad
        //(Ver LoseLife para un ejemplo de componente de colision)
        cosaA->playCollisionMethod();

        Entity* cosaB = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
        cosaB->playCollisionMethod();
    }

    void EndContact(b2Contact* contact) {
        //Debugg
        std::cout << "STOPPED TOUCHING\n";


    }
};