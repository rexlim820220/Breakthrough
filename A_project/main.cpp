#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <string>
#include <SDL2/SDL.h>
using namespace std;

enum Color {black,white};

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;

class Bitboard
{
public:
    Bitboard()
    {
        whitepawn=0xffff000000000000;
        blackpawn=0x000000000000ffff;
    }
    Bitboard make_move(int,int,int,Bitboard);
    bool endgame();
    void showboard();
    float Evaulate(int color);
    ~Bitboard() {}
private:
    uint64_t whitepawn;
    uint64_t blackpawn;
};

Bitboard Bitboard::make_move(int x,int y,int color,Bitboard board)
{
    if(color==Color::black)
    {
        unsigned long long a=1<<12;
        a=~a;
        blackpawn=blackpawn&a;
        a=1<<20;
        blackpawn=blackpawn|a;
        showboard();
    }
    else
    {
        unsigned long long b=0x0004000000000000;
        b=1ll<<x;//48
        b=~b;
        whitepawn=whitepawn&b;
        b=1ll<<y;//40
        whitepawn=whitepawn|b;
        showboard();
    }
    return board;
}

void Bitboard::showboard()
{
    for(int i=0; i<64; i++)
    {
        unsigned long long a=1;
        a=a<<i;
        if((blackpawn&a)!=0)
        {
            printf("*");
        }
        else if((whitepawn&a)!=0)
        {
            printf("@");
        }
        else
        {
            printf("0");
        }
        if(i%8==7)printf("\n");
    }
}

float Bitboard::Evaulate(int color)
{
    int score=0;
    if(color==Color::black)
    {
        for(int i=7; i>=0; i--)
        {
            for(int j=i*8; j<i*8+8; j++)
            {
                if((blackpawn&(1<<j))==0)
                {
                    /*no pawn*/;
                }
                else
                {
                    score+=pow(8-i,2);/*exist pawn*/
                }
            }
        }
        return score;
    }
    else
    {
        for(int i=0; i<8; i++)
        {
            for(int j=i*8; j<i*8+8; j++)
            {
                if((whitepawn&(1<<j))==0)
                {
                    /*no pawn*/;
                }
                else
                {
                    score+=pow(i,2);/*exist pawn*/
                }
            }
        }
        return score;
    }
}

bool Bitboard::endgame()
{
    for(int i=56; i<64; i++)
    {
        if((blackpawn&(1ll<<i))!=0)
        {
            return true;
        }
    }
    for(int i=0; i<8; i++)
    {
        if((whitepawn&(1ll<<i))!=0)
        {
            return true;
        }
    }
    return(blackpawn==0x0000000000000000||
           whitepawn==0x0000000000000000);
}

int NegaMax(int depth,Bitboard board,int color)
{
    int value=0;
    int best=-1e6;
    if(depth<=0||board.endgame())
    {
        return value;
    }
    //GenerateLegalMoves();
    int x=0,y=0;
    while(!board.endgame())
    {
        board.make_move(x,y,color,board);
        value=-NegaMax(depth-1,board,color);
        //unmakemove;
        if(value>best)
        {
            best=value;
        }
    }
    return best;
}

bool init(),loadMedia();

void close();

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gXOut = NULL;
SDL_Surface* bpawn = NULL,* wpawn = NULL;
SDL_Renderer *gRenderer = NULL;

