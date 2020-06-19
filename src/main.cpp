#include "game.hpp"

int main(int argv, char **args)
{
    Game g;

    g.Init();
    g.Loop();
    g.Close();

    return 0;
}
