#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "game_geometry.hpp"
#include "texture.hpp"
#include "settings.hpp"

const SDL_Color BG_COLOR = {0x0F, 0x38, 0x0F, 0xFF};
const SDL_Color FG_COLOR = {0x9B, 0xBC, 0x0F, 0xFF};

class SDLRenderer
{
private:
    SDL_Window *SDLWin;
    SDL_Renderer *SDLRender;

public:
    void Init();
    void Clear();
    void Present();
    void Destroy();

    void RenderRectangle(Rectangle rect, int r, int g, int b, int a);
    void RenderWholeTexture(std::shared_ptr<Texture> texture, Rectangle dest);
    void RenderWholeTextureRotate(std::shared_ptr<Texture> texture, Rectangle dest, float angle, Point center);
    void RenderTexture(std::shared_ptr<Texture> texture, Rectangle source, Rectangle dest);
    void RenderTextureRotate(std::shared_ptr<Texture> texture, Rectangle source, Rectangle dest, float angle, Point center);

    std::shared_ptr<Texture> LoadTexture(std::string fileName);
};

#endif // RENDERER_H
