#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <utility>
 
#define OFFSET_X 550
#define OFFSET_Y 100
 
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
    FIREFLY_BOMB = 3,
    FIREFLY_WILD,
    FIREFLY_FINAL,
    FIREFLY_ANY,
    FIREFLY_NONE = -1
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
 
    // SDL_Texture *bomb;
    // SDL_Texture *selectedBomb;
    // SDL_Texture *highlightedBomb;
    // SDL_Texture *selectedHighlightedBomb;
 
    // SDL_Texture *wildcard;
    // SDL_Texture *selectedWildcard;
    // SDL_Texture *highlightedWildcard;
    // SDL_Texture *selectedHighlightedWildcard;
 
    SDL_Texture *fireflyFinal;
 
    SDL_Texture *arrow;
 
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
    SDL_Texture* GetFinalFirefly(void)
    {
        return fireflyFinal;
    }
    SDL_Texture* GetArrow(void)
    {
        return arrow;
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
 
class FontRenderer
{
public:
    ~FontRenderer();
    static FontRenderer* getInstance();
    void DisplayText(const char *msg, int x, int y);
private:
    FontRenderer();
    static FontRenderer *instance;
    TTF_Font *font;
};
 
FontRenderer* FontRenderer::instance = NULL;
FontRenderer::FontRenderer()
{
    if(TTF_Init() == -1){
        fprintf(stderr,"Could not initialize TTF library: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    font = TTF_OpenFont("../assets/fonts/Poppins-SemiBold.ttf", 36);
    if(font == NULL){
        fprintf(stderr, "Loading font failed: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    } 
}
FontRenderer::~FontRenderer()
{
    TTF_CloseFont(font);
    TTF_Quit();
    instance = NULL;
}
 
FontRenderer* FontRenderer::getInstance()
{
    if(!instance)
        instance = new FontRenderer;
    return instance;
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
    std::pair<int, int> position;
public:
    Firefly(int i, int j, bool highlighted = false)
    {
        fireflyType = (FireflyType) (rand() % FireflyType::FIREFLY_NUM);
        int bombOrWild = rand() % 100;
        if(bombOrWild == 10)
            fireflyType = FireflyType::FIREFLY_BOMB;
        if(bombOrWild == 20)
            fireflyType = FireflyType::FIREFLY_WILD;
        // TODO: width and height from config file
        width = 80;
        height = 80;
        // TODO: make the offfset macro
        dstRect.x = OFFSET_X + i * width;
        dstRect.y = OFFSET_Y + j * height;
        position.first = i;
        position.second = j;
        if(i % 2 == 0)
            dstRect.y -= height/2;
        dstRect.w = width;
        dstRect.h = height;
        this->highlighted = highlighted;
        this->selected = false;
        UpdateTexture();
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
    std::pair<int, int> GetPosition(void)
    {
        return position;
    }
    void SetPosition(int i, int j)
    {
        position.first = i;
        position.second = j;
        dstRect.x = OFFSET_X + i * width;
        dstRect.y = OFFSET_Y + j * height;
        if(i % 2 == 0)
            dstRect.y -= height/2;
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
        
    void UpdateTexture(void)
    {
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
    void SetSelected(bool selected)
    {
        this->selected = selected;
        UpdateTexture();
    }
 
    void SetHighlighted(bool highlighted)
    {
        this->highlighted = highlighted;
        UpdateTexture();
    }
 
    bool GetHighlighted(void)
    {
        return this->highlighted;
    }
};
 
class Board
{
    Firefly ***fireflies;
    int sizeX;
    int sizeY;
    std::list<std::pair<int, int>> visitedCells;
    int numHighlighted;
    int boardSize;
public:
    Board(int sizeX, int sizeY)
    {
        this->sizeX = sizeX;
        this->sizeY = sizeY;
        this->boardSize = sizeX * sizeY;
        this->numHighlighted = 0;
        fireflies = (Firefly***) malloc(sizeX * sizeof(Firefly**));
        for(int i = 0; i < sizeX; i++)
        {
            fireflies[i] = (Firefly**) malloc(sizeY * sizeof(Firefly*));
        }
        for(int i = 0; i < sizeX; i++)
            for(int j = 0; j < sizeY; j++)
            {
                fireflies[i][j] = new Firefly(i, j);
                fireflies[i][j]->SetSelected(false);
            }
    }
    ~Board()
    {
        // for(int i = 0; i < sizeX; i++)
        //     for(int j = 0; j < sizeY; j++)
        //         if(fireflies[i][j] != nullptr)
        //             delete fireflies[i][j];
        // for(int i = 0; i < sizeX; i++)
        //     free(fireflies[i]);
        // free(fireflies);
    }
    void DisplayBoard(void)
    {
        for(int i = 0; i < sizeX; i++)
            for(int j = 0; j < sizeY; j++)
            {
                if(fireflies[i][j] == nullptr)
                    continue;
                SDL_Rect dstRect = fireflies[i][j]->GetDstRect();
                SDL_RenderCopy(GUIManager::GetInstance()->GetRenderer(), fireflies[i][j]->GetFireflyTexture(), NULL, &dstRect);
            }
    }
 
    std::list<Firefly*> AdjacentCells(int i, int j)
    {
        std::list<Firefly*> adjacentFireflies;
 
        // top left corner - 2 neighbours
        if(i == 0 && j == 0)
        {
            // bottom, right
            adjacentFireflies.push_back(fireflies[i+1][j]);
            adjacentFireflies.push_back(fireflies[i][j+1]);
            return adjacentFireflies;
        }
        // top right corner
        if(i == sizeX-1 && j == 0)
        {
            // top, right, top-right
            adjacentFireflies.push_back(fireflies[i-1][j]);
            adjacentFireflies.push_back(fireflies[i][j+1]);
            adjacentFireflies.push_back(fireflies[i-1][j+1]);
            return adjacentFireflies;
        }
        // bottom left corner
        if(i == 0 && j == sizeY-1)
        {
            // left, bottom for sure
            adjacentFireflies.push_back(fireflies[i+1][j]);
            adjacentFireflies.push_back(fireflies[i][j-1]);
            // even number of columns has 1 more neighbour: bottom-left
            if(sizeY % 2 == 0)
            {
                adjacentFireflies.push_back(fireflies[i+1][j-1]);
            }
            return adjacentFireflies;
        }
        // bottom right corner
        if(i == sizeX-1 && j == sizeY-1)
        {
            // up, left for sure
            adjacentFireflies.push_back(fireflies[i-1][j]);
            adjacentFireflies.push_back(fireflies[i][j-1]);
            // odd number of columns has 1 more neighbour: top-left
            if(sizeY % 2 == 1)
            {
                adjacentFireflies.push_back(fireflies[i-1][j-1]);
            }
            return adjacentFireflies;
        }
 
        // any piece on first column that isnt a corner has 4 neighbours
        if(i == 0 && j != 0 && j != sizeY-1)
        {
            // all of them have the neighbours left, right, and down
            adjacentFireflies.push_back(fireflies[i][j-1]);
            adjacentFireflies.push_back(fireflies[i][j+1]);
            adjacentFireflies.push_back(fireflies[i+1][j]);
            adjacentFireflies.push_back(fireflies[i+1][j-1]);
            return adjacentFireflies;
        }
 
        // last column
        if(i == sizeX-1 && j != 0 && j != sizeY-1)
        {
            adjacentFireflies.push_back(fireflies[i][j-1]);
            adjacentFireflies.push_back(fireflies[i][j+1]);
            adjacentFireflies.push_back(fireflies[i-1][j]);
            adjacentFireflies.push_back(fireflies[i-1][j+1]);
            return adjacentFireflies;
        }
 
        // any piece on first row that isnt a corner has 3 or 5 neighbours
        if(j == 0 && i != 0 && i != sizeX-1)
        {
            adjacentFireflies.push_back(fireflies[i-1][j]);
            adjacentFireflies.push_back(fireflies[i+1][j]);
            adjacentFireflies.push_back(fireflies[i][j+1]);
            if(i % 2 == 1)
            {
                adjacentFireflies.push_back(fireflies[i-1][j+1]);
                adjacentFireflies.push_back(fireflies[i+1][j+1]);
            }
            return adjacentFireflies;
        }
 
        // any piece on last row that isnt a corner has 3 or 5 neighbours
        if(j == sizeY-1 && i != 0 && i != sizeX-1)
        {
            // up, down, left
            adjacentFireflies.push_back(fireflies[i-1][j]);
            adjacentFireflies.push_back(fireflies[i+1][j]);
            adjacentFireflies.push_back(fireflies[i][j-1]);
            if(i % 2 == 0)
            {
                // bottom left, top left
                adjacentFireflies.push_back(fireflies[i-1][j-1]);
                adjacentFireflies.push_back(fireflies[i+1][j-1]);
            }
            return adjacentFireflies;
        }
 
        // any piece not in a corner or side of the board has 6 neighbours
        if(i > 0 && j > 0 && i < sizeX && j < sizeY)
        {
            // four of them are left, right, up, down, regardless
            adjacentFireflies.push_back(fireflies[i-1][j]);
            adjacentFireflies.push_back(fireflies[i+1][j]);
            adjacentFireflies.push_back(fireflies[i][j-1]);
            adjacentFireflies.push_back(fireflies[i][j+1]);
            // for even columns, two of them are up-left, and up-right
            if(i % 2 == 0)
            {
                adjacentFireflies.push_back(fireflies[i-1][j-1]);
                adjacentFireflies.push_back(fireflies[i+1][j-1]);
            }
            // for odd columns, two of them are down-left, and down-right
            else
            {
                adjacentFireflies.push_back(fireflies[i-1][j+1]);
                adjacentFireflies.push_back(fireflies[i+1][j+1]);
            }
            return adjacentFireflies;
        }
        return adjacentFireflies;
    }
 
    void SetCellVisited(std::pair<int, int> cell)
    {
        visitedCells.push_back(cell);
    }
    bool IsCellVisited(std::pair<int, int> cell)
    {
        for(auto it = visitedCells.begin(); it != visitedCells.end(); it++)
            if(it->first == cell.first && it->second == cell.second)
                return true;
        return false;
    }
    void RemoveCell(std::pair<int, int> cell)
    {
        visitedCells.remove(cell);
    }
    Firefly* GetFirefly(int i, int j)
    {
        return fireflies[i][j];
    }
    void SetFirefly(int i, int j, Firefly *firefly)
    {
        fireflies[i][j] = firefly;
    }
    bool IsLevelOver(void)
    {
        return boardSize == numHighlighted;
    }
    void HandleFirefliesCombo(int highlightedThisFrame)
    {
        // TODO: aici si scorul - in fct de highlightedThisFrame
        numHighlighted = 0;
        for(int i = 0; i < sizeX; i++)
            for(int j = 0; j < sizeY; j++)
                if(fireflies[i][j]->GetHighlighted() == true)
                    numHighlighted++;
    }
};
class Game
{
    GameContext *context;
    Board *board;
    std::vector<Firefly*> selectedFireflies;
    FireflyType selectedFireflyType;
    bool running;
    bool paused;
    bool dragging;
    bool clearSelection;
    long score;
    int level;
    int sizeX;
    int sizeY;
    int prev_i;
    int prev_j;
    FILE *levelManager;
    std::list<std::pair<std::pair<int, int>, float>> selectedFirefliesArrows;
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
        level = 1;
        prev_i = -1;
        prev_j = -1;
        score = 0;
        sizeX = 6;
        sizeY = 6;
        board = new Board(sizeX, sizeY);
        levelManager = fopen("levelManager.bin", "r");
        if(levelManager == NULL)
        {
            perror("Missing settings file levelManager.bin");
            exit(1);
        }
    }
 
    void UpdateBoardAfterSelection(void)
    {
        std::list<std::pair<int, int>> visitedCells;
        // the selection is cleared when the clearSelection variable is set, on mouse up
        if (clearSelection)
        {
            // selections smaller than a size of 3 are marked unselected
            if(selectedFireflies.size() < 3)
            {
                for (auto it = selectedFireflies.begin(); it != selectedFireflies.end(); it++)
                {
                    Firefly *firefly = *it;
                    firefly->SetSelected(false);
                }
                selectedFirefliesArrows.clear();
                selectedFireflies.clear();
                clearSelection = false;
                return;
            }
 
            // each selected firefly is nulled and marked as unselected in the board
            // its cell is set to visited so that it will be highlighted when another firefly drops
            int *numNullsPerColumn = (int*) malloc(sizeY * sizeof(int));
            memset(numNullsPerColumn, 0, sizeY*sizeof(int));
 
            std::map<std::pair<int, int>, bool> isSelectedFireflyHighlighted;
            std::list<Firefly*> firefliesThatAreBombs;
            for (auto it = selectedFireflies.begin(); it != selectedFireflies.end(); it++)
            {
                Firefly *firefly = *it;
                if(firefly == nullptr)
                    continue;
 
                std::pair<int, int> cell = firefly->GetPosition();
 
                if(firefly->GetType() == FireflyType::FIREFLY_BOMB)
                    firefliesThatAreBombs.push_back(firefly);
 
                firefly->SetSelected(false);
                board->SetCellVisited(cell);
                visitedCells.push_back(cell);
                board->SetFirefly(cell.first, cell.second, nullptr); 
                numNullsPerColumn[cell.first]++;
                isSelectedFireflyHighlighted[cell] = firefly->GetHighlighted();
                delete firefly;
            }
 
            // recursively explode all bombs on columns and lines
            while(firefliesThatAreBombs.size() > 0)
            {
                Firefly *firefly = firefliesThatAreBombs.back();
                firefliesThatAreBombs.pop_back();
                std::pair<int, int> cell = firefly->GetPosition();
                int i = cell.first;
                int j = cell.second;
                for(int ii = 0; ii < sizeX; ii++)
                {
                    Firefly *crt = board->GetFirefly(ii, j);
                    if(crt == nullptr)
                        continue;
                    if(crt->GetType() == FireflyType::FIREFLY_BOMB)
                        firefliesThatAreBombs.push_back(crt);
                    crt->SetSelected(false);
                    board->SetCellVisited(crt->GetPosition());
                    visitedCells.push_back(crt->GetPosition());
                    board->SetFirefly(ii, j, nullptr); 
                    numNullsPerColumn[ii]++;
                    isSelectedFireflyHighlighted[crt->GetPosition()] = crt->GetHighlighted();
 
                }
                for(int jj = 0; jj < sizeY; jj++)
                {
                    Firefly *crt = board->GetFirefly(i, jj);
                    if(crt == nullptr)
                        continue;
                    if(crt->GetType() == FireflyType::FIREFLY_BOMB)
                        firefliesThatAreBombs.push_back(crt);
                    crt->SetSelected(false);
                    board->SetCellVisited(crt->GetPosition());
                    visitedCells.push_back(crt->GetPosition());
                    board->SetFirefly(i, jj, nullptr); 
                    numNullsPerColumn[i]++;
                    isSelectedFireflyHighlighted[crt->GetPosition()] = crt->GetHighlighted();
                }
            }
            std::list<std::pair<std::pair<int, int>, Firefly*>> indicesToMoveDown;
            for (int i = 0; i < sizeX; i++)
            {
                int indicesAboveNulls = 0;
                for (int j = 0; j < sizeY; j++)
                {
                    int numNulls = 0;
                    Firefly *firefly = board->GetFirefly(i, j);
                    if(firefly == nullptr)
                        continue;
                    // count the nulls (selected fireflies that were destroyed) under the current firefly
                    for(int j1 = j+1; j1 < sizeY; j1++)
                    {
                        if(board->GetFirefly(i, j1) == nullptr)
                        {
                            numNulls++;
                        }
                    }
                    if(numNulls > 0)
                    {
                        indicesToMoveDown.push_back(std::make_pair(std::make_pair(i, j+numNulls), firefly));
                    }
                }
            }
 
            // move all fireflies that are above the nulls to their new position
            for(auto it = indicesToMoveDown.begin(); it != indicesToMoveDown.end(); it++)
            {
                bool prevHighlighted = isSelectedFireflyHighlighted[it->first];
                board->SetFirefly(it->first.first, it->first.second, it->second);
                board->GetFirefly(it->first.first, it->first.second)->SetPosition(it->first.first, it->first.second);
                board->GetFirefly(it->first.first, it->first.second)->SetHighlighted(prevHighlighted);
                // if(prevHighlighted == true)
                //     board->GetFirefly(it->first.first, it->first.second)->SetFireflyTexture(FireflyTextureType::FIREFLY_HIGHLIGHTED_UNSELECTED);
                // else
                //     board->GetFirefly(it->first.first, it->first.second)->SetFireflyTexture(FireflyTextureType::FIREFLY_NORMAL_UNSELECTED);
            }
 
            // nulling fireflies at the top of the columns that have selected fireflies in them
            // so that new ones can be created instead
            for(int i = 0; i < sizeX; i++)
                for(int j = 0; j < numNullsPerColumn[i]; j++)
                    board->SetFirefly(i, j, nullptr);
 
            // creating new fireflies for all null firelies remaining at the end
            for (int i = 0; i < sizeX; i++)
            {
                for (int j = 0; j < sizeY; j++)
                {
                    if (board->GetFirefly(i, j) == nullptr)
                    {
                        //bool highlighted = board->IsCellVisited(std::make_pair(i, j));
                        Firefly *newFirefly = new Firefly(i, j, false);
                        board->SetFirefly(i, j, newFirefly);
                    }
                }
            }
 
            // highlight all fireflies where the board says they should be highlighted
            int highlightedThisFrame = 0;
            for (int i = 0; i < sizeX; i++)
            {
                for (int j = 0; j < sizeY; j++)
                {
                    if(board->IsCellVisited(std::make_pair(i, j)))
                    {
                        Firefly *firefly = board->GetFirefly(i, j);
                        if(firefly->GetHighlighted() == false)
                            highlightedThisFrame++;
                        firefly->SetHighlighted(true);
                    }
                }
            }
 
            for (int i = 0; i < sizeX; i++)
            {
                for (int j = 0; j < sizeY; j++)
                {
                    Firefly *firefly = board->GetFirefly(j, i);
                    if(firefly == nullptr)
                        printf("%d ", -1);
                    else
                        printf(" %d ", (int) board->GetFirefly(j,i)->GetType());
                }
                printf("\n");
            }
            printf("\n");
 
            firefliesThatAreBombs.clear();
            isSelectedFireflyHighlighted.clear();
            visitedCells.clear();
            selectedFirefliesArrows.clear();
            selectedFireflies.clear();
            indicesToMoveDown.clear();
            free(numNullsPerColumn);
            clearSelection = false;
            board->HandleFirefliesCombo(highlightedThisFrame);
            if(board->IsLevelOver())
            {
                delete board;
                level++;
                sizeX = GetSizeX(level);
                sizeY = GetSizeY(level);
                board = new Board(sizeX, sizeY);
            }

        }
    }
    int GetSizeX(int level)
    {
        int sizeRequested;
        fseek(levelManager, level*2*sizeof(int), SEEK_SET);
        fread(&sizeRequested, sizeof(int), 1, levelManager);
        return sizeRequested;
    }
    int GetSizeY(int level)
    {
        int sizeRequested;
        fseek(levelManager, level*2*sizeof(int) + sizeof(int), SEEK_SET);
        fread(&sizeRequested, sizeof(int), 1, levelManager);
        return sizeRequested;
    }
    void Update(void)
    {
        UpdateBoardAfterSelection();
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
        clearSelection = true;
    }
    void HandleMouseDown(SDL_Event& event)
    {
        SDL_GetMouseState(&context->mouse.x, &context->mouse.y);
        if (event.button.button == SDL_BUTTON_LEFT && event.button.state == SDL_PRESSED)
        {
            for(int i = 0; i < sizeX; i++)
                for(int j = 0; j < sizeY; j++)
                {
                    Firefly *firefly = board->GetFirefly(i, j);
                    if(firefly == nullptr)
                        continue;
 
                    SDL_Rect dstRect = firefly->GetDstRect();
                    if(SDL_PointInRect(&context->mouse, &dstRect))
                    {
                        selectedFireflyType = firefly->GetType();
                        if(selectedFireflyType == FireflyType::FIREFLY_WILD || selectedFireflyType == FireflyType::FIREFLY_BOMB)
                            selectedFireflyType = FireflyType::FIREFLY_ANY;
 
                        firefly->SetSelected(true);
                        selectedFireflies.push_back(firefly);
                        dragging = true;
                        // std::list<Firefly*> neighbours = board->AdjacentCells(i, j);
                        // bool isAdjacent = false;
                        // printf("(%d, %d): ", i, j);
                        // for(auto it = neighbours.begin(); it != neighbours.end(); it++)
                        // {
                        //     Firefly *neighbour = *it;
                        //     if(neighbour->GetType() == firefly->GetType())
                        //         printf("(%d, %d), ", neighbour->GetPosition().first, neighbour->GetPosition().second);
                        // }
                        // printf("\n");
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
        std::pair<int, int> position;
        for(int i = 0; i < sizeX; i++)
            for(int j = 0; j < sizeY; j++)
            {
                Firefly *firefly = board->GetFirefly(i, j);
                if(firefly == nullptr)
                    continue;
                SDL_Rect dstRect = firefly->GetDstRect();
                // save the position where the mouse currently is to be accessible after the for for
                if(SDL_PointInRect(&context->mouse, &dstRect))
                    position = std::make_pair(i, j);
                // when the firefly is not selected and the mouse enters we do some checks
                if(firefly->IsSelected() == false && SDL_PointInRect(&context->mouse, &dstRect))
                {
                    // when the type of the selected firefly corresponds to the type of the current selection,
                    // look through the list of neighbours to see if it's adjacent to the cell
                    if((firefly->GetType() == selectedFireflyType) || (firefly->GetType() == FireflyType::FIREFLY_WILD) || 
                       (selectedFireflyType == FireflyType::FIREFLY_ANY) || (firefly->GetType() == FireflyType::FIREFLY_BOMB))
                    {
                        if(selectedFireflyType == FireflyType::FIREFLY_ANY)
                            selectedFireflyType = firefly->GetType();
                        if(firefly->GetType() == FireflyType::FIREFLY_WILD || selectedFireflyType == FireflyType::FIREFLY_BOMB)
                            selectedFireflyType = FireflyType::FIREFLY_ANY;
 
                        std::list<Firefly*> neighbours = board->AdjacentCells(prev_i, prev_j);
                        std::list<Firefly*> neighboursCrt = board->AdjacentCells(i, j);
                        bool isAdjacent = false;
                        for(auto it = neighbours.begin(); it != neighbours.end(); it++)
                        {
                            Firefly *neighbour = *it;
                            if(neighbour == firefly)
                            {
                                isAdjacent = true;
                                break;
                            }
                        }
                        printf("(%d, %d): ", i, j);
                        for(auto it = neighboursCrt.begin(); it != neighboursCrt.end(); it++)
                        {
                            Firefly *neighbour = *it;
                            if(neighbour->GetType() == selectedFireflies.back()->GetType())
                                printf("(%d, %d), ", neighbour->GetPosition().first, neighbour->GetPosition().second);
                        }
                        printf("\n");
                        // add the adjacent firefly of the same color to the selected fireflies
                        if(isAdjacent == true)
                        {
                            firefly->SetSelected(true);
                            selectedFireflies.push_back(firefly);
                            // draw the arrow
                            SDL_Rect prevFireflyRect = board->GetFirefly(prev_i, prev_j)->GetDstRect();
                            SDL_Rect crtFireflyRect  = board->GetFirefly(i, j)->GetDstRect();
                            std::pair<int, int> vec1 = std::make_pair(prevFireflyRect.x + prevFireflyRect.w/2, prevFireflyRect.y + prevFireflyRect.h/2);
                            std::pair<int, int> vec2 = std::make_pair(crtFireflyRect.x + crtFireflyRect.w/2, crtFireflyRect.y + crtFireflyRect.h/2);
                            std::pair<int, int> vec  = std::make_pair(vec2.first - vec1.first, vec2.second - vec1.second);
                            std::pair<int, int> arrowPos = std::make_pair((vec1.first + vec2.first)/2, (vec1.second + vec2.second)/2);
                            float theta = atan2f(vec.second, vec.first);
                            selectedFirefliesArrows.push_back(std::make_pair(arrowPos, theta));
                            // for(int idx = 0; idx < selectedFireflies.size(); idx++)
                            // {
                            //     printf("%d, %d\n", selectedFireflies[idx]->GetPosition().first, selectedFireflies[idx]->GetPosition().second);
                            // }
                            // printf("--\n");
                            prev_i = i;
                            prev_j = j;
                            break;
                        }
                    }                        
                }
            }
 
        Firefly *prevFirefly = nullptr;
        Firefly *lastFirefly = selectedFireflies.back();
        if(selectedFireflies.size() >= 2)
        {
            prevFirefly = selectedFireflies[selectedFireflies.size() - 2];
        }
 
        if(prevFirefly != nullptr && prevFirefly->GetPosition().first == position.first && prevFirefly->GetPosition().second == position.second)
        {
            lastFirefly->SetSelected(false);
            selectedFireflies.pop_back();
            selectedFireflyType = selectedFireflies.back()->GetType();
 
            selectedFirefliesArrows.pop_back();
 
            // board->RemoveCell(lastFirefly->GetPosition());
            prev_i = position.first;
            prev_j = position.second;
            // for(int idx = 0; idx < selectedFireflies.size(); idx++)
            // {
            //     printf("%d, %d\n", selectedFireflies[idx]->GetPosition().first, selectedFireflies[idx]->GetPosition().second);
            // }
            // printf("--\n");
        }
        if(selectedFireflyType == FireflyType::FIREFLY_BOMB || selectedFireflyType == FireflyType::FIREFLY_WILD)
        {
            if(prevFirefly != nullptr)
            {
                selectedFireflyType = prevFirefly->GetType();
                if(selectedFireflyType == FireflyType::FIREFLY_WILD)
                    selectedFireflyType = FireflyType::FIREFLY_ANY;
            }
            else
                selectedFireflyType = FireflyType::FIREFLY_ANY;
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
        for(auto it = selectedFirefliesArrows.begin(); it != selectedFirefliesArrows.end(); it++)
        {
            std::pair<std::pair<int, int>, float> arrow = *it;
            int width = 40;
            int height = 20;
            SDL_Rect arrowRect = (SDL_Rect) {.x = arrow.first.first-width/2, .y = arrow.first.second-height/2, .w = width, .h = height};
            double angle = arrow.second * 180 / M_PI;
            SDL_RenderCopyEx(GUIManager::GetInstance()->GetRenderer(), TextureManager::GetInstance()->GetArrow(), NULL, &arrowRect, angle, NULL, SDL_FLIP_NONE);
        }
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
        // else
        //     std::cout << "it takes too much time " << (int)frameDelay - (int)frameTime << "\n";
 
    }
    SDL_Quit();
    printf("Game over, your score is %ld\n", game.getScore());
}
 
void InitContext(GameContext *context)
{
    srand(time(NULL));
    SDL_Init(SDL_INIT_VIDEO);
    context->width = 1600;
    context->height = 900;
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