#ifndef TEXTURE_MANAGER_H_
#define TEXTURE_MANAGER_H_

#include "../include/enums/FireflyType.h"
#include <vector>
#include <SDL2/SDL.h>

class TextureManager {
private:
    std::vector<SDL_Texture*> fireflies;
    std::vector<SDL_Texture*> selectedFireflies;
    std::vector<SDL_Texture*> selectedHighlightedFireflies;
    std::vector<SDL_Texture*> highlightedFireflies;
 
    SDL_Texture *fireflyFinal;
    SDL_Texture *arrow;
    SDL_Texture *backgroundTexture;

public:
    static TextureManager* GetInstance(void);
private:
    static TextureManager *instance;
    // private constructor to assure the singleton pattern
    TextureManager();

public:
    SDL_Texture* GetFirefly(FireflyType fireflyType);
    SDL_Texture* GetSelectedFirefly(FireflyType fireflyType);
    SDL_Texture* GetSelectedHighlightedFirefly(FireflyType fireflyType);
    SDL_Texture* GetHighlightedFirely(FireflyType fireflyType);
    SDL_Texture* GetBackground(void);
    SDL_Texture* GetFinalFirefly(void);
    SDL_Texture* GetArrow(void);
    ~TextureManager();
private:
    SDL_Texture* CreateTextureFromImage(const char *path);
};

#endif /* TEXTURE_MANAGER_H_ */
