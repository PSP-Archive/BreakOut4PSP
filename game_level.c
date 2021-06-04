#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pspiofilemgr.h>
#include <pspctrl.h>

#include "common.h"
#include "game.h"

char levels_move_state=none,
     levelnumber=1;
int  levels_position=0;

char getLevelNumber()
  {
  return levelnumber;
  }

int getLevelsPosition()
  {
  return levels_position;
  }

char getLevelsMoveState()
  {
  return levels_move_state;
  }

void goToLevelMoveState(char state)
  {
  levels_move_state=state;
  initVars();
  if (state == left) reInitBricks(2);
  else if (state == right) reInitBricks(0);
  }

void reInitBricks(char number)
  {
  int i,i2;
  for (i=0; i!=3; i++)
    {
    if (i==number || number==3)
      {
      for (i2=0; i2!=getLevelSize(i); i2++)
        {
        getLevelPointer(i)[i2].d_brick=abs(getLevelPointer(i)[i2].t_brick+1);
        if (getLevelPointer(i)[i2].t_brick==20)
          {
          getLevelPointer(i)[i2].d_brick=2;
          }
        getLevelPointer(i)[i2].e_brick=0;
        }
      }
    }
  }

void copyOneToAnother(char a, char b)
  {
  Brick* tmpa = getLevelPointer(a);
  Brick* tmpb = getLevelPointer(b);
  clearLevel(b);
  int i;
  for (i=0; i!=getLevelSize(a); i++) tmpb[i]=tmpa[i];
  changeLevelSize(b,getLevelSize(a));
  }

#define ANGLEINCREMENT 4
int angle=0;
void moveBricks()
  {
  int i,i2;
  for (i=0; i!=3; i++)
    {
    if ((i==0 && getLevelsPosition()>0) || (i==2 && getLevelsPosition()<0) || i==1)
      {
      for(i2=0; i2!=getLevelSize(i); i2++)
        {
        if (getLevelPointer(i)[i2].d_brick!=0)
          {
          if (getLevelPointer(i)[i2].xm_brick)
            getLevelPointer(i)[i2].tx_brick = getLevelPointer(i)[i2].x_brick+
                                              getLevelPointer(i)[i2].xm_brick*
                                              sinf(angle*3.14159F/180);
          if (getLevelPointer(i)[i2].ym_brick)
            getLevelPointer(i)[i2].ty_brick = getLevelPointer(i)[i2].y_brick+
                                              getLevelPointer(i)[i2].ym_brick*
                                              sinf(angle*3.14159F/180);
          }
        }
      }
    }
  angle+=ANGLEINCREMENT;
  if (angle>=360) angle-=360;
  }

void doExplosionsStuff()
  {
  int i;
  for (i=0; i!=getLevelSize(1); i++)
    {
    if (getLevelPointer(1)[i].e_brick!=0) getLevelPointer(1)[i].e_brick--;
    if (getLevelPointer(1)[i].e_brick==5) destroyExplosiveBrick(i);
    }
  }

void reloadEditedLevels()
  {
  if (levelnumber<1) levelnumber = getNol();
  else if (levelnumber>getNol()) levelnumber = 1;
  if (getModifiedP()[0])
    {
    loadLevel(0,(levelnumber-1<1) ? getNol() : (levelnumber-1));
    reInitBricks(0);
    getModifiedP()[0]=0;
    }
  if (getModifiedP()[1])
    {
    loadLevel(1,levelnumber);
    reInitBricks(1);
    getModifiedP()[1]=0;
    }
  if (getModifiedP()[2])
    {
    loadLevel(2,(levelnumber+1>getNol()) ? 1 : (levelnumber+1));
    reInitBricks(2);
    getModifiedP()[2]=0;
    }
  }

void checkValidLevel()
  {
  if (getThinkState() == invalid_lvl) return;
  Brick* lvl = getLevelPointer(1);
  int i, vcount=0;
  for (i=0; i!=getLevelSize(1); i++)
    if (lvl[i].t_brick >= 0 && lvl[i].t_brick <= 2) vcount++;
  if (!vcount) goToThinkState(invalid_lvl);
  }

void game_levels_init()
  {
  loadLevel(0,getNol());
  loadLevel(1,levelnumber);
  loadLevel(2,levelnumber+1);
  reInitBricks(3);
  }

#define LEVELMOVE 10
void do_game_level()
  {
  reloadEditedLevels();
  doExplosionsStuff();

  if (levels_move_state==left)
    {
    levels_position-=LEVELMOVE*((testButton(PSP_CTRL_CROSS,0))?2:1);
    if (levels_position == 0 && !testButton(PSP_CTRL_RTRIGGER,0))
      {      
      levels_move_state=none;
      }
    else if (levels_position <= -480)
      {
      if (!testButton(PSP_CTRL_RTRIGGER,0))
        {
        levels_move_state=none;
        levels_position=0;
        }
      else levels_position+=480;
      levelnumber++;
      if (levelnumber>getNol()) levelnumber=1;
      copyOneToAnother(1,0);
      copyOneToAnother(2,1);
      loadLevel(2,levelnumber+1);
      resetBallsLeft();
      reInitBricks(3);
      initVars();
      if (getThinkState()!=pause) goToThinkState(running);
      checkValidLevel();
      }
    }
  else if (levels_move_state==right)
    {
    levels_position+=LEVELMOVE*((testButton(PSP_CTRL_CROSS,0))?2:1);
    if (levels_position == 0 && !testButton(PSP_CTRL_LTRIGGER,0))
      {
      levels_move_state=none;
      }
    else if (levels_position >= 480)
      {
      if (!testButton(PSP_CTRL_LTRIGGER,0))
        {
        levels_move_state=none;
        levels_position=0;
        }
      else levels_position-=480;
      levelnumber--;
      if (levelnumber<1) levelnumber=getNol();
      copyOneToAnother(1,2);
      copyOneToAnother(0,1);
      loadLevel(0,levelnumber-1);
      resetBallsLeft();
      reInitBricks(3);
      initVars();
      if (getThinkState()!=pause) goToThinkState(running);
      checkValidLevel();
      }
    }

  moveBricks();
  }