bool init()
{
    bool success = true;

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }
        gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
        gWindow = SDL_CreateWindow( "Breakthrough", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
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

class LTexture
{
public:
    LTexture()
    {
        mTexture=NULL;
        mWidth=0;
        mHeight=0;
    }
    ~LTexture()
    {
        free();
    }
    bool loadFromFile(string path)
    {
        free();
        SDL_Texture *newTexture = NULL;
        SDL_Surface *loadedSurface = SDL_LoadBMP( "03_event_driven_programming/x.bmp" );
        if(loadedSurface == NULL)
        {
            printf( "Unable to load image %s! SDL_image Error:\n", path.c_str());
        }
        else
        {
            SDL_SetColorKey(loadedSurface,SDL_TRUE,SDL_MapRGB(loadedSurface->format,0,0xFF,0xFF));
            newTexture = SDL_CreateTextureFromSurface(gRenderer,loadedSurface);
            if(newTexture == NULL)
            {
                printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
            }
            else
            {
                mWidth = loadedSurface->w;
                mHeight = loadedSurface->h;
            }
            SDL_FreeSurface(loadedSurface);
        }
        mTexture = newTexture;
        return mTexture != NULL;
    }
    void free()
    {
        if( mTexture != NULL )
        {
            SDL_DestroyTexture( mTexture );
            mTexture = NULL;
            mWidth = 0;
            mHeight = 0;
        }
    }
    void render(int x,int y)
    {
        SDL_Rect renderquad = {x, y, mWidth, mHeight};
        SDL_RenderCopy( gRenderer, mTexture, NULL, &renderquad );
    }
    int getWidth()
    {
        return mWidth;
    }
    int getHeight()
    {
        return mHeight;
    }
private:
    SDL_Texture * mTexture;
    int mWidth;
    int mHeight;
};

LTexture gFooTexture;
LTexture gBackgroundTexture;

bool loadMedia()
{
    bool success = true;

    gXOut = SDL_LoadBMP( "03_event_driven_programming/x.bmp" );
    bpawn = SDL_LoadBMP( "04_key_presses/white.bmp" );
    wpawn = SDL_LoadBMP( "04_key_presses/black.bmp" );
    if( gXOut == NULL )
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

void close()
{
    SDL_FreeSurface( gXOut );
    gXOut = NULL;

    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    gFooTexture.free();
    gBackgroundTexture.free();

    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    SDL_Quit();
}


int main(int argc,char **argv)
{
    Bitboard bitboard;
    int color=1;
    cout<<"First,or second?"<<endl;
    cin>>color;
    if(color!=0||color!=1)
    {
        color=1;
    }
    if(!init())
    {
        cout<<"Failed to initialize!"<<endl;
    }
    else
    {
        if( !loadMedia() )
        {
            cout<<"Failed to load media!"<<endl;
        }
        else
        {
            bool quit = false;
            SDL_Event e;
            while( !quit )
            {
                switch(e.type)
                {
                    int x,y;
                    case SDL_MOUSEMOTION:
                        cout<<"@"<<endl;
                        SDL_GetMouseState(&x,&y);
                        x-=47;y-=47;
                        if((x>0&&x<495)&&(y>0&&y<495))
                        {
                            x/=62;y/=62;
                        }
                        cout<<x<<' '<<y<<endl;

                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        cout<<"#"<<endl;
                        SDL_GetMouseState(&x,&y);
                        x-=47;y-=47;
                        if((x>0&&x<495)&&(y>0&&y<495))
                        {
                            x/=62;y/=62;
                        }
                        cout<<x<<' '<<y<<endl;
                        break;
                    case SDL_MOUSEBUTTONUP:
                        cout<<"*"<<endl;
                        SDL_GetMouseState(&x,&y);
                        x-=47;y-=47;
                        if((x>0&&x<495)&&(y>0&&y<495))
                        {
                            x/=62;y/=62;
                        }
                        cout<<x<<' '<<y<<endl;
                        break;
                }
                while( SDL_PollEvent( &e ) != 0 )
                {
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                }
                SDL_Rect source_rect[16];
                for(int i=0; i<16; i++)
                {
                    if(i<8)
                    {
                        source_rect[i] = {-48-(i*63),-44,SCREEN_WIDTH,SCREEN_HEIGHT};
                    }
                    else
                    {
                        source_rect[i] = {-48-((i-8)*63),-107,SCREEN_WIDTH,SCREEN_HEIGHT};
                    }
                }

                SDL_Rect source_rect_forb[16];
                for(int i=0; i<16; i++)
                {
                    if(i<8)
                    {
                        source_rect_forb[i] = {-48-(63*i),-484,SCREEN_WIDTH,SCREEN_HEIGHT};
                    }
                    else
                    {
                        source_rect_forb[i] = {-48-(63*(i-8)),-420,SCREEN_WIDTH,SCREEN_HEIGHT};
                    }
                }
                /**/
                SDL_Rect dest_rect = {1,0,SCREEN_HEIGHT,SCREEN_WIDTH};
                SDL_BlitSurface( gXOut, NULL, gScreenSurface, &dest_rect );
                /**/
                SDL_Surface* Wpawn[16],*Bpawn[16];
                for(int i=0; i<16; i++)
                {
                    Wpawn[i] = SDL_LoadBMP( "sprite/white.bmp" );

                    SDL_SetColorKey( Wpawn[i],SDL_TRUE, 0xFF00 );
                    SDL_BlitSurface( Wpawn[i], &source_rect[i], gScreenSurface, NULL );
                }
                for(int i=0; i<16; i++)
                {
                    Bpawn[i] = SDL_LoadBMP( "sprite/black.bmp" );
                    SDL_SetColorKey( Bpawn[i],SDL_TRUE, 0xFF00 );
                    SDL_BlitSurface( Bpawn[i], &source_rect_forb[i], gScreenSurface, NULL );
                }
                SDL_UpdateWindowSurface( gWindow );
                SDL_Delay( 30 );
            }
        }
    }
    close();
    return 0;
}


