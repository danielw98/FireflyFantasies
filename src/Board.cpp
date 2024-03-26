#include "../include/Board.h"
#include "../include/GUIManager.h"
#include "../include/Board.h"

Board::Board(int sizeX, int sizeY)
{
    this->sizeX = sizeX;
    this->sizeY = sizeY;
    this->boardSize = sizeX * sizeY;
    this->numHighlighted = 0;
    this->levelOverFirely = false;
    this->finalFirefly = nullptr;

    SetupFireflies();
}

void Board::DisplayBoard(void)
{
    for(int i = 0; i < sizeX; i++)
    {
        for(int j = 0; j < sizeY; j++)
        {
            if(fireflies[i][j] == nullptr)
                continue;
            SDL_Rect dstRect = fireflies[i][j]->GetDstRect();
            SDL_RenderCopy(GUIManager::GetInstance()->GetRenderer(), fireflies[i][j]->GetFireflyTexture(), NULL, &dstRect);
        }
    }
    if(finalFirefly != nullptr)
    {
        SDL_Rect dstRect = finalFirefly->GetDstRect();
        SDL_RenderCopy(GUIManager::GetInstance()->GetRenderer(), finalFirefly->GetFireflyTexture(), NULL, &dstRect);
    }
}

bool Board::ShowLevelOverFirefly(void)
{
    return levelOverFirely;
}

void Board::SetLevelOverFirefly(bool val)
{
    levelOverFirely = val;
}

void Board::SetFinalFirefly(Firefly *firefly)
{
    finalFirefly = firefly;
}

std::list<Firefly*> Board::AdjacentCells(int i, int j)
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
            adjacentFireflies.push_back(fireflies[i-1][j-1]);
        
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
        if(sizeX % 2 == 1)
            adjacentFireflies.push_back(fireflies[i-1][j-1]);
        else
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

void Board::SetCellVisited(std::pair<int, int> cell)
{
    visitedCells.push_back(cell);
}

bool Board::IsCellVisited(std::pair<int, int> cell)
{
    for(auto it = visitedCells.begin(); it != visitedCells.end(); it++)
        if(it->first == cell.first && it->second == cell.second)
            return true;
    return false;
}

void Board::RemoveCell(std::pair<int, int> cell)
{
    visitedCells.remove(cell);
}

Firefly* Board::GetFirefly(int i, int j)
{
    return fireflies[i][j];
}

void Board::SetFirefly(int i, int j, Firefly *firefly)
{
    fireflies[i][j] = firefly;
}

bool Board::IsLevelOver(void)
{
    return boardSize == numHighlighted;
}

void Board::HandleFirefliesCombo(int highlightedThisFrame)
{
    // TODO: aici si scorul - in fct de highlightedThisFrame
    numHighlighted = 0;
    for(int i = 0; i < sizeX; i++)
        for(int j = 0; j < sizeY; j++)
            if(fireflies[i][j]->GetHighlighted() == true)
                numHighlighted++;
}

void Board::DeleteFireflies(void)
{
    for(int i = 0; i < sizeX; i++)
        for(int j = 0; j < sizeY; j++)
            delete fireflies[i][j];
    
    for(int i = 0; i < sizeX; i++)
        delete[] fireflies[i];

    delete[] fireflies;
}

void Board::Resize(int newSizeX, int newSizeY)
{
    visitedCells.clear();
    
    DeleteFireflies();

    sizeX = newSizeX;
    sizeY = newSizeY;
    boardSize = sizeX * sizeY;
    numHighlighted = 0;
    
    SetupFireflies();
}

void Board::SetupFireflies(void)
{
    fireflies = new Firefly**[sizeX];
    for(int i = 0; i < sizeX; i++)
    {
        fireflies[i] = new Firefly*[sizeY];
    }
    for(int i = 0; i < sizeX; i++)
    {
        for(int j = 0; j < sizeY; j++)
        {
            fireflies[i][j] = new Firefly(i, j);
            fireflies[i][j]->SetSelected(false);
            fireflies[i][j]->SetHighlighted(false);
        }
    }
}

Board::~Board()
{
    DeleteFireflies();
    delete finalFirefly;
}