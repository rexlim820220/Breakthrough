#include <iostream>
#include <vector>
#include <utility>
#include <SDL2/SDL.h>
#include "bitboard.h"
#include "sdl.h"

using namespace std;

int main(int argc,char **argv)
{
    Bitboard bitboard;
    if(!init())
    {
        cout<<"Failed to initialize!"<<endl;
    }
    else
    {
        if(!loadmedia())
        {
            cout<<"Failed to load media!"<<endl;
        }
        else
        {
            SDL_Rect white_pawn[16];
            SDL_Rect black_pawn[16];
            int chessnum=16,click=0,no=-1,NO=8,tmp=0;
            vector<pair<int,int>>black(chessnum);
            vector<pair<int,int>>white(chessnum);
            for(int i=0; i<8; i++)
            {
                white[i].first=i;
                white[i].second=0;
            }
            for(int i=8; i<16; i++)
            {
                white[i].first=(i-8);
                white[i].second=1;
            }
            for(int j=0; j<8; j++)
            {
                black[j].first=j;
                black[j].second=6;
            }
            for(int j=8; j<16; j++)
            {
                black[j].first=(j-8);
                black[j].second=7;
            }
            set_white_position(white_pawn);
            set_black_position(black_pawn);
            loadbackground();
            loadwhitepawn(white_pawn);
            loadblackpawn(black_pawn,click,no);
            while(!bitboard.endgame())
            {
                bool quit=false,state=FiniteState::Initial;
                SDL_Event e;
                while(!quit)
                {
                    bool turn = true;
                    int x=0,y=0;
                    if(turn)
                    {
                        switch(e.type)
                        {
                        case SDL_MOUSEMOTION:
                            getlocation(&x,&y);
                            break;
                        case SDL_MOUSEBUTTONDOWN:
                            ++click;
                            getlocation(&x,&y);
                            no = bitboard.Click(black_pawn,black,x,y,bitboard).no;
                            if(no>=0&&no<16)
                            {
                                turn=true;
                                tmp=no;
                                state=FiniteState::Select;
                            }
                            cout<<no<<endl;
                            if((no<0)&&state==FiniteState::Select)
                            {
                                bitboard.MoveBlack(black_pawn,black,bitboard,tmp,x,y);
                                state = FiniteState::Initial;
                                turn = false;
                            }
                            break;
                        case SDL_MOUSEBUTTONUP:
                            getlocation(&x,&y);
                            break;
                        }
                    }
                    if(turn == false)
                    {
                        if(NO<16)
                        {
                            bitboard.MoveWhite(white_pawn,white,bitboard,NO);
                            NO++;
                            if(NO==16)
                            {
                                NO=0;
                            }
                        }
                    }
                    loadbackground();
                    loadwhitepawn(white_pawn);
                    loadblackpawn(black_pawn,click,no);
                    // = renderercopy(,,,,);
                    while(SDL_PollEvent(&e)!=0)
                    {
                        if(e.type==SDL_QUIT)
                        {
                            quit=true;
                        }
                    }
                    slot();
                }
            }
        }
    }
    close();
    return 0;
}
