#include "../include/Firefly.h"
#include "../include/TextureManager.h"
#include "../include/enums/FireflyTextureType.h"
#include "../include/Firefly.h"

static constexpr int OFFSET_X       = 550;
static constexpr int OFFSET_Y       = 100;
static constexpr int FIREFLY_WIDTH  = 80;
static constexpr int FIREFLY_HEIGHT = 80;

Firefly::Firefly(int i, int j, bool highlighted)
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

Firefly::Firefly(SDL_Texture *finalFirefly, int sizeX)
{
    fireflyType = FireflyType::FIREFLY_FINAL;
    dstRect = {.x = OFFSET_X, .y = OFFSET_Y, .w = FIREFLY_WIDTH, .h = FIREFLY_HEIGHT};
    texture = finalFirefly;
}

SDL_Texture* Firefly::GetFireflyTexture(void)
{
    return texture;
}

void Firefly::SetFireflyTexture(FireflyTextureType firelyTextureType)
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

std::pair<int, int> Firefly::GetPosition(void)
{
    return position;
}

void Firefly::SetPosition(int i, int j)
{
    position.first = i;
    position.second = j;
    dstRect.x = OFFSET_X + i * width;
    dstRect.y = OFFSET_Y + j * height;
    if(i % 2 == 0)
        dstRect.y -= height/2;
}

SDL_Rect Firefly::GetDstRect(void)
{
    return dstRect;
}

FireflyType Firefly::GetType(void)
{
    return fireflyType;
}

bool Firefly::IsSelected(void)
{
    return selected;
}
    
void Firefly::UpdateTexture(void)
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

void Firefly::SetSelected(bool selected)
{
    this->selected = selected;
    UpdateTexture();
}

void Firefly::SetHighlighted(bool highlighted)
{
    this->highlighted = highlighted;
    UpdateTexture();
}

bool Firefly::GetHighlighted(void)
{
    return highlighted;
}
