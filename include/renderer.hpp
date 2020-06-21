#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "game_geometry.hpp"
#include "texture.hpp"
#include "font.hpp"
#include "sound.hpp"
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

    // Texture rendering
    void RenderRectangle(Rectangle rect, int r, int g, int b, int a);
    void RenderWholeTexture(std::shared_ptr<Texture> texture, Rectangle dest);
    void RenderWholeTextureRotate(std::shared_ptr<Texture> texture, Rectangle dest, float angle, Point center);
    void RenderTexture(std::shared_ptr<Texture> texture, Rectangle source, Rectangle dest);
    void RenderTextureRotate(std::shared_ptr<Texture> texture, Rectangle source, Rectangle dest, float angle, Point center);

    // Font rendering
    void RenderFont(std::shared_ptr<MyFont> font, std::string text, Rectangle dest, SDL_Color color);

    void PlaySound(std::shared_ptr<Sound> sound);
    void PlayMusic(std::shared_ptr<MyMusic> music);

    // Resource loading
    std::shared_ptr<Texture> LoadTexture(std::string fileName);
    std::shared_ptr<MyFont> LoadFont(std::string fileName);
    std::shared_ptr<Sound> LoadSound(std::string fileName);
    std::shared_ptr<MyMusic> LoadMusic(std::string fileName);
};

#endif // RENDERER_H
