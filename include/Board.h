#ifndef BOARD_H_
#define BOARD_H_

#include "Firefly.h"
#include <list>

class Board {
private:
    Firefly ***fireflies;
    Firefly *finalFirefly;
    int sizeX;
    int sizeY;
    std::list<std::pair<int, int>> visitedCells;
    bool levelOverFirely;
    int numHighlighted;
    int boardSize;
public:
    Board(int sizeX, int sizeY);
    void DisplayBoard(void);
    bool ShowLevelOverFirefly(void);
    void SetLevelOverFirefly(bool val);
    void SetFinalFirefly(Firefly *firefly);
    std::list<Firefly*> AdjacentCells(int i, int j);
    void SetCellVisited(std::pair<int, int> cell);
    bool IsCellVisited(std::pair<int, int> cell);
    void RemoveCell(std::pair<int, int> cell);
    Firefly* GetFirefly(int i, int j);
    void SetFirefly(int i, int j, Firefly *firefly);
    bool IsLevelOver(void);
    void HandleFirefliesCombo(int highlightedThisFrame);
    void SetupFireflies(void);
    void Resize(int newSizeX, int newSizeY);
    void DeleteFireflies(void);
    ~Board();
};

#endif /* BOARD_H_ */
