#ifndef BITBOARD_H_INCLUDED
#define BITBOARD_H_INCLUDED
#include <utility>
#include <vector>

struct Word{
    int a;
    int b;
};

struct Information{
    int no;
    int each_bit_movable;
};

class Bitboard{
public:
    Bitboard();
    bool endgame();
    float Evaluation(Bitboard,int);
    int whiteAB(Bitboard,int,int,int);
    int blackAB(Bitboard,int,int,int);
    void MoveWhite(SDL_Rect *,std::vector<std::pair<int,int>>&,const Bitboard&,const int&);
    int RandMakeMove(SDL_Rect *,Bitboard&,int No)const;
    void MoveBlack(SDL_Rect *,std::vector<std::pair<int,int>>&,const Bitboard&,const int&,const int&,const int&);
    Information Click(SDL_Rect *,std::vector<std::pair<int,int>>&,const int&,const int&,const Bitboard&);
    unsigned long long whitemask(const int&,const int&)const;
    unsigned long long blackmask(const int&,const int&)const;
    ~Bitboard();
private:
    unsigned long long whitepawn;
    unsigned long long blackpawn;
};

enum whose_turn{isblack,iswhite};
enum FiniteState{Initial,Select};

#endif // BITBOARD_H_INCLUDED
