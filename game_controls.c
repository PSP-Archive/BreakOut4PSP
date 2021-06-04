#include <pspctrl.h>
#include <math.h>

#include "main.h"
#include "game.h"
#include "common.h"

char pausestate=0;
void do_game_controls()
  {
  updateControls();

  switch (getThinkState())
    {
    case running:
      if      (testButton(PSP_CTRL_LEFT,0))  moveRacket(-1);
      else if (testButton(PSP_CTRL_RIGHT,0)) moveRacket(+1);
      else    moveRacketWithJoystick();
      if      (testButton(PSP_CTRL_CROSS,1) && getLevelsMoveState() == none) goToMoveState(moving);
      if      (testButton(PSP_CTRL_START,1)) goToThinkState(pause);
      if      (testButton(PSP_CTRL_LTRIGGER,0)) goToLevelMoveState(right);
      else if (testButton(PSP_CTRL_RTRIGGER,0)) goToLevelMoveState(left);
    break;
    case win:
      if      (testButton(PSP_CTRL_CROSS,2)) { initVars(); goToLevelMoveState(left); }
      else if (testButton(PSP_CTRL_CIRCLE,2)) { initVars(); resetBallsLeft(); reInitBricks(3); goToThinkState(running); }
    break;
    case lose:
      if      (testButton(PSP_CTRL_CROSS,2))  { initVars(); goToThinkState(running); }
    break;
    case retry:
      if      (testButton(PSP_CTRL_CROSS,2))  { initVars(); resetBallsLeft();
                                                reInitBricks(3); goToThinkState(running); }
    break;
    case pause:
      if      (!pausestate && testButton(PSP_CTRL_START,2)) pausestate=1;
      else if ( pausestate && testButton(PSP_CTRL_START,2)) { goToThinkState(running); pausestate=0; }
      if      (testButton(PSP_CTRL_CIRCLE,2)) setGameState(menu);
    break;
    case invalid_lvl:
      if      (testButton(PSP_CTRL_LEFT,0))  moveRacket(-1);
      else if (testButton(PSP_CTRL_RIGHT,0)) moveRacket(+1);
      else    moveRacketWithJoystick();
      if      (testButton(PSP_CTRL_LTRIGGER,0)) goToLevelMoveState(right);
      else if (testButton(PSP_CTRL_RTRIGGER,0)) goToLevelMoveState(left);
    break;
    }
  }

#define RACKETMOVE 6
void moveRacket(char dir)
  {
  ThinkVars* tmp = getThinkPointer();
  Brick* tmplevel = getLevelPointer(1);
  tmp->x_racket += dir*RACKETMOVE;
  if (tmp->x_racket<0) tmp->x_racket=0;
  else if (tmp->x_racket+96>480) tmp->x_racket=480-96;
  int i;
  for (i=0; i!=getLevelSize(1); i++)
    {
    if (tmplevel[i].d_brick)
      {
      if (tmplevel[i].y_brick+tmplevel[i].yd_brick>tmp->y_racket && tmplevel[i].y_brick<tmp->y_racket+10)
        {
        if (tmp->x_racket<tmplevel[i].tx_brick+tmplevel[i].xd_brick && tmp->x_racket>tmplevel[i].tx_brick+tmplevel[i].xd_brick-RACKETMOVE-2) tmp->x_racket = tmplevel[i].tx_brick+tmplevel[i].xd_brick;
        else if (tmp->x_racket+96>tmplevel[i].tx_brick && tmp->x_racket+96<tmplevel[i].tx_brick+RACKETMOVE+2) tmp->x_racket=tmplevel[i].tx_brick-96;
        }
      }
    }
  }

#define DEAD_ZONE 30

void moveRacketWithJoystick()
	{
  ThinkVars* tmp = getThinkPointer();
  Brick* tmplevel = getLevelPointer(1);
	if (fabs(getPad()->Lx-127)>DEAD_ZONE) tmp->x_racket += (getPad()->Lx-127)*RACKETMOVE/127;
  if (tmp->x_racket<0) tmp->x_racket=0;
  else if (tmp->x_racket+96>480) tmp->x_racket=480-96;
  int i;
  for (i=0; i!=getLevelSize(1); i++)
    {
    if (tmplevel[i].d_brick)
      {
      if (tmplevel[i].y_brick+tmplevel[i].yd_brick>tmp->y_racket && tmplevel[i].y_brick<tmp->y_racket+10)
        {
        if (tmp->x_racket<tmplevel[i].tx_brick+tmplevel[i].xd_brick && tmp->x_racket>tmplevel[i].tx_brick+tmplevel[i].xd_brick-RACKETMOVE-2) tmp->x_racket = tmplevel[i].tx_brick+tmplevel[i].xd_brick;
        else if (tmp->x_racket+96>tmplevel[i].tx_brick && tmp->x_racket+96<tmplevel[i].tx_brick+RACKETMOVE+2) tmp->x_racket=tmplevel[i].tx_brick-96;
        }
      }
    }
	}
