#ifndef OBJETO_DTO_H_
#define OBJETO_DTO_H_

#include <string>
#include <map>
#include <vector>
#include <ostream>
#include <iostream>
#include <SDL2pp/SDL2pp.hh>
#include "../graphics/texture_manager.h"
// #include "../weapons/weapon.h"
// #include "../weapons/common_ataque_aereo.h"

class ObjetoDTO
{
private:
    int id;
    int x;
    int y;
    bool direccion;
    int angulo;
    bool is_running = true;   // whether the character is currently running
    int run_phase = 0; // run animation phase
    int tipo;
    bool explosion;

public:
    ObjetoDTO(int tipo,int x, int y, bool direccion, int angulo,  bool explosion);

    void mostrar() const;

    int obtenerId();

    void activa_animacion(bool permiso);

    void actualizar(ObjetoDTO jugador);

    void renderizar(SDL2pp::Renderer &renderer, TextureManager &tex_manager);

    int posicion_x();

    int posicion_y();
};

#endif
