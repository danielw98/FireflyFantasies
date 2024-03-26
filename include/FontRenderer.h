#ifndef FONT_RENDERER_H_
#define FONT_RENDERER_H_

#include <SDL2/SDL_ttf.h>

class FontRenderer {
private:
    TTF_Font *font;
public:
    static FontRenderer* GetInstance();
    static FontRenderer *instance;
private:
    FontRenderer();
public:
    ~FontRenderer();
    void DisplayText(const char *msg, int x, int y);
};

#endif /* FONT_RENDERER_H_ */
