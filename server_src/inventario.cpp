#include "inventario.h"
#include <map>
#include <string>

Inventario::Inventario() : ammo({0, 12, 100, 100, 100, 100, 100, 100, 100, 100, 100})
{
}

Inventario::~Inventario() {}

bool Inventario::disparar(Mundo *world, b2Body *posicion, Arma *arma)
{

    if (this->ammo[this->arma_actual] <= 0)
    {
        return false;
    }
    int ammo_usada = arma->disparar(world, posicion);
    this->ammo[this->arma_actual] -= ammo_usada;
    arma_actual = 0;
    return true;
}

int Inventario::cambiar_arma(int tipo)
{
    if (tipo >= 0 && tipo < 11)
    {
        arma_actual = tipo;
        return ammo[tipo];
    }
    else
    {
        std::cerr << "Error: Tipo de arma fuera de rango." << std::endl;
        return -1;
    }
}
