#include<SDL_mixer.h>

int processEvents(SDL_Window* window, Opject* alCar1, Opject* alCar2, Opject* alBall, Opject* alNet1, Opject* alNet2, itemOpject* sleep, itemOpject* big);
void doRender(SDL_Renderer* renderTarget, Opject* alCar, Opject* alCar2, Opject* alBall, SDL_Texture* ruler, SDL_Texture* car1, SDL_Texture* car2, SDL_Texture* ball, SDL_Texture* background, SDL_Texture* goal, SDL_Rect goalRect, SDL_Texture* goalCount1, SDL_Texture* goalCount2, SDL_Rect SgoalCount1, SDL_Rect SgoalCount2, SDL_Rect DgoalCount1, SDL_Rect DgoalCount2, SDL_Texture* goalNet1, SDL_Texture* goalNet2, Opject* alNet1, Opject* alNet2, itemOpject* sleep, itemOpject* Big, itemOpject* magicball);
void initOpject(Opject * opjects, int posX, int posY, int angOject);
void goalCheer(SDL_Rect *goalRec, int H, int W, float dental, Opject *alCar1, Opject *alCar2, Opject *alBall, float *delay, int *goalCount1, int *goalCount2, Mix_Chunk *soundEffectGoal);
void goalCounting(int goalCount1, int goalCount2, SDL_Rect *SgoalCount1, SDL_Rect *SgoalCount2, float FrameW);
void random_pos(int* pos, int left, int right);
void init_item(itemOpject* item, const char* path, SDL_Renderer* renderTarget);
void item_event(itemOpject* item, Opject* alCar1, Opject* alCar2, int startTime, int againTime, int effectTime, int realTime, int* item_posX, int* item_posY, int *a);
void item_magicball(Opject* ball, itemOpject* magicball, Opject* alCar1, Opject* alCar2, int startTime, int againTime, int effectTime, int realTime, int* item_posX, int* item_posY, int *a);
int menu(SDL_Renderer* renderTarget, SDL_Window* window, SDL_Texture* background_menu, SDL_Texture* start, SDL_Texture* exit1, SDL_Texture* start1, SDL_Texture* exit2);
int CheckForcusWithRect(int x, int y, SDL_Rect rect);