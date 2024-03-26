#ifndef FIREFLY_H_
#define FIREFLY_H_

#include "../include/enums/FireflyType.h"
#include "../include/enums/FireflyTextureType.h"
#include <SDL2/SDL.h>
#include <utility>

class Firefly {
private:
    int width;
    int height;
    bool selected;
    bool highlighted;
    SDL_Texture *texture;
    SDL_Rect dstRect;
    FireflyType fireflyType;
    std::pair<int, int> position;
public:
    Firefly(int i, int j, bool highlighted = false);
    Firefly(SDL_Texture *finalFirefly, int sizeX);
    SDL_Texture* GetFireflyTexture(void);
    void SetFireflyTexture(FireflyTextureType firelyTextureType);
    std::pair<int, int> GetPosition(void);
    void SetPosition(int i, int j);
    SDL_Rect GetDstRect(void);
    FireflyType GetType(void);
    bool IsSelected(void);
    void SetSelected(bool selected);
    void SetHighlighted(bool highlighted);
    bool GetHighlighted(void);
private:
    void UpdateTexture(void);
};

#endif /* FIREFLY_H_ */