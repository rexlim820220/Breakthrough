#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <utility>
#include <map>
#include <SDL2/SDL.h>
#include "bitboard.h"
#include "sdl.h"

Bitboard::Bitboard()
{
    whitepawn=0xffff000000000000;
    blackpawn=0x000000000000ffff;
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
    return ((blackpawn==0x0000000000000000)||
            (whitepawn==0x0000000000000000));
}

int no=-1;

float Bitboard::Evaluation(Bitboard bitboard,int depth)
{
    float value=0;
    if(bitboard.endgame()||depth<0)
    {
        return value;
    }
    for(int i=0; i<8; i++)
    {
        for(int j=i*8; j<i*8+8; j++)
        {
            if((whitepawn&(1<<j))==0)
            {
                /*no white pawn*/;
            }
            else
            {
                value+=pow(i,2);
                no=i;
                //break;
            }
        }
    }
    return value;
}

int Bitboard::whiteAB(Bitboard bitboard,int alpha,int beta,int depth)
{
    if(bitboard.endgame()){return 0;}
    if(depth<0){return 0;}
    int value;
    for(int i=0;i<16;i++)
    {
        unsigned long long mov=bitboard.whitepawn;
        while(mov)
        {
            unsigned long long pos = mov & (-mov);
            mov &= (mov-1);
            Bitboard child = bitboard;
            value = -bitboard.blackAB(bitboard,-beta,-alpha,depth-1);
            bitboard = child;
            bitboard.whitepawn^=pos;
            if(value>=beta){return value;}
            if(value>alpha){alpha=value;}
        }
    }
    return alpha;
}

int Bitboard::blackAB(Bitboard bitboard,int alpha,int beta,int depth)
{
    if(bitboard.endgame()){return 0;}
    if(depth<0){return 0;}
    int value;
    for(int i=0;i<16;i++)
    {
        unsigned long long mov=bitboard.blackpawn;
        while(mov)
        {
            unsigned long long pos = mov & (-mov);
            mov &= (mov-1);
            Bitboard child = bitboard;
            value = -bitboard.whiteAB(bitboard,-beta,-alpha,depth-1);
            bitboard = child;
            bitboard.blackpawn^=pos;
            if(value>=beta){return value;}
            if(value>alpha){alpha=value;}
        }
    }
    return alpha;
}

void Bitboard::MoveWhite(SDL_Rect white_pawn[],std::vector<std::pair<int,int>>&white,const Bitboard &bitboard,const int &No)
{
    //bitboard.whiteAB(bitboard);
    //int alpha = bitboard.whiteAB(bitboard,-100,100,5);
    //int color= wite;
    //int value = 0;
    if(No>7){white_pawn[No] = {-48-((No-8)*63),-170,SCREEN_WIDTH,SCREEN_HEIGHT};}
    else{white_pawn[No] = {-48-((No)*63),-107,SCREEN_WIDTH,SCREEN_HEIGHT};}
}

Information Bitboard::Click(SDL_Rect black_pawn[],std::vector<std::pair<int,int>>&black,const int &x,const int &y,const Bitboard &bitboard)
{
    int state=FiniteState::Initial,no=-100;
    for(int i=0; i<16; i++)
    {
        if((black[i].first==x)&&(black[i].second==y))
        {
            no=i;
            state=FiniteState::Select;
            break;
        }
    }
    unsigned long long mov = bitboard.blackmask(x,y);
    mov &= (~bitboard.blackpawn);
    int length = __builtin_popcountll(bitboard.blackmask(x,y));
    std::vector<int>position(length);
    for(int i=0;i<__builtin_popcountll(bitboard.blackmask(x,y));i++)
    {
        unsigned long long firstzero = 0x0000000000000001<<__builtin_ctzll(mov);
        mov ^= firstzero;
        position[i] = __builtin_ctzll(mov);
    }

    if((state==FiniteState::Initial)&&(no>-1))
    {
        black_pawn[no] = {convert_x(x),convert_y(y),SCREEN_WIDTH,SCREEN_HEIGHT};
    }
    return {no,__builtin_ctz(mov)};
}

void Bitboard::MoveBlack(SDL_Rect black_pawn[],std::vector<std::pair<int,int>>&black,const Bitboard &bitboard,const int &BlackID,const int &x,const int &y)
{
    black_pawn[(BlackID+8)%16] = {-48-(x*63),-44-(y*63),SCREEN_WIDTH,SCREEN_HEIGHT};
    black[BlackID].first = x;
    black[BlackID].second = y;
}

