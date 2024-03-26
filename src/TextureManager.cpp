#include "../include/TextureManager.h"
#include "../include/GUIManager.h"
#include <SDL2/SDL_image.h>

TextureManager* TextureManager::instance = nullptr;

TextureManager* TextureManager::GetInstance(void)
{
    if(instance == nullptr)
        instance = new TextureManager();
    return instance;
}

TextureManager::TextureManager()
{
    // load all firefly types
    for(int i = 0; i < FireflyType::FIREFLY_NUM; i++)
    {
        char fileName[128];
        // normal firefly
        sprintf(fileName, "assets/fireflies/firefly_%d.png", i+1);
        fireflies.push_back(CreateTextureFromImage(fileName));
        // selected firefly
        sprintf(fileName, "assets/fireflies/firefly_selected_%d.png", i+1);
        selectedFireflies.push_back(CreateTextureFromImage(fileName));
        // selected highlighted firefly
        sprintf(fileName, "assets/fireflies/firefly_selected_highlighted_%d.png", i+1);
        selectedHighlightedFireflies.push_back(CreateTextureFromImage(fileName));
        // highlighted firefly
        sprintf(fileName, "assets/fireflies/firefly_highlighted_%d.png", i+1);
        highlightedFireflies.push_back(CreateTextureFromImage(fileName));
    }

    // normal bomb
    fireflies.push_back(CreateTextureFromImage("assets/fireflies/bomb.png"));
    // selected bomb
    selectedFireflies.push_back(CreateTextureFromImage("assets/fireflies/bomb_selected.png"));
    // selected highlighted bomb
    selectedHighlightedFireflies.push_back(CreateTextureFromImage("assets/fireflies/bomb_selected_highlighted.png"));
    // highlighted bomb
    highlightedFireflies.push_back(CreateTextureFromImage("assets/fireflies/bomb_highlighted.png"));

    // wildcard
    fireflies.push_back(CreateTextureFromImage("assets/fireflies/wildcard.png"));
    // selected wildcard
    selectedFireflies.push_back(CreateTextureFromImage("assets/fireflies/wildcard_selected.png"));
    // selected highlighted wildcard
    selectedHighlightedFireflies.push_back(CreateTextureFromImage("assets/fireflies/wildcard_selected_highlighted.png"));
    // highlighted wildcard
    highlightedFireflies.push_back(CreateTextureFromImage("assets/fireflies/wildcard_highlighted.png"));

    // final firefly
    fireflyFinal = CreateTextureFromImage("assets/fireflies/final_firefly.png");

    // arrow
    arrow = CreateTextureFromImage("assets/fireflies/arrow.png");

    // load background
    backgroundTexture = CreateTextureFromImage("assets/ui/background.png");
}

SDL_Texture* TextureManager::CreateTextureFromImage(const char *path)
{
    SDL_Surface *surface;
    SDL_Texture *texture;

    surface = IMG_Load(path);
    if(surface == NULL)
    {
        fprintf(stderr, "File %s not found\n", path);
        exit(EXIT_FAILURE);
    }

    texture = SDL_CreateTextureFromSurface(GUIManager::GetInstance()->GetRenderer(), surface);

    SDL_FreeSurface(surface);
    
    return texture;
}

SDL_Texture* TextureManager::GetFirefly(FireflyType fireflyType)
{
    return fireflies[fireflyType];
}

SDL_Texture* TextureManager::GetSelectedFirefly(FireflyType fireflyType)
{
    return selectedFireflies[fireflyType];
}

SDL_Texture* TextureManager::GetSelectedHighlightedFirefly(FireflyType fireflyType)
{
    return selectedHighlightedFireflies[fireflyType];
}

SDL_Texture* TextureManager::GetHighlightedFirely(FireflyType fireflyType)
{
    return highlightedFireflies[fireflyType];
}

SDL_Texture* TextureManager::GetBackground(void)
{
    return backgroundTexture;
}

SDL_Texture* TextureManager::GetFinalFirefly(void)
{
    return fireflyFinal;
}

SDL_Texture* TextureManager::GetArrow(void)
{
    return arrow;
}

TextureManager::~TextureManager()
{
    if(instance == nullptr)
        return;
    
    for(auto firefly : fireflies)
        SDL_DestroyTexture(firefly);
    fireflies.clear();

    for(auto firefly: selectedFireflies)
        SDL_DestroyTexture(firefly);
    selectedFireflies.clear();

    for(auto firefly: selectedHighlightedFireflies)
        SDL_DestroyTexture(firefly);
    selectedHighlightedFireflies.clear();
    
    SDL_DestroyTexture(fireflyFinal);
    SDL_DestroyTexture(arrow);
    SDL_DestroyTexture(backgroundTexture);

    instance = nullptr;
}
