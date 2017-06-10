#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstdlib>
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
		gWindow = SDL_CreateWindow( "Breakthrough", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			gScreenSurface = SDL_GetWindowSurface( gWindow );
		}
	}
	return success;
}

bool loadMedia()
{
	bool success = true;

	gXOut = SDL_LoadBMP( "03_event_driven_programming/x.bmp" );
	bpawn = SDL_LoadBMP( "04_key_presses/down.bmp" );
	wpawn = SDL_LoadBMP( "04_key_presses/left.bmp" );
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

	SDL_Quit();
}


int main(int argc,char **argv)
{
    Bitboard bitboard;
    int pos_x=0,pos_y=0,i=0,color=1;
    cout<<"First,or second?"<<endl;
    cin>>color;
    if(color!=0||color!=1)
    {
        color=1;
    }
    while(!bitboard.endgame())
    {
        /*cin>>pos_x>>pos_y;
        if((pos_x<48||pos_x>55)||(pos_y<40||pos_y>47))
        {
            pos_x=48+i;
            pos_y=40+i;
            i+=1;
            if(i>15)
            {
                i*=(-1);
            }
        }*/
        if(!init())
        {
            /*Start SDL*/
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
                    while( SDL_PollEvent( &e ) != 0 )
                    {
                        if( e.type == SDL_QUIT )
                        {
                            quit = true;
                        }
                    }
                    SDL_BlitSurface( gXOut, NULL, gScreenSurface, NULL );
                    SDL_UpdateWindowSurface( gWindow );
                    SDL_Delay( 2000 );
                }
            }
        }
        system("cls");
        bitboard.make_move(pos_x,pos_y,color,bitboard);
        if(color==0)
        {
            color=1;
        }
        else
        {
            color=0;
        }
    }
    close();
    return 0;
}


