
#include "SDL.h"
#include "SDL_image.h"
#include "MathUtil.h"
#include <math.h>
#include <stdio.h>

typedef struct
{
  float x, y;
  float dx, dy;
  float gdx, gdy;
  float ang;
} Man;

static SDL_Texture *background = NULL;
static SDL_Texture *car1 = NULL;
static SDL_Texture *car2 = NULL;

static void applyForces(Man *man)
{
  const float w = 14*2, h = 22*2;

  if(man->x < 25)
  {
    man->x = 25+5;
    if(man->gdx < 0)
    {
      man->gdx *= -1;
    }
    if(man->dx < 0)
    {
      man->dx *= -1;
    }    
  }
  if(man->x+w > 640-25)
  {
    man->x = 640-(25+5)-w;
    if(man->gdx > 0)
    {
      man->gdx *= -1;
    }
    if(man->dx > 0)
    {
      man->dx *= -1;
    }    
  }
  if(man->y < 25)
  {
    man->y = 25+5;
    if(man->gdy < 0)
    {
      man->gdy *= -1;
    }
    if(man->dy < 0)
    {
      man->dy *= -1;
    }    
  }  
  if(man->y+h > 480-25)
  {
    man->y = 480-(25+5)-h;
    if(man->gdy > 0)
    {
      man->gdy *= -1;
    }
    if(man->dy > 0)
    {
      man->dy *= -1;
    }    
  }
        
  man->x += man->dx;
  man->y += man->dy;
  man->x += man->gdx;
  man->y += man->gdy;  
  man->dx *= 0.96;
  man->dy *= 0.96;
  man->gdx *= 0.92;
  man->gdy *= 0.92;
}

//useful utility function to see if two rectangles are colliding at all
int collide2d(float x1, float y1, float x2, float y2, float wt1, float ht1, float wt2, float ht2)
{
  return (!((x1 > (x2+wt2)) || (x2 > (x1+wt1)) || (y1 > (y2+ht2)) || (y2 > (y1+ht1))));
}

static void carCollision(Man *man, Man *man2)
{
  const float w = 14*2, h = 22*2;
  
  if(collide2d(man->x, man->y, man2->x, man2->y, w, h, w, h))
  {
    float vx = man2->x-man->x;
    float vy = man2->y-man->y;    
    float man1Speed = length(man->dx, man->dy);
    float man2Speed = length(man2->dx, man2->dy);
    
    if(man1Speed < 1.0f)
    {
      man1Speed = 1.0f;
    }
    if(man2Speed < 1.0f)
    {
      man2Speed = 1.0f;
    }
    
    normalize(&vx, &vy);    
    
    man->gdx += -vx*man2Speed;
    man->gdy += -vy*man2Speed;    
    man2->gdx += vx*man1Speed;
    man2->gdy += vy*man1Speed;    
  }
}

int processEvents(SDL_Window *window, Man *man, Man *man2)
{
  SDL_Event event;
  int done = 0;
  const float acc = 2.0f;

  while(SDL_PollEvent(&event))
  {
    switch(event.type)
    {
      case SDL_WINDOWEVENT_CLOSE:
      {
        if(window)
        {
          SDL_DestroyWindow(window);
          window = NULL;
          done = 1;
        }
      }
      break;
      case SDL_KEYDOWN:
      {
        switch(event.key.keysym.sym)
        {
          case SDLK_ESCAPE:
            done = 1;
          break;
        }
      }
      break;
      case SDL_QUIT:
        //quit out of the game
        done = 1;
      break;
    }
  }
  
  const Uint8 *state = SDL_GetKeyboardState(NULL);
  if(state[SDL_SCANCODE_LEFT])
  {
    man->ang -= 2;
  }
  if(state[SDL_SCANCODE_RIGHT])
  {
    man->ang += 2;
  }
  if(state[SDL_SCANCODE_UP])
  {
    float cdx = sinf(radiansFromDegrees(man->ang))*acc*0.1;
    float cdy = -cosf(radiansFromDegrees(man->ang))*acc*0.1;
    
    man->dx += cdx;
    man->dy += cdy;    
  }
  if(state[SDL_SCANCODE_DOWN])
  {
    float cdx = sinf(radiansFromDegrees(man->ang))*0.1;
    float cdy = -cosf(radiansFromDegrees(man->ang))*0.1;
    
    man->dx += -cdx;
    man->dy += -cdy;    
  }

  float vx = man2->x-man->x;
  float vy = man2->y-man->y;    
  float theta = degreesFromRadians(atan2(vy, vx))-90;
  man2->ang = theta;  

  float cdx = sinf(radiansFromDegrees(man2->ang))*acc*0.1;
  float cdy = -cosf(radiansFromDegrees(man2->ang))*acc*0.1;
    
  man2->dx += cdx;
  man2->dy += cdy;    

  carCollision(man, man2);  
  applyForces(man);
  applyForces(man2);
  
  return done;
}

