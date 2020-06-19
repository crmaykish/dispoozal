#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <SDL.h>

class Texture
{
private:
    SDL_Texture *SDLTexture;

public:
    Texture(SDL_Texture *sdlTexture);
    ~Texture();

    SDL_Texture *GetSDLTexture();
};

#endif // TEXTURE_HPP
