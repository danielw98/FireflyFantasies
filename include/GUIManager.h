#ifndef GUI_MANAGER_H_
#define GUI_MANAGER_H_

#include <SDL2/SDL.h>

class GUIManager {
private:
    SDL_Renderer *renderer;
    SDL_Window *window;

public:
    static GUIManager* GetInstance(void);
private:
    static GUIManager *instance;
    // private constructor to assure the singleton pattern
    GUIManager();

public:
    void SetRenderer(SDL_Renderer *renderer);
    void SetWindow(SDL_Window *window);
    SDL_Renderer* GetRenderer(void);
    SDL_Window *GetWindow(void);
    ~GUIManager();
};

#endif /* GUI_MANAGER_H_ */