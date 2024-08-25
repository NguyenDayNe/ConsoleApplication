#include "Bullet.h"

int IDCOUNT = 0;
Bullet::Bullet(SDL_Renderer* renderer, SDL_Texture* image, SDL_Rect bulletRect, int bulletDelay,int enemieId) :
	renderer(renderer), image(image), bulletRect(bulletRect), bulletDelay(bulletDelay),enemieId(enemieId) {
	id = IDCOUNT;
	IDCOUNT++;
};

void Bullet::bulletRender(int screenHeight, std::vector<Bullet>& bullets) {
	if (!isDestroy) {
		if (enemieId != 0) {
			SDL_RenderCopy(renderer, image, NULL, &bulletRect);
			bulletCount++;
			if (bulletCount % bulletDelay == 0) bulletRect.y++;
			if (bulletRect.y > screenHeight) destroyBullet(bullets);
		}
		else {
			SDL_RenderCopy(renderer, image, NULL, &bulletRect);
			bulletCount++;
			if (bulletCount % bulletDelay == 0) bulletRect.y--;
			if (bulletRect.y < 0) destroyBullet(bullets);
		}
	}
	else {
		animationCount--;
		SDL_RenderCopy(renderer, image, NULL, &bulletRect);
		if (animationCount == 0) destroyBullet(bullets);
	}
}
void Bullet::destroyBullet(std::vector<Bullet>& bullets) {
	auto it = std::find(bullets.begin(), bullets.end(), *this);
	if (it != bullets.end()) bullets.erase(it);
}