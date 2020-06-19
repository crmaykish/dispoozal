#ifndef FONT_H
#define FONT_H

#include <memory>
#include <SDL_ttf.h>

class MyFont
{
private:
    TTF_Font *SDLFont;

public:
    MyFont(TTF_Font *sdlFont);
    ~MyFont();

    TTF_Font *GetSDLFont();
};

#endif // FONT_H
