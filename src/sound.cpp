#include "sound.hpp"

Sound::Sound(Mix_Chunk *chunk)
{
    Chunk = chunk;
}

Mix_Chunk *Sound::GetSDLChunk()
{
    return Chunk;
}

MyMusic::MyMusic(Mix_Music *mus)
{
    Mus = mus;
}

Mix_Music *MyMusic::GetSDLMusic()
{
    return Mus;
}