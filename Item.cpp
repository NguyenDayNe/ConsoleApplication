#include "Item.h"
#include "Enemie.h"
#include<random>

const int ITEM_VERTICAL_SPEED_SLOWDOWN = 8;
const int ITEM_SIZE = 50;
int currentItemId = 0;

Item::Item(SDL_Renderer* renderer,SDL_Texture* itemTexture,const int screenWidth)
	:renderer(renderer),itemTexture(itemTexture){
	std::uniform_int_distribution<> dis(0, screenWidth);
	itemRect = { dis(gen),0,ITEM_SIZE,ITEM_SIZE };
	stack = 0;
	id = currentItemId;
	currentItemId++;
}

void Item::execute() {
	stack++;
	if (stack % ITEM_VERTICAL_SPEED_SLOWDOWN==0) itemRect.y++;
	SDL_RenderCopy(renderer, itemTexture, NULL, &itemRect);
}
void Item :: deleteItem() {
	delete this;
}