unsigned long long Bitboard::whitemask(const int &x,const int &y)const
{
    unsigned long long mask[64] = {0x0000000000000000};
    std::pair<int,int>wlocation[64];
    for(int i=0;i<8;i++)
    {
        wlocation[i].first=i;
        wlocation[i].second=0;
    }
    for(int i=8;i<16;i++)
    {
        wlocation[i].first=(i-8);
        wlocation[i].second=1;
    }
    for(int i=16;i<24;i++)
    {
        wlocation[i].first=(i-16);
        wlocation[i].second=2;
    }
    for(int i=24;i<32;i++)
    {
        wlocation[i].first=(i-8);
        wlocation[i].second=3;
    }
    for(int i=32;i<40;i++)
    {
        wlocation[i].first=(i-8);
        wlocation[i].second=4;
    }
    for(int i=40;i<48;i++)
    {
        wlocation[i].first=(i-8);
        wlocation[i].second=5;
    }
    for(int i=48;i<56;i++)
    {
        wlocation[i].first=(i-8);
        wlocation[i].second=6;
    }
    for(int i=56;i<64;i++)
    {
        wlocation[i].first=(i-8);
        wlocation[i].second=7;
    }
    mask[0] = 0x000000000000C000;
    mask[1] = 0x000000000000E000;
    mask[2] = 0x0000000000007000;
    mask[3] = 0x0000000000003800;
    mask[4] = 0x0000000000001C00;
    mask[5] = 0x0000000000000E00;
    mask[6] = 0x0000000000000700;
    mask[7] = 0x0000000000000300;

    mask[8] = 0x0000000000C00000;
    mask[9] = 0x0000000000E00000;
    mask[10] = 0x0000000000700000;
    mask[11] = 0x0000000000380000;
    mask[12] = 0x00000000001C0000;
    mask[13] = 0x00000000000E0000;
    mask[14] = 0x0000000000070000;
    mask[15] = 0x0000000000030000;

    mask[16] = 0x00000000C0000000;
    mask[17] = 0x00000000E0000000;
    mask[18] = 0x0000000070000000;
    mask[19] = 0x0000000038000000;
    mask[20] = 0x000000001C000000;
    mask[21] = 0x000000000E000000;
    mask[22] = 0x0000000007000000;
    mask[23] = 0x0000000003000000;

    mask[24] = 0x000000C000000000;
    mask[25] = 0x000000E000000000;
    mask[26] = 0x0000007000000000;
    mask[27] = 0x0000003800000000;
    mask[28] = 0x0000001C00000000;
    mask[29] = 0x0000000E00000000;
    mask[30] = 0x0000000700000000;
    mask[31] = 0x0000000300000000;

    mask[32] = 0x0000C00000000000;
    mask[33] = 0x0000E00000000000;
    mask[34] = 0x0000700000000000;
    mask[35] = 0x0000380000000000;
    mask[36] = 0x00001C0000000000;
    mask[37] = 0x00000E0000000000;
    mask[38] = 0x0000070000000000;
    mask[39] = 0x0000030000000000;

    mask[40] = 0x00C0000000000000;
    mask[41] = 0x00E0000000000000;
    mask[42] = 0x0070000000000000;
    mask[43] = 0x0038000000000000;
    mask[44] = 0x001C000000000000;
    mask[45] = 0x000E000000000000;
    mask[46] = 0x0007000000000000;
    mask[47] = 0x0003000000000000;

    mask[48] = 0xC000000000000000;
    mask[49] = 0xE000000000000000;
    mask[50] = 0x7000000000000000;
    mask[51] = 0x3800000000000000;
    mask[52] = 0x1C00000000000000;
    mask[53] = 0x0E00000000000000;
    mask[54] = 0x0700000000000000;
    mask[55] = 0x0300000000000000;

    mask[56] = 0x0000000000000000;
    mask[57] = 0x0000000000000000;
    mask[58] = 0x0000000000000000;
    mask[59] = 0x0000000000000000;
    mask[60] = 0x0000000000000000;
    mask[61] = 0x0000000000000000;
    mask[62] = 0x0000000000000000;
    mask[63] = 0x0000000000000000;

    int chosen = -1;
    for(int i=0;i<64;i++)
    {
        if((x==wlocation[i].first)&&(y==wlocation[i].second))
        {
            chosen=i;
            break;
        }
    }

    return mask[chosen];
}

