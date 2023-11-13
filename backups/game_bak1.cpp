#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <list>

struct Coords
{
    int i;
    int j;
};

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
    SDL_Texture *screen;
    SDL_Rect screensize;
    SDL_Event event;
    SDL_Color bkg = {0, 255, 255, 255};

    int frameCount;
    int timerFPS;
    int lastFrame;
    int fps;
    int lastTime;
};

enum FireflyType
{
    FIREFLY_1,
    FIREFLY_2,
    FIREFLY_3,
    FIREFLY_NUM,
    FIREFLY_NONE
};

enum FireflyTextureType
{
    FIREFLY_NORMAL_UNSELECTED,
    FIREFLY_NORMAL_SELECTED,
    FIREFLY_HIGHLIGHTED_UNSELECTED,
    FIREFLY_HIGHLIGHTED_SELECTED
};
class GUIManager
{
private:
    SDL_Renderer *renderer;
    SDL_Window *window;
    static GUIManager *instance;
public:
    void SetRenderer(SDL_Renderer *renderer)
    {
        this->renderer = renderer;
    }
    void SetWindow(SDL_Window *window)
    {
        this->window = window;
    }
    SDL_Renderer* GetRenderer(void)
    {
        return renderer;
    }
    SDL_Window *GetWindow(void)
    {
        return window;
    }
public:
    static GUIManager* GetInstance(void)
    {
        if(instance == NULL)
            instance = new GUIManager();
        return instance;
    }
};
GUIManager* GUIManager::instance = NULL;
class TextureManager
{
public:
    static TextureManager *instance;
private:
    std::vector<SDL_Texture*> fireflies;
    std::vector<SDL_Texture*> selectedFireflies;
    std::vector<SDL_Texture*> selectedHighlightedFireflies;
    std::vector<SDL_Texture*> highlightedFireflies;
    SDL_Texture *backgroundTexture;
private:
    TextureManager()
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
        // load background
        backgroundTexture = CreateTextureFromImage("assets/ui/background.png");
    }
    SDL_Texture* CreateTextureFromImage(const char *path)
    {
        SDL_Surface *surface;
        SDL_Texture *texture;
        surface = IMG_Load(path);
        if(surface == NULL){
            fprintf(stderr, "File %s not found\n", path);
            exit(EXIT_FAILURE);
        }
        texture = SDL_CreateTextureFromSurface(GUIManager::GetInstance()->GetRenderer(), surface);
        SDL_FreeSurface(surface);
        return texture;
    }
public:
    SDL_Texture* GetFirefly(FireflyType fireflyType)
    {
        return fireflies[fireflyType];
    }
    SDL_Texture* GetSelectedFirefly(FireflyType fireflyType)
    {
        return selectedFireflies[fireflyType];
    }
    SDL_Texture* GetSelectedHighlightedFirefly(FireflyType fireflyType)
    {
        return selectedHighlightedFireflies[fireflyType];
    }
    SDL_Texture* GetHighlightedFirely(FireflyType fireflyType)
    {
        return highlightedFireflies[fireflyType];
    }
    SDL_Texture* GetBackground(void)
    {
        return backgroundTexture;
    }
public:
    static TextureManager* GetInstance()
    {
        if(instance == NULL)
        {
            instance = new TextureManager();
        }
        return instance;
    }
};
TextureManager* TextureManager::instance = NULL;

class Firefly
{
private:
    int width;
    int height;
    bool selected;
    bool highlighted;
    SDL_Texture *texture;
    SDL_Rect dstRect;
    FireflyType fireflyType;
public:
    Firefly(int i, int j)
    {
        fireflyType = (FireflyType) 1; //(rand() % FireflyType::FIREFLY_NUM);
        // TODO: width and height from config file
        width = 80;
        height = 80;
        // TODO: make the offfset macro
        dstRect.x = 100 + i * width;
        dstRect.y = 100 + j * height;
        if(i % 2 == 0)
            dstRect.y -= height/2;
        dstRect.w = width;
        dstRect.h = height;
        texture = TextureManager::GetInstance()->GetFirefly(fireflyType);
        highlighted = false;
    }
    SDL_Texture* GetFireflyTexture(void)
    {
        return texture;
    }
    void SetFireflyTexture(FireflyTextureType firelyTextureType)
    {
        if(firelyTextureType == FireflyTextureType::FIREFLY_NORMAL_UNSELECTED)
            texture = TextureManager::GetInstance()->GetFirefly(fireflyType);
        if(firelyTextureType == FireflyTextureType::FIREFLY_NORMAL_SELECTED)
            texture = TextureManager::GetInstance()->GetSelectedFirefly(fireflyType);
        if(firelyTextureType == FireflyTextureType::FIREFLY_HIGHLIGHTED_UNSELECTED)
            texture = TextureManager::GetInstance()->GetHighlightedFirely(fireflyType);
        if(firelyTextureType == FireflyTextureType::FIREFLY_HIGHLIGHTED_SELECTED)
            texture = TextureManager::GetInstance()->GetSelectedHighlightedFirefly(fireflyType);
    }
    SDL_Rect GetDstRect(void)
    {
        return dstRect;
    }
    FireflyType GetType(void)
    {
        return fireflyType;
    }
    bool IsSelected(void)
    {
        return selected;
    }
    void SetSelected(bool selected)
    {
        this->selected = selected;
        if(selected == true)
        {
            if(highlighted == false)
                SetFireflyTexture(FireflyTextureType::FIREFLY_NORMAL_SELECTED);
            else
                SetFireflyTexture(FireflyTextureType::FIREFLY_HIGHLIGHTED_SELECTED);
        }
        else
        {
            if(highlighted == false)
                SetFireflyTexture(FireflyTextureType::FIREFLY_NORMAL_UNSELECTED);
            else
                SetFireflyTexture(FireflyTextureType::FIREFLY_HIGHLIGHTED_UNSELECTED);
        }
    }
};

