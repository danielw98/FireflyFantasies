#include "include/Game.h"

int main()
{
    GameContext context;
    InitContext(&context);
 
    GameLoop(&context);
    // emscripten_set_main_loop_arg(GameLoop, &context, -1, 1);
    return 0;
}