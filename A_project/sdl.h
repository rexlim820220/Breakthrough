#ifndef SDL_H_INCLUDED
#define SDL_H_INCLUDED

bool init();
bool loadmedia();
void set_white_position(SDL_Rect *);
void set_black_position(SDL_Rect *);
void loadbackground();
void loadwhitepawn(SDL_Rect *);
void loadblackpawn(SDL_Rect *,int,int);
void getlocation(int *,int *);
int convert_x(int);
int convert_y(int);
void close();
void slot();
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;

#endif // SDL_H_INCLUDED
