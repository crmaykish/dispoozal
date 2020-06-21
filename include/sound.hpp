#ifndef SOUND_H
#define SOUND_H

#include <SDL_mixer.h>

class Sound
{
private:
    Mix_Chunk *Chunk;

public:
    Sound(Mix_Chunk *chunk);
    Mix_Chunk *GetSDLChunk();
};

class MyMusic
{
private:
    Mix_Music *Mus;

public:
    MyMusic(Mix_Music *mus);
    Mix_Music *GetSDLMusic();
};

#endif // SOUND_H