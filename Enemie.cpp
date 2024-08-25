#include "Enemie.h"
#include<random>
std::random_device dv;
std::mt19937 gen(dv());
Enemie::Enemie(const int x,const int y, SDL_Texture* enemieShape,SDL_Renderer* renderer,SDL_Rect shipRect):
	x(x),enemieShape(enemieShape),renderer(renderer),shipRect(shipRect){}

 bool Enemie::onDamageEvent(SDL_Rect enemieRect, SDL_Rect shipRect) {
	if (
		shipRect.x + shipRect.w<enemieRect.x ||
		shipRect.x> enemieRect.x + enemieRect.w ||
		shipRect.y + shipRect.h<enemieRect.y ||
		shipRect.y>enemieRect.y + enemieRect.h)
		return false;
	return true;
}