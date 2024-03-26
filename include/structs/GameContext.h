#ifndef GAME_CONTEXT_H_
#define GAME_CONTEXT_H_

#include <string>
#include <SDL2/SDL.h>

struct GameContext
{
    std::string title;
    int width;
    int height;
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Point mouse;
    const Uint8 *keystates;
    Uint32 mousestate;
    SDL_Rect screensize;
    SDL_Event event;
    SDL_Color bkg = {0, 255, 255, 255};
 
    int frameCount;
    int timerFPS;
    int lastFrame;
    int fps;
    int lastTime;
};

#endif /* GAME_CONTEXT_H_ */