#include "provision.h"

Provision::Provision(Mundo *mundo, int tipo, int x, int y) : tipo(tipo), x(x), y(y) {
    this->mundo = mundo;
    float_t squareSize = 1.0f;
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    b2Vec2 spawn(x, y + 10);
    bd.position = spawn;
    body = mundo->crear_objeto(bd);
    b2PolygonShape boxShape;
    boxShape.SetAsBox(squareSize * 0.5f, squareSize * 0.5f);
    b2FixtureDef fd;
    fd.shape = &boxShape;
    fd.density = 0.0f;
    fd.friction = 10.0f;
    fd.restitution = 0.0f;
    body->CreateFixture(&fd);
    body->ApplyLinearImpulse(b2Vec2(0.0, -1.0), body->GetWorldCenter(), true);
}

Provision::~Provision() {}

void Provision::recargar(b2Body *disparador) {

}

void Provision::intentar_curar()
{
    b2Contact *contact = mundo->recibir_contactos();
    while (contact != nullptr)
    {

        b2Fixture *fixtureA = contact->GetFixtureA();
        b2Fixture *fixtureB = contact->GetFixtureB();

        // Comprueba si uno de los cuerpos es el cuadrado
        if (fixtureA->GetBody() == body || fixtureB->GetBody() == body)
        {
            b2Body *otherBody = (fixtureA->GetBody() == body) ? fixtureB->GetBody() : fixtureA->GetBody();
            if (otherBody->gusano)
            {
                otherBody->vida += configuracion.getVidaExtraProvisiones();
                is_dead = true;
            }
        }
        contact = contact->GetNext();
    }
}

PosicionLanzable Provision::conseguir_posicion()
{
    if (tipo == EXPLOTAR) {
        intentar_explotar();
    } else if (tipo == CURAR) {
        intentar_curar();
    } else if (tipo == RECARGAR) {
        intentar_curar();
    }

    b2Vec2 posicion = body->GetPosition();
    int x = static_cast<int>(posicion.x);
    int y = static_cast<int>(posicion.y);
    
    return PosicionLanzable(11, x, y, 0, 0, is_dead);
}

void Provision::intentar_explotar()
{
    b2Contact *contact = mundo->recibir_contactos();
    while (contact != nullptr)
    {

        b2Fixture *fixtureA = contact->GetFixtureA();
        b2Fixture *fixtureB = contact->GetFixtureB();

        // Comprueba si uno de los cuerpos es el cuadrado
        if (fixtureA->GetBody() == body || fixtureB->GetBody() == body)
        {
            b2Body *otherBody = (fixtureA->GetBody() == body) ? fixtureB->GetBody() : fixtureA->GetBody();
            if (otherBody->gusano)
            {
                b2Vec2 center = this->body->GetPosition();
                explotar(center);
                is_dead = true;
            }
        }
        contact = contact->GetNext();
    }
}