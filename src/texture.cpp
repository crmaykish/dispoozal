#include "texture.hpp"

Texture::Texture(SDL_Texture *sdlTexture)
{
    SDLTexture = sdlTexture;
}

Texture::~Texture()
{
    SDL_DestroyTexture(SDLTexture);
}

SDL_Texture *Texture::GetSDLTexture()
{
    return SDLTexture;
}
