#include<SDL.h>
#include<SDL_mixer.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<iostream>
#include<vector>
#include<string>
#include "Music.h"
#include "SmallEnemie.h"
#include "Bullet.h"
#include <Windows.h>
#include "Item.h"


using namespace std;

const int SCREEN_WIDTH = 1500;
const int SCREEN_HEIGHT = 780;
const int SPACE_SHIP_SPEED = 2;
const int SPACE_SHIP_SIZE = 60;
const int HP_SIZE = 200;
const int HP_SIZE_HEIGHT = 20;
const int PADDING = 10;
const int ENEMIES_FREQUENCY_APPEAR = 3000;
const int ENEMIES_MEDIUM_FREQUENCY = 6000;
const int ENEMIES_LARGE_FREQUENCY = 12000;
const int MINIMUM_FEQUENCY_ENEMIE_APEAR = 500;
const int BACKGROUND_SPEED = 10;
const int HP = 10000;
const int HP_BULLET_COST = 500;
const int SHIP_SHOT_FREQUENTCY = 150;
const int SPACE_BULLET_SIZE = 30;
const int SUMMARY_ANALOG_SIZE_W = 300;
const int SUMMARY_ANALOG_SIZE_H = 200;
const int HOME_CONTINUE_SIZE = 30;
const int BOOST_FREQUENCY = 10000;
const int ITEM_FREQUENCY = 30000;


const int ENEMIE_LOST_HP_BY_BULLET = 2000;


SDL_Rect musicRect = { 750,500,30,30 };
SDL_Rect newGameRect = { 700,300,150,50 };
SDL_Rect exitRect = { 700,400,150,50 };
SDL_Rect pauseRect = { PADDING,PADDING,30,30 };
SDL_Rect mapRect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

SDL_Rect hpBarRect = { SCREEN_WIDTH - HP_SIZE - PADDING, PADDING ,HP_SIZE, HP_SIZE_HEIGHT };
SDL_Rect scoreRect = { 2 * PADDING + pauseRect.w,PADDING,20,30 };
const int scoreLength = 20;
bool openMusic = true;

std::string shot1Path = "image/enemies/animation/Shots/1.png";
std::string shot2Path = "image/enemies/animation/Shots/shot2_2.png";
std::string shot3Path = "image/enemies/animation/Shots/shot3_3.png";
std::string shot4Path = "image/enemies/animation/Shots/shot4_4.png";
std::string shot5Path = "image/enemies/animation/Shots/shot5_5.png";
std::string shot6Path = "image/enemies/animation/Shots/shot6_6.png";

std::string shot1EPath = "image/enemies/animation/Shots/shot1.png";
std::string shot2EPath = "image/enemies/animation/Shots/shot2.png";
std::string shot3EPath = "image/enemies/animation/Shots/shot3.png";
std::string shot4EPath = "image/enemies/animation/Shots/shot4.png";
std::string shot5EPath = "image/enemies/animation/Shots/shot5.png";
std::string shot6EPath = "image/enemies/animation/Shots/shot6.png";

std::string itemPath = "image/item/heal.png";

std::string shipShotPath = "image/spaceShip/bullet.png";
std::string shipShotExplosionPath = "image/spaceShip/explosion.png";

TTF_Font* font;
std::string fontPath = "font/suse.ttf";
SDL_Surface* fontSurface;
SDL_Texture* fontTexture;

//  INIT SDL

void SDL_Init(SDL_Window*& window, SDL_Renderer*& renderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) cout << "ERROR";
    window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_MAXIMIZED | SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);
    initMix();
    playMusicBackground();
    TTF_Init();
    font = TTF_OpenFont(fontPath.c_str(), 24);
}

//  QUIT SDL

