#pragma once
#include<SDL.h>
#include<SDL_image.h>
class Item
{
public:
	SDL_Renderer* renderer;
	SDL_Rect itemRect;
	SDL_Texture* itemTexture;
	int stack,id;
	void execute();
	Item(SDL_Renderer* renderer,SDL_Texture* itemTexture,const int screenWidth);
	bool operator==(const Item& other) const {
		return id == other.id;
	}
	void deleteItem();
};

