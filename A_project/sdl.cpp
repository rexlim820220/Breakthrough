#include <SDL2/SDL.h>
#include <cstdio>
#include <iostream>
#include "bitboard.h"
#include "sdl.h"

SDL_Window* gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
SDL_Surface* gScreenSurface = NULL;

bool init()
{
    bool success = true;
    if(SDL_Init(SDL_INIT_VIDEO)<0)
    {
        printf("SDL could not initialized!SDL_Error:%s\n",SDL_GetError());
        success=false;
    }
    else
    {
        if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled!");
        }
        gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
        gWindow = SDL_CreateWindow( "Breakthrough", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if(gWindow == NULL)
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            gScreenSurface = SDL_GetWindowSurface( gWindow );
            SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        }
    }
    return success;
}

SDL_Surface* backgorund = NULL;
SDL_Surface* bpawn = NULL,* wpawn = NULL;

bool loadmedia()
{
    bool success = true;
    backgorund =  SDL_LoadBMP("canvas/board.bmp");
    bpawn = SDL_LoadBMP("canvas/black.bmp");
    wpawn = SDL_LoadBMP("canvas/white.bmp");
    if(backgorund==NULL)
    {
        printf( "Unable to load image %s! SDL Error: %s\n", "03_event_driven_programming/x.bmp", SDL_GetError() );
        success = false;
    }
    if(( bpawn == NULL )&&( wpawn == NULL ))
    {
        printf( "Unable to load image %s! SDL Error: %s\n", "03_event_driven_programming/x.bmp", SDL_GetError() );
        success = false;
    }
    return success;
}

void set_white_position(SDL_Rect white_pawn[])
{
    for(int i=0; i<16; i++)
    {
        if(i<8)
        {
            white_pawn[i] = {-48-(i*63),-44,SCREEN_WIDTH,SCREEN_HEIGHT};
        }
        else
        {
            white_pawn[i] = {-48-((i-8)*63),-107,SCREEN_WIDTH,SCREEN_HEIGHT};
        }
    }
}

void set_black_position(SDL_Rect black_pawn[])
{
    for(int i=0; i<16; i++)
    {
        if(i<8)
        {
            black_pawn[i] = {-48-(63*i),-484,SCREEN_WIDTH,SCREEN_HEIGHT};
        }
        else
        {
            black_pawn[i] = {-48-(63*(i-8)),-420,SCREEN_WIDTH,SCREEN_HEIGHT};
        }
    }
}

void loadbackground()
{
    SDL_Rect load_background = {1,0,SCREEN_HEIGHT,SCREEN_WIDTH};
    SDL_BlitSurface(backgorund,NULL,gScreenSurface,&load_background);
}

void loadwhitepawn(SDL_Rect white_pawn[])
{
    SDL_Surface* Wpawn[16];
    for(int i=0; i<16; i++)
    {
        Wpawn[i] = SDL_LoadBMP( "canvas/white.bmp" );

        SDL_SetColorKey( Wpawn[i],SDL_TRUE, 0xFF00 );
        SDL_BlitSurface( Wpawn[i], &white_pawn[i], gScreenSurface, NULL );
    }
}

void loadblackpawn(SDL_Rect black_pawn[],int click,int no)
{
    SDL_Surface* Bpawn[16];
    for(int i=0; i<16; i++)
    {
        Bpawn[i] = SDL_LoadBMP( "canvas/black.bmp" );
        if(((click%2)==FiniteState::Select)||(no>0)){Bpawn[(no+8)%16] = SDL_LoadBMP( "canvas/selected.bmp" );}
        SDL_SetColorKey( Bpawn[i],SDL_TRUE, 0xFF00 );
        SDL_BlitSurface( Bpawn[i], &black_pawn[i], gScreenSurface, NULL );
    }
}

void getlocation(int *x,int *y)
{
    SDL_GetMouseState(x,y);
    *x-=47;
    *y-=47;
    if((*x>0&&*x<495)&&(*y>0&&*y<495))
    {
        *x/=62;
        *y/=62;
    }
    //std::cout<<*x<<' '<<*y<<std::endl;
}

int convert_x(int x)
{
    int x_=-48;
    for(int i=0;i<8;i++)
    {
        if(x==i)
        {
            x_-=(i*63);
        }
    }
    return x_;
}

int convert_y(int y)
{
    int y_=-44;
    for(int j=0;j<8;j++)
    {
        if(y==j)
        {
            y_-=(63*j);
        }
    }
    return y_;
}

void close()
{
    SDL_FreeSurface(backgorund);
    backgorund = NULL;
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    SDL_Quit();
}

void slot()
{
    SDL_UpdateWindowSurface(gWindow);
    SDL_Delay(30);
}
