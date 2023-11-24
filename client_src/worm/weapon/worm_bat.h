#ifndef WORM_BAT_H
#define WORM_BAT_H

#include "worm_weapon.h"
#include "crosshair.h"
class WormBat : public WormWeapon {

    private:
    Crosshair mira;
    int municion;

    public:
    explicit WormBat(int ammo);
    void render(SDL2pp::Renderer& renderer, TextureManager& tex_manager, int x, int y, int flip, int angulo);
    const int getWeapon();
    void increaseAngle();
    void decreaseAngle();
    int getAmmo();
    ~WormBat() {}
};

#endif