void QuitSDL(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

void rendertest(SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
}
/*
    IMAGE LOADER
    CAN USE IMG_LoadTexture, NO NEED TO WRITE THIS FUNTION
*/

SDL_Texture* loadTexture(const string &file,SDL_Renderer* renderer) {
    return IMG_LoadTexture(renderer, file.c_str());
}

//  BUTTON CLICKED EVENT

bool onButtonClicked(SDL_Rect buttonRect, int mouseX, int mouseY) {
    return (mouseX > buttonRect.x && mouseX < buttonRect.x + buttonRect.w &&
        mouseY > buttonRect.y && mouseY < buttonRect.y + buttonRect.h);
}

//  RENDER GAME COMPONANT

void gameRender(SDL_Renderer* renderer,SDL_Texture* background,
    int currentYBackground,SDL_Texture* spaceShip,
    SDL_Texture* backButton,SDL_Rect spaceShipRect) {
    SDL_Rect back1 = { 0,currentYBackground,SCREEN_WIDTH,SCREEN_HEIGHT };
    SDL_Rect back2 = { 0,currentYBackground - SCREEN_HEIGHT,SCREEN_WIDTH,SCREEN_HEIGHT };
    
    SDL_RenderCopy(renderer, background, NULL, &back1);
    SDL_RenderCopy(renderer, background, NULL, &back2);
    SDL_RenderCopy(renderer, spaceShip, NULL, &spaceShipRect);
    SDL_RenderCopy(renderer, backButton, NULL, &pauseRect);
    
}


//  UPDATE HP BAR

void hpUpdate(int x,SDL_Renderer* renderer,SDL_Texture* health,SDL_Texture* healthBar) {
    SDL_Rect length = hpBarRect;
    length.w = (int)x * hpBarRect.w / HP;
    SDL_RenderCopy(renderer, healthBar, NULL, &hpBarRect);
    SDL_RenderCopy(renderer, health, NULL, &length);
}


void enemieShot(vector<Bullet>& bullets) {
    for (Bullet& bullet : bullets) {
        bullet.bulletRender(SCREEN_HEIGHT, bullets);
    }
}

void scoreRender(SDL_Renderer* renderer,int score,
    SDL_Rect& scoreRect,SDL_Color textColor) {
    SDL_DestroyTexture(fontTexture);
    SDL_FreeSurface(fontSurface);
    int n = score, dem = 0;
    while (n > 0) {
        n = n / 10;
        dem++;
    }
    if (score == 0) dem = 1;
    dem += 6; //length of "SCORE "
    scoreRect.w = dem * scoreLength;
    
    fontSurface = TTF_RenderText_Solid(font, ("SCORE "+to_string(score)).c_str(), textColor);
    fontTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);
    SDL_RenderCopy(renderer, fontTexture, NULL, &scoreRect);
}

void gameOverTextRender(SDL_Renderer* renderer, SDL_Rect rect, SDL_Color textColor) {
    SDL_DestroyTexture(fontTexture);
    SDL_FreeSurface(fontSurface);
    rect.w = scoreLength * 9;
    rect.x = rect.x - rect.w / 2;
    fontSurface = TTF_RenderText_Solid(font, "GAME OVER", textColor);
    fontTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);
    SDL_RenderCopy(renderer, fontTexture, NULL, &rect);
}

void changeScoreRectSummary(SDL_Rect& rect,SDL_Rect summary,int score) {
    int n = score, dem = 0;
    while (n > 0) {
        n = n / 10;
        dem++;
    }
    if (score == 0) dem = 1;
    dem += 6; //length of "SCORE "
    rect.w = dem * scoreLength;
   
} 
//  GAMEPLAY

