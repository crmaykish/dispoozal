#include "renderer.hpp"
#include "logger.hpp"

const std::string WINDOW_TITLE = "Dispoozal";

static SDL_Rect ConvertRect(Rectangle r, bool flipAndScale = false)
{
    SDL_Rect n = {(int)std::round(r.pos.x),
                  (int)std::round(r.pos.y),
                  (int)std::round(r.size.w),
                  (int)std::round(r.size.h)};

    if (flipAndScale)
    {
        // Flip and scale the position
        n.x *= (RESOLUTION_W / WORLDSIZE_W);
        n.y = (WORLDSIZE_H - n.y - n.h) * (RESOLUTION_H / WORLDSIZE_H);

        // Scale the size
        n.w *= (RESOLUTION_W / WORLDSIZE_W);
        n.h *= (RESOLUTION_H / WORLDSIZE_H);
    }

    return n;
}

// SDLRenderer

void SDLRenderer::Init()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0)
    {
        Log("Failed to initialize SDL: " + std::string(SDL_GetError()), LOG_ERROR);
        exit(1);
    }

    if (TTF_Init() != 0)
    {
        Log("Failed to initialize TTF: " + std::string(TTF_GetError()), LOG_ERROR);
        exit(1);
    }

    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        Log("Failed to initialize IMG: " + std::string(IMG_GetError()), LOG_ERROR);
        exit(1);
    }

    if (Mix_Init(MIX_INIT_MOD | MIX_INIT_MP3) == 0)
    {
        Log("Failed to initialize Mixer: " + std::string(Mix_GetError()), LOG_ERROR);
        exit(1);
    }

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    // Create the SDL Window
    SDLWin = SDL_CreateWindow(WINDOW_TITLE.c_str(),
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              RESOLUTION_W, RESOLUTION_H,
                              SDL_WINDOW_SHOWN);

    if (SDLWin == NULL)
    {
        Log("Failed to create SDL window: " + std::string(SDL_GetError()), LOG_ERROR);
        exit(1);
    }

    // Create an SDL renderer and attach it to the window
    SDLRender = SDL_CreateRenderer(SDLWin,
                                   -1,
                                   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (SDLRender == NULL)
    {
        Log("Failed to create SDL renderer: " + std::string(SDL_GetError()), LOG_ERROR);
        exit(1);
    }

    // Set render scaling

    // SDL_RenderSetScale(SDLRender, RENDER_SCALE, RENDER_SCALE);

    // Set the SDL blend mode to BLEND
    if (SDL_SetRenderDrawBlendMode(SDLRender, SDL_BLENDMODE_BLEND) != 0)
    {
        Log("Failed to set SDL blend mode: " + std::string(SDL_GetError()), LOG_ERROR);
        exit(1);
    }

    // Hide the OS cursor image
    SDL_ShowCursor(SDL_DISABLE);

    // Open the SDL window
    SDL_ShowWindow(SDLWin);
}

void SDLRenderer::Clear()
{
    SDL_SetRenderDrawColor(SDLRender, BG_COLOR.r, BG_COLOR.g, BG_COLOR.b, BG_COLOR.a);
    SDL_RenderClear(SDLRender);
}

void SDLRenderer::Present()
{
    SDL_RenderPresent(SDLRender);
}
void SDLRenderer::Destroy()
{
    SDL_DestroyRenderer(SDLRender);
    SDL_DestroyWindow(SDLWin);
    SDL_Quit();
}

void SDLRenderer::RenderRectangle(Rectangle rect, int r, int g, int b, int a)
{
    SDL_SetRenderDrawColor(SDLRender, r, g, b, a);
    SDL_Rect d = ConvertRect(rect, true);
    SDL_RenderFillRect(SDLRender, &d);
}

void SDLRenderer::RenderWholeTexture(std::shared_ptr<Texture> texture, Rectangle dest)
{
    SDL_Rect d = ConvertRect(dest, true);
    SDL_RenderCopy(SDLRender, texture->GetSDLTexture(), NULL, &d);
}

