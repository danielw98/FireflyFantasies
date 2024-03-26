#ifndef GAME_H_
#define GAME_H_

#include "structs/GameContext.h"
#include "Board.h"
#include "GlobalFunctions.h"
#include <vector>

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
    void UpdateBoardAfterSelection(void);
public:
    Game(GameContext *context);
    int GetSizeX(int level);
    int GetSizeY(int level);
    void Update(void);
    void HandleEvents(void);
    void HandleMouseUp(SDL_Event& event);
    void HandleMouseDown(SDL_Event& event);
    void HandleMouseMotion(SDL_Event &event);
    void DisplayBackground(void);
    void Render(void);
    long GetScore(void);
    bool IsRunning(void);
    bool IsPaused(void);
    void IncrementFrame(void);
    ~Game();
};

#endif /* GAME_H_ */
