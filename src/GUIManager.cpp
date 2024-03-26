#include "../include/GUIManager.h"

GUIManager* GUIManager::instance = nullptr;

GUIManager* GUIManager::GetInstance(void)
{
    if(instance == nullptr)
        instance = new GUIManager();
    return instance;
}

GUIManager::GUIManager() = default;

void GUIManager::SetRenderer(SDL_Renderer *renderer)
{
    this->renderer = renderer;
}

void GUIManager::SetWindow(SDL_Window *window)
{
    this->window = window;
}

SDL_Renderer* GUIManager::GetRenderer(void)
{
    return renderer;
}

SDL_Window* GUIManager::GetWindow(void)
{
    return window;
}

GUIManager::~GUIManager()
{
    if(instance == nullptr)
        return;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    delete instance;
    instance = nullptr;
}