class Board
{
    Firefly **fireflies;
    int sizeX;
    int sizeY;
public:
    Board(int sizeX, int sizeY)
    {
        this->sizeX = sizeX;
        this->sizeY = sizeY;
        fireflies = (Firefly**) malloc(sizeX * sizeof(Firefly*));
        for(int i = 0; i < sizeX; i++)
        {
            fireflies[i] = (Firefly*) malloc(sizeY * sizeof(Firefly));
        }
        for(int i = 0; i < sizeX; i++)
            for(int j = 0; j < sizeY; j++)
            {
                fireflies[i][j] = Firefly(i, j);
                fireflies[i][j].SetSelected(false);
            }
    }
    ~Board()
    {
        for(int i = 0; i < sizeX; i++)
            free(fireflies[i]);
        free(fireflies);
    }
    void DisplayBoard(void)
    {
        for(int i = 0; i < sizeX; i++)
            for(int j = 0; j < sizeY; j++)
            {
                SDL_Rect dstRect = fireflies[i][j].GetDstRect();
                SDL_RenderCopy(GUIManager::GetInstance()->GetRenderer(), 
                                fireflies[i][j].GetFireflyTexture(), 
                                NULL, &dstRect);  
            }
    }

    std::list<Firefly*> AdjacentCells(int i, int j)
    {
        std::list<Firefly*> adjacentFireflies;
        // any piece not in a corner or side of the board has 6 neighbours
        if(i > 0 && j > 0 && i < sizeX && j < sizeY)
        {
            // four of them are left, right, up, down, regardless
            adjacentFireflies.push_back(&fireflies[i-1][j]);
            adjacentFireflies.push_back(&fireflies[i+1][j]);
            adjacentFireflies.push_back(&fireflies[i][j-1]);
            adjacentFireflies.push_back(&fireflies[i][j+1]);
            // for even columns, two of them are up-left, and up-right
            if(j % 2 == 0)
            {
                adjacentFireflies.push_back(&fireflies[i-1][j-1]);
                adjacentFireflies.push_back(&fireflies[i-1][j+1]);
            }
            // for odd columns, two of them are down-left, and down-right
            else
            {
                adjacentFireflies.push_back(&fireflies[i+1][j-1]);
                adjacentFireflies.push_back(&fireflies[i+1][j+1]);
            }
        }
        // any piece on first row that isnt a corner has 3 or 5 neighbours
        if(i == 0 && j != 0 && j != sizeY-1)
        {
            // all of them have the neighbours left, right, and down
            adjacentFireflies.push_back(&fireflies[i][j-1]);
            adjacentFireflies.push_back(&fireflies[i][j+1]);
            adjacentFireflies.push_back(&fireflies[i+1][j]);

            // for odd columns, we have 2 more neighbours: bottom left, bottom right
            if(j % 2 == 1)
            {
                adjacentFireflies.push_back(&fireflies[i+1][j-1]);
                adjacentFireflies.push_back(&fireflies[i+1][j+1]);
            }
        }

        // any piece on last row that isnt a corner has 3 or 5 neighbours
        if(i == sizeX-1 && j != 0 && j != sizeY-1)
        {
            // all of them have neighbours left, right, and up
            adjacentFireflies.push_back(&fireflies[i][j-1]);
            adjacentFireflies.push_back(&fireflies[i][j+1]);
            adjacentFireflies.push_back(&fireflies[i-1][j]);
            // for even columns, we have 2 more neighbours: top left, and top right
            if(j % 2 == 0)
            {
                adjacentFireflies.push_back(&fireflies[i-1][j-1]);
                adjacentFireflies.push_back(&fireflies[i-1][j+1]);
            }
        }

        // any piece on first column that isnt a corner has 4 neighbours
        if(j == 0 && i != 0 && i != sizeX-1)
        {
            // up, down, right, top-right
            adjacentFireflies.push_back(&fireflies[i-1][j]);
            adjacentFireflies.push_back(&fireflies[i+1][j]);
            adjacentFireflies.push_back(&fireflies[i][j+1]);
            adjacentFireflies.push_back(&fireflies[i-1][j+1]);
        }
        // any piece on last column that isnt a corner
        if(j == sizeY-1 && i != 0 && i != sizeX-1)
        {
            // up, down, left, bottom-left
            adjacentFireflies.push_back(&fireflies[i-1][j]);
            adjacentFireflies.push_back(&fireflies[i+1][j]);
            adjacentFireflies.push_back(&fireflies[i][j-1]);
            adjacentFireflies.push_back(&fireflies[i+1][j-1]);
        }
        // top left corner - 2 neighbours
        if(i == 0 && j == 0)
        {
            // bottom, right
            adjacentFireflies.push_back(&fireflies[i+1][j]);
            adjacentFireflies.push_back(&fireflies[i][j+1]);
        }
        // bottom left corner
        if(i == sizeX-1 && j == 0)
        {
            // top, right, top-right
            adjacentFireflies.push_back(&fireflies[i-1][j]);
            adjacentFireflies.push_back(&fireflies[i][j+1]);
            adjacentFireflies.push_back(&fireflies[i-1][j+1]);
        }
        // top right corner
        if(i == 0 && j == sizeY-1)
        {
            // left, bottom for sure
            adjacentFireflies.push_back(&fireflies[i+1][j]);
            adjacentFireflies.push_back(&fireflies[i][j-1]);
            // even number of columns has 1 more neighbour: bottom-left
            if(sizeY % 2 == 0)
            {
                adjacentFireflies.push_back(&fireflies[i+1][j-1]);
            }
        }

        // bottom right corner
        if(i == sizeX-1 && j == sizeY-1)
        {
            // up, left for sure
            adjacentFireflies.push_back(&fireflies[i-1][j]);
            adjacentFireflies.push_back(&fireflies[i+1][j]);
            // odd number of columns has 1 more neighbour: top-left
            if(sizeY % 2 == 0)
            {
                adjacentFireflies.push_back(&fireflies[i-1][j-1]);
            }
        }
        return adjacentFireflies;
    }


