#include "../include/FontRenderer.h"
#include "../include/GUIManager.h"

FontRenderer* FontRenderer::instance = nullptr;

FontRenderer* FontRenderer::GetInstance()
{
    if(instance == nullptr)
        instance = new FontRenderer();
    return instance;
}

FontRenderer::FontRenderer()
{
    if(TTF_Init() == -1){
        fprintf(stderr,"Could not initialize TTF library: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    font = TTF_OpenFont("../assets/fonts/OpenSans-Regular.ttf", 36);
    if(font == NULL){
        fprintf(stderr, "Loading font failed: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    } 
}
 
void FontRenderer::DisplayText(const char *msg, int x, int y)
{
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Color color = {.r = 0, .g = 0, .b = 0, .a = SDL_ALPHA_OPAQUE};
    SDL_Rect rect;
 
    surface = TTF_RenderText_Solid(font, msg, color);
    texture = SDL_CreateTextureFromSurface(GUIManager::GetInstance()->GetRenderer(), surface);
 
    rect.x = x;
    rect.y = y;
    rect.w = surface->w;
    rect.h = surface->h;
 
    SDL_RenderCopy(GUIManager::GetInstance()->GetRenderer(), texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

FontRenderer::~FontRenderer()
{
    TTF_CloseFont(font);
    TTF_Quit();
    instance = nullptr;
}