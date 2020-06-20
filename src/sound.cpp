#include "sound.hpp"

Sound::Sound(Mix_Chunk *chunk)
{
    Chunk = chunk;
}

Mix_Chunk *Sound::GetSDLChunk()
{
    return Chunk;
}