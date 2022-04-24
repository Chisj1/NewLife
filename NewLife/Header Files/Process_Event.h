#include<SDL_mixer.h>

int processEvents(SDL_Window *window, Opject *alCar1, Opject *alCar2, Opject *alBall, Opject *alNet1, Opject *alNet2);
void doRender(SDL_Renderer *renderTarget, Opject *alCar, Opject *alCar2, Opject *alBall, SDL_Texture *car1, SDL_Texture *car2, SDL_Texture *ball, SDL_Texture *background, SDL_Texture *goal, SDL_Rect goalRect, SDL_Texture *goalCount1, SDL_Texture *goalCount2, SDL_Rect SgoalCount1, SDL_Rect SgoalCount2, SDL_Rect DgoalCount1, SDL_Rect DgoalCount2, SDL_Texture *goalNet1, SDL_Texture *goalNet2, Opject *alNet1, Opject *alNet2);
void initOpject(Opject * opjects, int posX, int posY, int angOject);
void goalCheer(SDL_Rect *goalRec, int H, int W, float dental, Opject *alCar1, Opject *alCar2, Opject *alBall, float *delay, int *goalCount1, int *goalCount2, Mix_Chunk *soundEffectGoal);
void goalCounting(int goalCount1, int goalCount2, SDL_Rect *SgoalCount1, SDL_Rect *SgoalCount2, float FrameW);