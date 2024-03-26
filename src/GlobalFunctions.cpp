#include "../include/GlobalFunctions.h"
#include "../include/Game.h"

static constexpr int  MILLISECONDS_PER_SECOND = 1000;
static constexpr int  SCREEN_WIDTH            = 1600;
static constexpr int  SCREEN_HEIGHT           = 900;
static constexpr int  FPS                     = 30;
static constexpr char WINDOW_TITLE[]          = "Firefly Fantasies";

void GameLoop(void *arg)
{
    GameContext *context = (GameContext*) arg;
    Game game = Game(context);
    const Uint32 frameDelay = MILLISECONDS_PER_SECOND / static_cast<Uint32>(context->fps);
    Uint32 frameStart;
    Uint32 frameTime;
    while (game.IsRunning() == true)
    {
        frameStart = SDL_GetTicks();
        game.HandleEvents();
        if(game.IsPaused() == false)
        {
            game.IncrementFrame();
            game.Update();
            game.Render();
        }
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);
        // else
        //     printf("it takes too much time %u\n", frameDelay - frameTime);
 
    }
    // printf("Game over, your score is %ld\n", game.GetScore());
}
 
void InitContext(GameContext *context)
{
    srand(time(NULL));
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    SDL_Init(SDL_INIT_EVERYTHING);
    context->width = SCREEN_WIDTH;
    context->height = SCREEN_HEIGHT;
    context->title = WINDOW_TITLE;
    context->window = SDL_CreateWindow(context->title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, context->width, context->height, SDL_WINDOW_SHOWN);
    context->renderer = SDL_CreateRenderer(context->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetWindowTitle(context->window, context->title.c_str());
    context->fps = FPS;
}