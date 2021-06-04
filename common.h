#ifndef COMMON_H
#define COMMON_H

#include "./librairies/graphics.h"
#include <pspctrl.h>

/*
* DEFINES, MACROS
*/

#define NBRTYPES 5
#define MAXLEVELSIZE 100
#define BRICK_WIDTH 60
#define BRICK_HEIGHT 20
#define RACKET_WIDTH 96

#define BRICK_TEX_U(nbr)\
  ((nbr == 0)?BRICK_WIDTH*0:\
  ((nbr == 1)?BRICK_WIDTH*1:\
  ((nbr == 2)?BRICK_WIDTH*2:\
  ((nbr == 10)?BRICK_WIDTH*3:\
  ((nbr == 20)?BRICK_WIDTH*4:0)))))
#define BRICK_DIRECT_U(i) \
  BRICK_TEX_U(lvl[i].t_brick)

// End (defines, macros)



/*
* STRUCTURES
*/

// Common
typedef struct
  {
  char list[64][32];  
  } Matrix;

// Display
typedef struct
  {
  Image* background;
  Image* racket;
  Image* ball;
  Image* brick;
  Image* b_brick[2];
  Image* explosion;
  Image* geecko;
  Image* breakout4psp;
  Image* fade;
  } Sprites;

// Language
typedef struct
  {
  char* choice[5];
  char* sub_back;
  } MenuText;
typedef struct
  {
  char* thx;
  } CreditsText;
typedef struct
  {
  char* sball;
  char* ballleft;
  char* ballsleft;
  char* tlose;
  char* slose;
  char* sretry;
  char* tpause;
  char* spause;
  char* twin;
  char* swin1;
  char* swin2;
  char* invalidlvl;
  } GameText;
typedef struct
  {
  char* Lchoice[5];  
  char* Rchoice[8];
  char* yes;
  char* no;
  } EditorText;
typedef struct
  {
  char* choice[2];  
  } OptionsText;
typedef struct
  {
  MenuText menu;
  CreditsText credits;
  OptionsText options;
  GameText game;
  EditorText editor;
  } Text;

// Levels
typedef struct
  {
  int x_brick;
  int y_brick;
  int tx_brick;
  int ty_brick;
  int t_brick;
  char d_brick;
  int xm_brick;
  int ym_brick;
  int xd_brick;
  int yd_brick;
  char e_brick;
  } Brick;

// End (structures)



/*
* FUNCTIONS
*/

// Config
void writeConfigFile();

// Controls
char testButton(unsigned int button, char test);
void updateControls();
SceCtrlData* getPad();

// Levels
char* getModifiedP();
void countNol();
char getNol();
void setNol(char number);
void loadLevel(int targetnbr, char levelnbr);
void clearLevel(int levelnbr);
Brick* getLevelPointer(int number);
char getLevelSize(int number);
void changeLevelSize(int buffernbr, char new);

// Display
int getBrickType(int nbr);
void common_disp_init();
void loadTheme(char* name);
void listThemes();
Matrix getThemeList();
int getThemeNbr();
int getThemeChoosed();
void intraFont_init();
void startDraw();
void finishDraw();
void drawLR();
void drawTopInfo(int number1, int number2, char mode);
void drawFade();
void setFade(char mode, Image* pointer);
unsigned char getFadeValue();
void fastDrawBricks(int x, char number, char mode);
#define fastDrawGameBricks(x,number)\
  fastDrawBricks(x,number,0)
#define fastDrawEditorBricks()\
  fastDrawBricks(0,3,1)
void intraFontRestoreDraw();

// Language
void initLanguages();
void loadLanguage(char* name);
Matrix getList();
int getListNbr();
int getNbrChoosed();

// Sound
//void initAudio();
//void playAudio(char* nameID);

// End (functions)

#endif