    Firefly* GetFirefly(int i, int j)
    {
        return &fireflies[i][j];
    }
};
class Game
{
    GameContext *context;
    Board *board;
    std::list<Firefly*> selectedFireflies;
    FireflyType selectedFireflyType;
    bool running;
    bool paused;
    bool dragging;
    long score;
    int sizeX;
    int sizeY;
    int prev_i;
    int prev_j;
public:
    Game(GameContext *context)
    {
        this->context = context;
        GUIManager::GetInstance()->SetRenderer(context->renderer);
        GUIManager::GetInstance()->SetWindow(context->window);
        running = true;
        paused = false;
        dragging = false;
        // TODO: all configs in a file
        selectedFireflyType = FireflyType::FIREFLY_NONE;
        prev_i = -1;
        prev_j = -1;
        score = 0;
        sizeX = 6;
        sizeY = 6;
        board = new Board(sizeX, sizeY);
    }
    void Update(void)
    {
        
    }
    void InitBoard(void)
    {

    }

    void HandleEvents(void)
    {
        while (SDL_PollEvent(&context->event))
        {
            switch (context->event.type)
            {
            case SDL_QUIT:{
                running = false;
                break;
            }
            case SDL_KEYDOWN:{
                switch (context->event.key.keysym.sym){
                case SDLK_SPACE:{
                    break;
                }
                case SDLK_a:{
                    // toggle left rotation
                    //rotateLeft = !rotateLeft;
                    //rotateRight = false;
                    break;
                }
                case SDLK_d:{
                    // toggle right rotation
                    //rotateRight = !rotateRight;
                    //rotateLeft = false;
                    break;
                }
                case SDLK_p:{
                    // toggle pause state
                    paused = !paused;
                    break;
                }
                case SDLK_r:{
                    //Entity::ToggleRectangleVisible();
                    break;
                }
                case SDLK_LEFT:{
                    //ChangeLevel(-1);
                    break;
                }
                case SDLK_RIGHT:{
                    //ChangeLevel(1);
                    break;
                }
                }
                break;
            }
            case SDL_MOUSEMOTION:{
                HandleMouseMotion(context->event);
                break;
            }
            case SDL_MOUSEBUTTONDOWN:{
                HandleMouseDown(context->event);
                break;
            }
            case SDL_MOUSEBUTTONUP:{
                HandleMouseUp(context->event);
                break;
            }
            default:{
                //printf("Unhandled event %d\n", event.type);
                break;
            }
            }
        }
    }
    void HandleMouseUp(SDL_Event& event)
    {
        dragging = false;
        selectedFireflyType = FireflyType::FIREFLY_NONE;
        for(int i = 0; i < sizeX; i++)
            for(int j = 0; j < sizeY; j++)
                board->GetFirefly(i, j)->SetSelected(false);
    }
    void HandleMouseDown(SDL_Event& event)
    {
        SDL_GetMouseState(&context->mouse.x, &context->mouse.y);
        if (event.button.button == SDL_BUTTON_LEFT && event.button.state == SDL_PRESSED)
        {
            for(int i = 0; i < sizeX; i++)
                for(int j = 0; j < sizeY; j++)
                {
                    Firefly *firefly = board->GetFirefly(i,j);
                    SDL_Rect dstRect = firefly->GetDstRect();
                    if(SDL_PointInRect(&context->mouse, &dstRect))
                    {
                        selectedFireflyType = firefly->GetType();
                        firefly->SetSelected(true);
                        selectedFireflies.push_back(firefly);
                        dragging = true;
                        prev_i = i;
                        prev_j = j;
                        break;
                    }
                }
        }
    }
    void HandleMouseMotion(SDL_Event &event)
    {
        if(dragging == false)
            return;
        SDL_GetMouseState(&context->mouse.x, &context->mouse.y);
        // handle mouse motion

        for(int i = 0; i < sizeX; i++)
            for(int j = 0; j < sizeY; j++)
            {
                Firefly *firefly = board->GetFirefly(i, j);
                SDL_Rect dstRect = firefly->GetDstRect();
                // when the firefly is not selected and the mouse enters we do some checks
                if(firefly->IsSelected() == false && SDL_PointInRect(&context->mouse, &dstRect))
                {
                    // when the type of the selected firefly corresponds to the type of the current selection,
                    // look through the list of neighbours to see if it's adjacent to the cell
                    if(firefly->GetType() == selectedFireflyType)
                    {
                        std::list<Firefly*> neighbours = board->AdjacentCells(prev_i, prev_j);
                        bool isAdjacent = false;
                        for(auto it = neighbours.begin(); it != neighbours.end(); it++)
                            if(*it == firefly)
                                isAdjacent = true;
                   
                        // add the adjacent firefly of the same color to the selected fireflies
                        if(isAdjacent == true)
                        {
                            firefly->SetSelected(true);
                            selectedFireflies.push_back(firefly);
                            prev_i = i;
                            prev_j = j;
                            break;
                        }
                        else
                        {
                            printf("Not adjacent %d %d\n", i, j);
                        }
                    }                        
                }
            }
    }
    void DisplayBackground(void)
    {
        SDL_RenderCopy(GUIManager::GetInstance()->GetRenderer(), TextureManager::GetInstance()->GetBackground(), NULL, NULL);
    }
    
