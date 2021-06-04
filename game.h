#ifndef GAME_H
#define GAME_H

#include "common.h"

/*
* DEFINES, ENUMERATIONS
*/

#define BALLSLEFT 3
enum { left, none, right }; // Etat du changement de niveau
enum { running, pause, win, lose, retry, invalid_lvl };   // Etat du jeu
enum { no_move, moving }; // Etat de la balle
enum { axis_x, axis_y };

// End (defines, enumerations)



/*
* STRUCTURES
*/

typedef struct
  {
  float x_ball;
  float y_ball;
  int d_ball;  // Angle de direction
  float f_ball; // Force
  int x_racket;
  const int y_racket;
  } ThinkVars;

// End (structures)



/*
* FUNCTIONS
*/

// Controls
void do_game_controls();
void moveRacket(char dir);
void moveRacketWithJoystick();

// Display
void do_game_graphics();
void drawLevelInfo();
void drawIntraFontStuff();
void drawDebug();
void drawAllBricks();
void drawExplosions();
void flashScreen();

// Levels
char getLevelNumber();
int  getLevelsPosition();
char getLevelsMoveState();
void goToLevelMoveState(char state);
void reInitBricks(char number);
void copyOneToAnother(char a, char b);
void moveBricks();
void doExplosionsStuff();
void reloadEditedLevels();
void game_levels_init();
void do_game_level();

// Think
void initVars();
void resetBallsLeft();
char getThinkState();
void goToThinkState(char state);
char getBallsLeft();
int getAngleVar();
char getMoveState();
void goToMoveState(char movestate);
ThinkVars* getThinkPointer();
void do_game_think();
void detectEdges();
void detectBricksEdges();
void brickDestruction(int number, char annihilate);
void destroyExplosiveBrick(int number);
void moveItFromTrajectory();
void invertTrajectory(char axis, char test);
void detectIfWin();
void detectIfLose();

// End (functions)

#endif