void gamePlay(SDL_Renderer* renderer,bool* quit) {
    uniform_int_distribution<> dis35(3, 4);
    uniform_int_distribution<> dis13(1, 3);
    uniform_int_distribution<> dis26(2, 6);
    uniform_int_distribution<> dise26(200, 600);
    uniform_int_distribution<> dis12(1, 2);
    int enemieCreateCount=0;

    SDL_Rect spaceShipRect = { SCREEN_WIDTH / 2,SCREEN_HEIGHT - SPACE_SHIP_SIZE,SPACE_SHIP_SIZE,SPACE_SHIP_SIZE };

    int currentYBackground = 0;
    SDL_Texture* background= loadTexture("image/background/background.png", renderer);
    SDL_Texture* spaceShip = loadTexture("image/Componant/spaceShip/ships/blue.png", renderer);
    SDL_Texture* backButton = loadTexture("image/ui/back.png",renderer);
    SDL_Texture* pauseButton = loadTexture("image/ui/pause.png", renderer);
    SDL_Texture* homeButton = loadTexture("image/ui/home.png", renderer);
    SDL_Texture* board = loadTexture("image/ui/board.png", renderer);

    SDL_Texture* enemieRandom1 = loadTexture("image/enemies/ship/Ship1/Ship1.png", renderer);
    SDL_Texture* enemieRandom2 = loadTexture("image/enemies/ship/Ship2/Ship2.png", renderer);
    SDL_Texture* enemieRandom3 = loadTexture("image/enemies/ship/Ship3/Ship3.png", renderer);
    SDL_Texture* enemieRandom4 = loadTexture("image/enemies/ship/Ship4/Ship4.png", renderer);
    SDL_Texture* enemieRandom5 = loadTexture("image/enemies/ship/Ship5/Ship5.png", renderer);
    SDL_Texture* enemieRandom6 = loadTexture("image/enemies/ship/Ship6/Ship6.png", renderer);

    SDL_Texture* healthBar = loadTexture("image/Componant/healthBar.png", renderer);
    SDL_Texture* health = loadTexture("image/Componant/health.png", renderer);

    SDL_Rect summary = { SCREEN_WIDTH / 2 - SUMMARY_ANALOG_SIZE_W / 2,
                SCREEN_HEIGHT / 2 - SUMMARY_ANALOG_SIZE_H / 2,
                SUMMARY_ANALOG_SIZE_W,SUMMARY_ANALOG_SIZE_H };
    SDL_Rect summaryScoreRect = { summary.x + summary.w / 2,summary.y + summary.h / 2 - 30,
        60,60 };
    SDL_Rect homeButtonRect = { summary.x+summary.w/2-30,summary.y+summary.h-60,60,60 };

    vector<SmallEnemie> enemies;
    vector<Bullet> bullets;
    SDL_Texture* shots[12];
    shots[0] = loadTexture(shot1Path, renderer); shots[6] = loadTexture(shot1EPath, renderer);
    shots[1] = loadTexture(shot2Path, renderer); shots[7] = loadTexture(shot2EPath, renderer);
    shots[2] = loadTexture(shot3Path, renderer); shots[8] = loadTexture(shot3EPath, renderer);
    shots[3] = loadTexture(shot4Path, renderer); shots[9] = loadTexture(shot4EPath, renderer);
    shots[4] = loadTexture(shot5Path, renderer); shots[10] = loadTexture(shot5EPath, renderer);
    shots[5] = loadTexture(shot6Path, renderer); shots[11] = loadTexture(shot6EPath, renderer);

    SDL_Texture* shipShot = loadTexture(shipShotPath,renderer);
    SDL_Texture* shipShotDestroy = loadTexture(shipShotExplosionPath,renderer);
    SDL_Texture* itemTexture = loadTexture(itemPath, renderer);
    vector<Bullet> shipBullets;
    SDL_Rect shipBulletRect;

    SDL_RenderClear(renderer);
    gameRender(renderer, background,currentYBackground, spaceShip, backButton,spaceShipRect);
    hpUpdate(100, renderer,health,healthBar);
    SDL_RenderPresent(renderer);

    SDL_Event e;
    bool isHomeClicked = false;
    bool isPauseClicked = false;
   
    int hp = HP; long long slowDown = 0;
    int score = 0;
    int frequencyEnemieApear = ENEMIES_FREQUENCY_APPEAR;
    Item* item=nullptr;
    EnemieSize enemieSize;

    while (!*quit && !isHomeClicked) {
        
        if (!isPauseClicked) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) *quit = true;
                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);
                    if (onButtonClicked(pauseRect, mouseX, mouseY)) isPauseClicked = true;
                }
            }
            const Uint8* kState = SDL_GetKeyboardState(NULL);
            if (kState[SDL_SCANCODE_W]) spaceShipRect.y -= SPACE_SHIP_SPEED;
            if (kState[SDL_SCANCODE_S]) spaceShipRect.y += SPACE_SHIP_SPEED;
            if (kState[SDL_SCANCODE_A]) spaceShipRect.x -= SPACE_SHIP_SPEED;
            if (kState[SDL_SCANCODE_D]) spaceShipRect.x += SPACE_SHIP_SPEED;
            

            if (spaceShipRect.x < 0) spaceShipRect.x = 0;
            if (spaceShipRect.y < 0) spaceShipRect.y = 0;
            if (spaceShipRect.x > SCREEN_WIDTH - spaceShipRect.w) spaceShipRect.x = SCREEN_WIDTH - spaceShipRect.w;
            if (spaceShipRect.y > SCREEN_HEIGHT - spaceShipRect.h) spaceShipRect.y = SCREEN_HEIGHT - spaceShipRect.h;

            if (hp < 0) break;

            slowDown++;
            if (slowDown % ITEM_FREQUENCY == 0) {
                item = new Item(renderer,itemTexture,SCREEN_WIDTH);
            }
            if (slowDown % BACKGROUND_SPEED == 0) {
                currentYBackground++;
            }

            if (currentYBackground > SCREEN_HEIGHT) {
                currentYBackground = 0;
            }
            if (slowDown % frequencyEnemieApear == 0) {
                enemieCreateCount = dis35(gen);
                enemieSize = EnemieSize::SMALL;
                if (slowDown % ENEMIES_MEDIUM_FREQUENCY == 0) {
                    enemieSize = EnemieSize::MEDIUM;
                    enemieCreateCount = dis13(gen);
                }
                if (slowDown % ENEMIES_LARGE_FREQUENCY == 0) {
                    enemieSize = EnemieSize::LARGE;
                    enemieCreateCount = dis12(gen);
                }

                for (int i = 1; i <= enemieCreateCount; i++) {
                    switch (dis26(gen)) {
                    case 1:
                        enemies.push_back(
                            SmallEnemie(1, enemieSize, SCREEN_WIDTH, SCREEN_HEIGHT,
                                enemieRandom1, renderer, spaceShipRect, bullets));
                        break;
                    case 2:
                        enemies.push_back(
                            SmallEnemie(2, enemieSize, SCREEN_WIDTH, SCREEN_HEIGHT,
                                enemieRandom2, renderer, spaceShipRect, bullets));
                        break;
                    case 3:
                        enemies.push_back(
                            SmallEnemie(3, enemieSize, SCREEN_WIDTH, SCREEN_HEIGHT,
                                enemieRandom3, renderer, spaceShipRect, bullets));
                        break;
                    case 4:
                        enemies.push_back(
                            SmallEnemie(4, enemieSize, SCREEN_WIDTH, SCREEN_HEIGHT,
                                enemieRandom4, renderer, spaceShipRect, bullets));
                        break;
                    case 5:
                        enemies.push_back(
                            SmallEnemie(5, enemieSize, SCREEN_WIDTH, SCREEN_HEIGHT,
                                enemieRandom5, renderer, spaceShipRect, bullets));
                        break;
                    case 6:
                        enemies.push_back(
                            SmallEnemie(6, enemieSize, SCREEN_WIDTH, SCREEN_HEIGHT,
                                enemieRandom6, renderer, spaceShipRect, bullets));
                        break;

                    }
                }
            }
            if (slowDown % BOOST_FREQUENCY == 0) {
                if (frequencyEnemieApear > MINIMUM_FEQUENCY_ENEMIE_APEAR)
                    frequencyEnemieApear -= 250;
            }
            shipBulletRect = { spaceShipRect.x + spaceShipRect.w / 2 - SPACE_BULLET_SIZE / 2,
                spaceShipRect.y - SPACE_SHIP_SIZE / 2 ,SPACE_BULLET_SIZE,2 * SPACE_BULLET_SIZE };
            for (SmallEnemie& enemie : enemies) if (!enemie.isInit) enemie.init(shots);
            if (slowDown % SHIP_SHOT_FREQUENTCY == 0) {
                shootPlay();
                shipBullets.push_back(Bullet(renderer, shipShot, shipBulletRect, 3, 0));
            }
            SDL_RenderClear(renderer);
            gameRender(renderer, background, currentYBackground, spaceShip, pauseButton,spaceShipRect);
            enemieShot(bullets);
            enemieShot(shipBullets);//spaceShipShot

            for (Bullet& bullet : bullets) if (SmallEnemie::onDamageEvent(bullet.bulletRect, spaceShipRect)) {
                bullet.image = shots[bullet.enemieId + 5];
                if (!bullet.isDestroy) hp -= HP_BULLET_COST;
                bullet.isDestroy = true;
            }
            for (Bullet& bullet : shipBullets) {
                for (SmallEnemie& enemie : enemies) if (SmallEnemie::onDamageEvent(
                    enemie.enemieRect, bullet.bulletRect
                )) {

                    bullet.image = shipShotDestroy;
                    if (!bullet.isDestroy) enemie.enemieHp -= ENEMIE_LOST_HP_BY_BULLET;
                    bullet.isDestroy = true;
                }
            }
            for (SmallEnemie& enemie : enemies) {
                enemie.execute(spaceShipRect, enemies, hp, score);

            }
            if (item != nullptr) {
                item->execute();
                if (SmallEnemie::onDamageEvent(item->itemRect, spaceShipRect)) {
                    hp += 3000;
                    if (hp > HP) hp = HP;
                    item->deleteItem();
                    item = nullptr;
                }
            }

            SDL_Color textColor = { 245,120,66 };
            scoreRender(renderer, score,scoreRect,textColor);
            hpUpdate(hp, renderer, health, healthBar);
            SDL_RenderPresent(renderer);
        }
        else {
            changeScoreRectSummary(summaryScoreRect,summary,score);
            summaryScoreRect.x = summary.x + summary.w / 2 - summaryScoreRect.w / 2;
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) *quit = true;
                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);
                    if (onButtonClicked(pauseRect, mouseX, mouseY)) isPauseClicked = false;
                    if (onButtonClicked(homeButtonRect, mouseX, mouseY)) isHomeClicked = true;
                }
            }

            SDL_Color textColor = { 245, 150, 66 };
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, board, NULL, &summary);
            SDL_RenderCopy(renderer, homeButton, NULL, &homeButtonRect);
            scoreRender(renderer, score,summaryScoreRect,textColor);
            SDL_RenderCopy(renderer, pauseButton, NULL, &pauseRect);
            SDL_RenderPresent(renderer);
        }
        
        SDL_Delay(1);
    }

    changeScoreRectSummary(summaryScoreRect, summary, score);
    summaryScoreRect.x = summary.x + summary.w / 2 - summaryScoreRect.w / 2;
    

    SDL_Color textColor = { 245, 150, 66 };
    SDL_Rect gameOverRect = { summary.x + summary.w / 2,summary.y,60,60 };
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, board, NULL, &summary);
    SDL_RenderCopy(renderer, homeButton, NULL, &homeButtonRect);
    scoreRender(renderer, score, summaryScoreRect, textColor);
    gameOverTextRender(renderer, gameOverRect, textColor);
    SDL_RenderPresent(renderer);

    bool isEndGame = false;
    if (!*quit && !isHomeClicked) {
        while (true) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) *quit = true;
                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);
                    if (onButtonClicked(homeButtonRect, mouseX, mouseY)) isEndGame=true;
                }
            }
            if (isEndGame) break;
        }
    }
}


