#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SDL2pp/SDL2pp.hh>
#include <map>
#include <string>

#include "path_loader.h"
class TextureManager {

    private:
        PathLoader assests;
        std::map<std::string, std::shared_ptr<SDL2pp::Texture> > sprites;
        
        std::shared_ptr<SDL2pp::Music> music;

        std::shared_ptr<SDL2pp::Texture> background;

        std::map<std::string, std::shared_ptr<SDL2pp::Chunk> > sonidos;

    public:

    explicit TextureManager();


    void loadTexture(SDL2pp::Renderer& renderer);
    void loadBackground(SDL2pp::Renderer& renderer);
    void loadMusic();
    void loadSounds();

    std::shared_ptr<SDL2pp::Texture> getTexture(std::string texture_path);
    std::shared_ptr<SDL2pp::Texture> getBackground();//pasarlo a map si hay varios fondos distintos
    std::shared_ptr<SDL2pp::Music> getMusic();//pasarlo a map si hay varias canciones distintas
    std::shared_ptr<SDL2pp::Chunk> getSound(std::string sound_path);
    
};

#endif