    void Render(void)
    {
        SDL_RenderClear(GUIManager::GetInstance()->GetRenderer());
        DisplayBackground();
        board->DisplayBoard();
        SDL_RenderPresent(GUIManager::GetInstance()->GetRenderer());
    }
    long getScore(void)
    {
        return score;
    }
    bool isRunning(void)
    {
        return running;
    }
    bool isPaused(void)
    {
        return paused;
    }

    void IncrementFrame(void)
    {
        context->frameCount++;
    }
    
    ~Game()
    {
        // delete all textures and free all memory and close SDL
        delete board;
    }
};

void GameLoop(void *arg)
{
    GameContext *context = (GameContext*) arg;

    Game game = Game(context);
    game.InitBoard();
    const int FPS = context->fps;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    Uint32 frameTime;
    while (game.isRunning())
    {
        frameStart = SDL_GetTicks();
        game.HandleEvents();
        if(game.isPaused() == false)
        {
            game.IncrementFrame();
            game.Update();
            game.Render();
        }
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);
        else
            std::cout << "it takes too much time " << (int)frameDelay - (int)frameTime << "\n";
        
    }
    SDL_Quit();
    printf("Game over, your score is %ld\n", game.getScore());
}

void InitContext(GameContext *context)
{
    srand(time(NULL));
    SDL_Init(SDL_INIT_VIDEO);
    context->width = 800;
    context->height = 600;
    SDL_CreateWindowAndRenderer(context->width, context->height, 0, &context->window, &context->renderer);
    SDL_SetWindowTitle(context->window, context->title.c_str());
    context->title = "web test";
    context->fps = 30;
}

int main()
{
    GameContext context;
    InitContext(&context);

    GameLoop(&context);
    return 0;
}