//  MAIN BACKGROUND INIT

void backgroundInit(SDL_Renderer* renderer) {
    SDL_Texture* background = loadTexture("image/background/background.png", renderer);
    SDL_Texture* backgroundShip = loadTexture("image/background/backgroundShip.png", renderer);

    SDL_Texture* newGame = loadTexture("image/ui/newgame.png", renderer);
    SDL_Texture* exitGame = loadTexture("image/ui/exit.png", renderer);
    SDL_Texture* spaceShip1 = loadTexture("image/Componant/spaceShip/ships/green.png", renderer);
    SDL_Texture* spaceShip2 = loadTexture("image/Componant/spaceShip/ships/blue.png", renderer);
    SDL_Texture* spaceShip3 = loadTexture("image/Componant/spaceShip/ships/gray.png", renderer);
    SDL_Texture* spaceShip4 = loadTexture("image/Componant/spaceShip/ships/purple.png", renderer);
    SDL_Texture* spaceShip5 = loadTexture("image/Componant/spaceShip/ships/brown.png", renderer);

        

    int SPACE_SHIP_BACKGROUND_SIZE = 50;
    SDL_Rect spaceShipRectBackground1 = { 100,150,SPACE_SHIP_BACKGROUND_SIZE,SPACE_SHIP_BACKGROUND_SIZE };
    SDL_Rect spaceShipRectBackground2 = { 200,300,SPACE_SHIP_BACKGROUND_SIZE,SPACE_SHIP_BACKGROUND_SIZE };
    SDL_Rect spaceShipRectBackground3 = { 700,200,SPACE_SHIP_BACKGROUND_SIZE,SPACE_SHIP_BACKGROUND_SIZE };
    SDL_Rect spaceShipRectBackground4 = { 1200,700,SPACE_SHIP_BACKGROUND_SIZE,SPACE_SHIP_BACKGROUND_SIZE };
    SDL_Rect spaceShipRectBackground5 = { 1400,350,SPACE_SHIP_BACKGROUND_SIZE,SPACE_SHIP_BACKGROUND_SIZE };

    SDL_Rect backgroundShipRect = { PADDING,PADDING,SCREEN_HEIGHT,SCREEN_HEIGHT };

    SDL_Point spacePoint = { SPACE_SHIP_BACKGROUND_SIZE / 2 , SPACE_SHIP_BACKGROUND_SIZE / 2 };
    SDL_RendererFlip flip = SDL_FLIP_NONE;

    

    SDL_RenderCopy(renderer, background, NULL, &mapRect);
    SDL_RenderCopy(renderer, backgroundShip, NULL, &backgroundShipRect);

    renderMusicBackground(renderer, musicRect);
    SDL_RenderCopyEx(renderer, spaceShip1, NULL, &spaceShipRectBackground1, 6.0, &spacePoint, flip);
    SDL_RenderCopyEx(renderer, spaceShip2, NULL, &spaceShipRectBackground2, 34, &spacePoint, flip);
    SDL_RenderCopyEx(renderer, spaceShip3, NULL, &spaceShipRectBackground3, 57, &spacePoint, flip);
    SDL_RenderCopyEx(renderer, spaceShip4, NULL, &spaceShipRectBackground4, 160, &spacePoint, flip);
    SDL_RenderCopyEx(renderer, spaceShip5, NULL, &spaceShipRectBackground5, 0, &spacePoint, flip);

        
    SDL_RenderCopy(renderer, newGame, NULL, &newGameRect);
    SDL_RenderCopy(renderer, exitGame, NULL, &exitRect);
}


