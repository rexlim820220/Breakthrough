#ifndef BITBOARD_H_INCLUDED
#define BITBOARD_H_INCLUDED
#include <utility>
#include <vector>

class Bitboard{
public:
    Bitboard();
    bool endgame();
    float Evaluation(Bitboard,int);
    int whiteAB(Bitboard,int,int,int);
    int blackAB(Bitboard,int,int,int);
    void MoveWhite(SDL_Rect *,std::vector<std::pair<int,int>>&,Bitboard,int);
    void MoveBlack(SDL_Rect *,std::vector<std::pair<int,int>>&,Bitboard,int,int,int);
    int Click(SDL_Rect *,std::vector<std::pair<int,int>>&,int,int);
    unsigned long long whitemask(int,int);
    unsigned long long blackmask(int,int);
    ~Bitboard();
private:
    unsigned long long whitepawn;
    unsigned long long blackpawn;
};

enum whose_turn{isblack,iswhite};
enum FiniteState{Initial,Select};

#endif // BITBOARD_H_INCLUDED
