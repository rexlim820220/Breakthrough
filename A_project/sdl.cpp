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

void reloaedwhitepawn(SDL_Renderer *renderer)
{
    //SDL_Texture *variable = IMG_LOADtexture(renderer,"canvas/white.bmp");
    //SDL_Rect dst;
    //SDL_RenderCopy(renderer,variable,NULL,&dst);
}

void reloadblackpawn(SDL_Rect *,int,int)
{

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

void change(int No,int *a,int *b)
{
    switch(No)
    {
        case 0:
            *a=0;*b=0;
            break;
        case 1:
            *a=1;*b=0;
            break;
        case 2:
            *a=2;*b=0;
            break;
        case 3:
            *a=3;*b=0;
            break;
        case 4:
            *a=4;*b=0;
            break;
        case 5:
            *a=5;*b=0;
            break;
        case 6:
            *a=6;*b=0;
            break;
        case 7:
            *a=7;*b=0;
            break;
        case 8:
            *a=0;*b=1;
            break;
        case 9:
            *a=1;*b=1;
            break;
        case 10:
            *a=2;*b=1;
            break;
        case 11:
            *a=3;*b=1;
            break;
        case 12:
            *a=4;*b=1;
            break;
        case 13:
            *a=5;*b=1;
            break;
        case 14:
            *a=6;*b=1;
            break;
        case 15:
            *a=7;*b=1;
            break;
        case 16:
            *a=0;*b=2;
            break;
        case 17:
            *a=1;*b=2;
            break;
        case 18:
            *a=2;*b=2;
            break;
        case 19:
            *a=3;*b=2;
            break;
        case 20:
            *a=4;*b=2;
            break;
        case 21:
            *a=5;*b=2;
            break;
        case 22:
            *a=6;*b=2;
            break;
        case 23:
            *a=7;*b=2;
            break;
        case 24:
            *a=0;*b=3;
            break;
        case 25:
            *a=1;*b=3;
            break;
        case 26:
            *a=2;*b=3;
            break;
        case 27:
            *a=3;*b=3;
            break;
        case 28:
            *a=4;*b=3;
            break;
        case 29:
            *a=5;*b=3;
            break;
        case 30:
            *a=6;*b=3;
            break;
        case 31:
            *a=7;*b=3;
            break;
        case 32:
            *a=0;*b=4;
            break;
        case 33:
            *a=1;*b=4;
            break;
        case 34:
            *a=2;*b=4;
            break;
        case 35:
            *a=3;*b=4;
            break;
        case 36:
            *a=4;*b=4;
            break;
        case 37:
            *a=5;*b=4;
            break;
        case 38:
            *a=6;*b=4;
            break;
        case 39:
            *a=7;*b=4;
            break;
        case 40:
            *a=0;*b=5;
            break;
        case 41:
            *a=1;*b=5;
            break;
        case 42:
            *a=2;*b=5;
            break;
        case 43:
            *a=3;*b=5;
            break;
        case 44:
            *a=4;*b=5;
            break;
        case 45:
            *a=5;*b=5;
            break;
        case 46:
            *a=6;*b=5;
            break;
        case 47:
            *a=7;*b=5;
            break;
        case 48:
            *a=0;*b=6;
            break;
        case 49:
            *a=1;*b=6;
            break;
        case 50:
            *a=2;*b=6;
            break;
        case 51:
            *a=3;*b=6;
            break;
        case 52:
            *a=4;*b=6;
            break;
        case 53:
            *a=5;*b=6;
            break;
        case 54:
            *a=6;*b=6;
            break;
        case 55:
            *a=7;*b=6;
            break;
        case 56:
            *a=0;*b=7;
            break;
        case 57:
            *a=1;*b=7;
            break;
        case 58:
            *a=2;*b=7;
            break;
        case 59:
            *a=3;*b=7;
            break;
        case 60:
            *a=4;*b=7;
            break;
        case 61:
            *a=5;*b=7;
            break;
        case 62:
            *a=6;*b=7;
            break;
        case 63:
            *a=7;*b=7;
            break;
    }
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
