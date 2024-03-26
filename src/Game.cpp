#include "../include/Game.h"
#include "../include/GUIManager.h"
#include "../include/TextureManager.h"
#include "../include/FontRenderer.h"
#include <map>

Game::Game(GameContext *context)
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
    levelManager = fopen("assets/levelManager.bin", "r");
    if(levelManager == NULL)
    {
        perror("Missing settings file assets/levelManager.bin");
        exit(1);
    }
    sizeX = GetSizeX(level-1);
    sizeY = GetSizeY(level-1);
    board = new Board(sizeX, sizeY);
}

void Game::UpdateBoardAfterSelection(void)
{
    std::list<std::pair<int, int>> visitedCells;
    static bool showingLevelOverFirefly = false;
    static int count = 0;
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
            count = 0;
            level++;
            sizeX = GetSizeX(level-1);
            sizeY = GetSizeY(level-1);
            board->Resize(sizeX, sizeY);
        }
    }
}

int Game::GetSizeX(int level)
{
    int sizeRequested;
    if(level > 4)
        return 10;
    fseek(levelManager, level*2*sizeof(int), SEEK_SET);
    fread(&sizeRequested, sizeof(int), 1, levelManager);
    return sizeRequested;
}

int Game::GetSizeY(int level)
{
    int sizeRequested;
    if(level > 4)
        return 10;
    fseek(levelManager, level*2*sizeof(int) + sizeof(int), SEEK_SET);
    fread(&sizeRequested, sizeof(int), 1, levelManager);
    return sizeRequested;
}

void Game::Update(void)
{
    UpdateBoardAfterSelection();
}

void Game::HandleEvents(void)
{
    while (SDL_PollEvent(&context->event))
    {
        switch (context->event.type)
        {
            case SDL_QUIT: {
                running = false;
                break;
            }
            case SDL_KEYDOWN: {
                switch (context->event.key.keysym.sym) {
                    case SDLK_p: {
                        // toggle pause state
                        paused = !paused;
                        break;
                    }
                    default: {
                        break;
                    }
                }
                break;
            }
            case SDL_MOUSEMOTION: {
                HandleMouseMotion(context->event);
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {
                HandleMouseDown(context->event);
                break;
            }
            case SDL_MOUSEBUTTONUP: {
                HandleMouseUp(context->event);
                break;
            }
            default: {
                //printf("Unhandled event %d\n", event.type);
                break;
            }
        }
    }
}

void Game::HandleMouseUp(SDL_Event& event)
{
    dragging = false;
    selectedFireflyType = FireflyType::FIREFLY_NONE;
    clearSelection = true;
}

void Game::HandleMouseDown(SDL_Event& event)
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
                    
                    prev_i = i;
                    prev_j = j;
                    break;
                }
            }
    }
}

void Game::HandleMouseMotion(SDL_Event &event)
{
    if(dragging == false)
        return;
    SDL_GetMouseState(&context->mouse.x, &context->mouse.y);
    // handle mouse motion
    std::pair<int, int> position;
    for(int i = 0; i < sizeX; i++)
    {

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
                     
                        prev_i = i;
                        prev_j = j;
                        break;
                    }
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

        prev_i = position.first;
        prev_j = position.second;
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

void Game::DisplayBackground(void)
{
    SDL_RenderCopy(GUIManager::GetInstance()->GetRenderer(), TextureManager::GetInstance()->GetBackground(), NULL, NULL);
}

void Game::Render(void)
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

long Game::GetScore(void)
{
    return score;
}
bool Game::IsRunning(void)
{
    return running;
}
bool Game::IsPaused(void)
{
    return paused;
}

void Game::IncrementFrame(void)
{
    context->frameCount++;
}

Game::~Game()
{
    // delete all textures and free all memory and close SDL
    fclose(levelManager);
    delete board;
    delete TextureManager::GetInstance();
    // TODO:
    // delete GUIManager::GetInstance();
    // delete FontRenderer::GetInstance();

    SDL_Quit();
}