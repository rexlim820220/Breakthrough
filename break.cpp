#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstdlib>
using namespace std;

enum Color{black,white};

class Bitboard{
public:
    Bitboard(){
        whitepawn=0xffff000000000000;
        blackpawn=0x000000000000ffff;
    }
    Bitboard make_move(int,int,int);
    bool endgame();
    void showboard();
    float Evaulate(int color);
    ~Bitboard(){}
private:
    uint64_t whitepawn;
    uint64_t blackpawn;
};

Bitboard Bitboard::make_move(int x,int y,int color)
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
}

void Bitboard::showboard()
{
    for(int i=0;i<64;i++)
    {
        unsigned long long a=1;
        a=a<<i;
        if((blackpawn&a)!=0){
            printf("*");
        }
        else if((whitepawn&a)!=0){
            printf("@");
        }
        else{
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
        for(int i=7;i>=0;i--)
        {
            for(int j=i*8;j<i*8+8;j++)
            {
                if(blackpawn&(1<<j)==0){/*no pawn*/;}
                else{score+=pow(8-i,2);/*exist pawn*/}
            }
        }
        return score;
    }
    else{
        for(int i=0;i<8;i++)
        {
            for(int j=i*8;j<i*8+8;j++)
            {
                if(whitepawn&(1<<j)==0){/*no pawn*/;}
                else{score+=pow(i,2);/*exist pawn*/}
            }
        }
        return score;
    }
}

bool Bitboard::endgame()
{
    for(int i=56;i<64;i++)
    {
        if((blackpawn&(1ll<<i))!=0){return true;}
    }
    for(int i=0;i<8;i++)
    {
        if((whitepawn&(1ll<<i))!=0){return true;}
    }
    return(blackpawn==0x0000000000000000||
           whitepawn==0x0000000000000000);
}

int NegaMax(int depth,Bitboard board,int color)
{
    int value;
    int best=-1e6;
    if(depth<=0||board.endgame())
    {
        return value;
    }
    //GenerateLegalMoves();
    int x=0,y=0;
    while(!board.endgame())
    {
        board.make_move(x,y,color);
        value=-NegaMax(depth-1,board,color);
        //unmakemove;
        if(value>best)
        {
            best=value;
        }
    }
    return best;
}

int main(int argc,char **argv)
{
    Bitboard bitboard;
    int pos_x=0,pos_y=0,i=0,color=1;
    cout<<"First,or second?"<<endl;
    cin>>color;
    if(color!=0||color!=1){color=1;}
    while(!bitboard.endgame())
    {
        cin>>pos_x>>pos_y;
        if((pos_x<48||pos_x>55)||(pos_y<40||pos_y>47)){
            pos_x=48+i;pos_y=40+i;
            i+=1;
            if(i>15){i*=(-1);}
        }
        system("cls");
        bitboard.make_move(pos_x,pos_y,color);
        cout<<endl;
        system("pause");
        if(color==0){color=1;}
        else{color=0;}
    }
    return 0;
}