//  MAIN MENU

void mainMenu(SDL_Renderer* renderer,bool* quit) {
    SDL_RenderClear(renderer);
    backgroundInit(renderer);

    SDL_Texture* musicOn = loadTexture("image/ui/music-on.png", renderer);
    SDL_Texture* musicOff = loadTexture("image/ui/music-off.png", renderer);
    if(openMusic) SDL_RenderCopy(renderer, musicOn, NULL, &musicRect);
    if(!openMusic) SDL_RenderCopy(renderer, musicOff, NULL, &musicRect);
    SDL_RenderPresent(renderer);

    
    SDL_Event e; bool isNewgameClicked = false;
    while (!*quit && !isNewgameClicked) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) *quit = true;
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                if (onButtonClicked(musicRect, mouseX, mouseY)) {
                    openMusic = !openMusic;
                    musicButtonAdapter(renderer, musicOn, musicOff, musicRect, openMusic);
                }
                if (onButtonClicked(exitRect, mouseX, mouseY)) *quit = true;
                if (onButtonClicked(newGameRect, mouseX, mouseY)) {
                    isNewgameClicked = true;
                }
            }
        }
    }
}

//  MAIN METHOD

int main(int argc, char* argv[]){
//int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Init(window, renderer);
    bool isMainMenu = true;
    bool quit = false;
    while (!quit) {
        mainMenu(renderer,&quit);
        if (quit) break;
        gamePlay(renderer,&quit);
    }
    cout << SDL_GetError();
    QuitSDL(window, renderer);
    return 0;
}