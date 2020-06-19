#include "font.hpp"

MyFont::MyFont(TTF_Font *sdlFont)
{
    SDLFont = sdlFont;
}

MyFont::~MyFont()
{
    TTF_CloseFont(SDLFont);
}

TTF_Font *MyFont::GetSDLFont()
{
    return SDLFont;
}