#include "mundo.h"
#include <list>

Mundo::Mundo(b2Vec2 gravity) : world(gravity)

{
    world.SetContinuousPhysics(true);
    world.SetAllowSleeping(true);
    world.SetWarmStarting(true);
    world.SetSubStepping(false);
}

Mundo::~Mundo()
{
}

void Mundo::agregar_viga(int x, int y, int largo, float_t angulo)
{
    float_t grosor = 8.0f;
    b2Body *viga = NULL;
    {

        b2BodyDef bd;
        viga = world.CreateBody(&bd);

        b2PolygonShape boxShape;
        float_t width = b2Distance(b2Vec2(x, y), b2Vec2(x + largo, y));
        float_t height = grosor;
        b2Vec2 center(x + largo * 0.5f, y + height * 0.45f);

        // Configurar la forma de la caja con un ángulo
        boxShape.SetAsBox(width * 0.5f, height * 0.5f, center, angulo);

        b2FixtureDef fd;
        fd.shape = &boxShape;
        fd.density = 0.01f;
        fd.friction = 1.0f; // Ajusta la fricción a un valor alto para hacerla pegajosa
        fd.restitution = 0.0f;

        viga->CreateFixture(&fd);
        viga->angle = angulo;
    }
}

void Mundo::paso(float dt, int32 velocity, int32 position)
{
    world.Step(dt, velocity, position);
}

b2Body *Mundo::crear_objeto(b2BodyDef body_def)
{
    return this->world.CreateBody(&body_def);
}

void Mundo::eliminar_objeto(b2Body *body)
{
    if (body)
    {
        this->world.DestroyBody(body);
    }
}

b2Contact *Mundo::recibir_contactos()
{
    return world.GetContactList();
}