void SDLRenderer::RenderWholeTextureRotate(std::shared_ptr<Texture> texture, Rectangle dest, float angle, Point center)
{
    SDL_Rect d = ConvertRect(dest, true);
    SDL_Point c = {(int)center.x, (int)center.y};
    // Note: angle is negative becacause world coordinates are upside down from rendering coordinates
    SDL_RenderCopyEx(SDLRender, texture->GetSDLTexture(), NULL, &d, -angle, &c, SDL_FLIP_NONE);
}

void SDLRenderer::RenderTexture(std::shared_ptr<Texture> texture, Rectangle source, Rectangle dest)
{
    SDL_Rect s = ConvertRect(source);
    SDL_Rect d = ConvertRect(dest, true);
    SDL_RenderCopy(SDLRender, texture->GetSDLTexture(), &s, &d);
}

void SDLRenderer::RenderTextureRotate(std::shared_ptr<Texture> texture, Rectangle source, Rectangle dest, float angle, Point center)
{
    SDL_Rect s = ConvertRect(source);
    SDL_Rect d = ConvertRect(dest, true);
    SDL_Point c = {(int)center.x, (int)center.y};
    // Note: angle is negative becacause world coordinates are upside down from rendering coordinates
    SDL_RenderCopyEx(SDLRender, texture->GetSDLTexture(), &s, &d, -angle, &c, SDL_FLIP_NONE);
}

void SDLRenderer::RenderFont(std::shared_ptr<MyFont> font, std::string text, Rectangle dest, SDL_Color color)
{
    SDL_Surface *s = TTF_RenderText_Blended(font->GetSDLFont(), text.c_str(), color);
    SDL_Texture *t = SDL_CreateTextureFromSurface(SDLRender, s);

    SDL_Rect r = ConvertRect(dest, true);

    SDL_RenderCopy(SDLRender, t, NULL, &r);

    SDL_FreeSurface(s);
    SDL_DestroyTexture(t);

    TTF_RenderText_Solid(font->GetSDLFont(), text.c_str(), color);
}

void SDLRenderer::PlaySound(std::shared_ptr<Sound> sound)
{
    Mix_PlayChannel(-1, sound->GetSDLChunk(), 0);
}

void SDLRenderer::PlayMusic(std::shared_ptr<MyMusic> music)
{
    Mix_PlayMusic(music->GetSDLMusic(), -1);
}

std::shared_ptr<Texture> SDLRenderer::LoadTexture(std::string fileName)
{
    Log("Loading texture: " + fileName, LOG_INFO);

    SDL_Texture *tex = IMG_LoadTexture(SDLRender, fileName.c_str());

    if (tex == NULL)
    {
        Log("Failed to load texture: " + fileName, LOG_ERROR);
    }

    return std::make_shared<Texture>(tex);

    // TextureMap.insert({fileName, IMG_LoadTexture(Renderer, fileName.c_str())});

    // // This could probably be more efficient;
    // return TextureMap.at(fileName);
}

std::shared_ptr<MyFont> SDLRenderer::LoadFont(std::string fileName)
{
    Log("Loading font: " + fileName, LOG_INFO);

    TTF_Font *font = TTF_OpenFont(fileName.c_str(), 80);

    if (font == NULL)
    {
        Log("Failed to load font: " + fileName, LOG_ERROR);
    }

    return std::make_shared<MyFont>(font);
}

std::shared_ptr<Sound> SDLRenderer::LoadSound(std::string fileName)
{
    Mix_Chunk *m = Mix_LoadWAV(fileName.c_str());

    return std::make_shared<Sound>(m);
}

std::shared_ptr<MyMusic> SDLRenderer::LoadMusic(std::string fileName)
{
    Mix_Music *m = Mix_LoadMUS(fileName.c_str());

    return std::make_shared<MyMusic>(m);
}