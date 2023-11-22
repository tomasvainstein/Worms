#include "worm_weapon.h"

class WormNoWeapon : public WormWeapon {

    public:
    explicit WormNoWeapon() {}
    void render(SDL2pp::Renderer& renderer, TextureManager& tex_manager, int x, int y, int flip, int angulo) {}
    const int getWeapon() {return NO_WEAPON;}
    ~WormNoWeapon() {}
};