void doRender(SDL_Renderer *renderer, Man *man, Man *man2)
{
  //set the drawing color to blue
  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  
  //Clear the screen (to blue)
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, background, NULL, NULL);

  //set the drawing color to white
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  
  SDL_Rect rect = { man->x, man->y, 14*2, 22*2 };
  SDL_RenderCopyEx(renderer, car1, NULL, &rect, man->ang, NULL, 0);  

  SDL_Rect rect2 = { man2->x, man2->y, 14*2, 22*2 };
  SDL_RenderCopyEx(renderer, car2, NULL, &rect2, man2->ang, NULL, 0);  
    
  //We are done drawing, "present" or show to the screen what we've drawn
  SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[])
{
  SDL_Window *window;                    // Declare a window
  SDL_Renderer *renderer;                // Declare a renderer
  
  SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2
  
  Man man, man2;
  man.x = 220;
  man.y = 140;
  man.dx = 0;
  man.dy = 0;
  man.gdx = 0;
  man.gdy = 0;  
  man.ang = 0;

  man2.x = 520;
  man2.y = 360;
  man2.dx = 0;
  man2.dy = 0;
  man2.gdx = 0;
  man2.gdy = 0;  
  man2.ang = 0;
      
  //Create an application window with the following settings:
  window = SDL_CreateWindow("Game Window",                     // window title
                            SDL_WINDOWPOS_UNDEFINED,           // initial x position
                            SDL_WINDOWPOS_UNDEFINED,           // initial y position
                            640,                               // width, in pixels
                            480,                               // height, in pixels
                            0                                  // flags
                            );
                            
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  SDL_Surface *surf = IMG_Load("bg.png");
  if(!surf)
  {
    printf("All is lost\n");
    return 1;
  }                            

  //load background
  background = SDL_CreateTextureFromSurface(renderer, surf);
  SDL_FreeSurface(surf);
  
  //load car
  surf = IMG_Load("car1.png");
  if(!surf)
  {
    printf("All is lost\n");
    return 1;
  }                            
  car1 = SDL_CreateTextureFromSurface(renderer, surf);  
  SDL_FreeSurface(surf);

  surf = IMG_Load("car2.png");
  if(!surf)
  {
    printf("All is lost\n");
    return 1;
  }                            
  car2 = SDL_CreateTextureFromSurface(renderer, surf);  
  SDL_FreeSurface(surf);

  // The window is open: enter program loop (see SDL_PollEvent)
  int done = 0;
  
  //Event loop
  while(!done)
  {
    //Check for events
    done = processEvents(window, &man, &man2);
    
    //Render display
    doRender(renderer, &man, &man2);
    
    //don't burn up the CPU
    SDL_Delay(10);
  }
  
  
  // Close and destroy the window
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  
  SDL_DestroyTexture(background);
  SDL_DestroyTexture(car1);
  SDL_DestroyTexture(car2);
  
  // Clean up
  SDL_Quit();
  return 0;
}