unsigned long long Bitboard::blackmask(const int &x,const int &y)const
{
    unsigned long long mask[64] = {0x0000000000000000};
    std::pair<int,int>blocation[64];
    for(int i=0;i<8;i++)
    {
        blocation[i].first=i;
        blocation[i].second=0;
    }
    for(int i=8;i<16;i++)
    {
        blocation[i].first=(i-8);
        blocation[i].second=1;
    }
    for(int i=16;i<24;i++)
    {
        blocation[i].first=(i-16);
        blocation[i].second=2;
    }
    for(int i=24;i<32;i++)
    {
        blocation[i].first=(i-8);
        blocation[i].second=3;
    }
    for(int i=32;i<40;i++)
    {
        blocation[i].first=(i-8);
        blocation[i].second=4;
    }
    for(int i=40;i<48;i++)
    {
        blocation[i].first=(i-8);
        blocation[i].second=5;
    }
    for(int i=48;i<56;i++)
    {
        blocation[i].first=(i-8);
        blocation[i].second=6;
    }
    for(int i=56;i<64;i++)
    {
        blocation[i].first=(i-8);
        blocation[i].second=7;
    }

    mask[0] = 0x0000000000000000;
    mask[1] = 0x0000000000000000;
    mask[2] = 0x0000000000000000;
    mask[3] = 0x0000000000000000;
    mask[4] = 0x0000000000000000;
    mask[5] = 0x0000000000000000;
    mask[6] = 0x0000000000000000;
    mask[7] = 0x0000000000000000;

    mask[8] = 0x00000000000000C0;
    mask[9] = 0x00000000000000E0;
    mask[10] = 0x0000000000000070;
    mask[11] = 0x0000000000000038;
    mask[12] = 0x000000000000001C;
    mask[13] = 0x000000000000000E;
    mask[14] = 0x0000000000000007;
    mask[15] = 0x0000000000000003;

    mask[16] = 0x000000000000C000;
    mask[17] = 0x000000000000E000;
    mask[18] = 0x0000000000007000;
    mask[19] = 0x0000000000003800;
    mask[20] = 0x0000000000001C00;
    mask[21] = 0x0000000000000E00;
    mask[22] = 0x0000000000000700;
    mask[23] = 0x0000000000000300;

    mask[24] = 0x0000000000C00000;
    mask[25] = 0x0000000000E00000;
    mask[26] = 0x0000000000700000;
    mask[27] = 0x0000000000380000;
    mask[28] = 0x00000000001C0000;
    mask[29] = 0x00000000000E0000;
    mask[30] = 0x0000000000070000;
    mask[31] = 0x0000000000030000;

    mask[32] = 0x00000000C0000000;
    mask[33] = 0x00000000E0000000;
    mask[34] = 0x0000000070000000;
    mask[35] = 0x0000000038000000;
    mask[36] = 0x000000001C000000;
    mask[37] = 0x000000000E000000;
    mask[38] = 0x0000000007000000;
    mask[39] = 0x0000000003000000;

    mask[40] = 0x000000C000000000;
    mask[41] = 0x000000E000000000;
    mask[42] = 0x0000007000000000;
    mask[43] = 0x0000001C00000000;
    mask[44] = 0x0000003800000000;
    mask[45] = 0x0000000E00000000;
    mask[46] = 0x0000000700000000;
    mask[47] = 0x0000000300000000;

    mask[48] = 0x0000C00000000000;
    mask[49] = 0x0000E00000000000;
    mask[50] = 0x0000700000000000;
    mask[51] = 0x00001C0000000000;
    mask[52] = 0x0000380000000000;
    mask[53] = 0x00000E0000000000;
    mask[54] = 0x0000070000000000;
    mask[55] = 0x0000030000000000;

    mask[56] = 0x00C0000000000000;
    mask[57] = 0x00E0000000000000;
    mask[58] = 0x0070000000000000;
    mask[59] = 0x001C000000000000;
    mask[60] = 0x0038000000000000;
    mask[61] = 0x000E000000000000;
    mask[62] = 0x0007000000000000;
    mask[63] = 0x0003000000000000;

    int chosen = -1;
    for(int i=0;i<64;i++)
    {
        if((x==blocation[i].first)&&(y==blocation[i].second))
        {
            chosen=i;
            break;
        }
    }
    return mask[chosen];
}

Bitboard::~Bitboard()
{

}
