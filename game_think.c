#include <math.h>
#include <stdlib.h>

#include "game.h" 

ThinkVars vars = {0,0,0,0,480/2-96/2,235};
char game_think_init=0,
     game_think_state=running,
     game_move_state=no_move,
     game_balls_left=BALLSLEFT;

char getThinkState()
  {
  return game_think_state;
  }

void goToThinkState(char state)
  {
  game_think_state = state;
  game_balls_left -= (state == lose);
  }

char getBallsLeft()
  {
  return game_balls_left;
  }

int getAngleVar()
  {
  return vars.d_ball;
  }

char getMoveState()
  {
  return game_move_state;
  }

void goToMoveState(char movestate)
  {
  game_move_state = movestate;
  }

ThinkVars* getThinkPointer()
  {
  return &vars;  
  }

/**
* Dans le vif du sujet...
*/

void initVars()
  {
  vars.x_ball=vars.x_racket+96/2-6/2;
  vars.y_ball=vars.y_racket-8;
  vars.d_ball=0;
  vars.f_ball=3;
  game_move_state=no_move;
  }

void resetBallsLeft()
  {
  game_balls_left=BALLSLEFT;
  }

void do_game_think()
  {
  if (game_move_state == no_move) vars.x_ball = vars.x_racket+96/2-6/2;
  else if (game_move_state == moving && game_think_state != pause) 
    {
    moveItFromTrajectory();
    detectEdges();
    detectBricksEdges();
    }
  if (game_move_state != no_move) detectIfWin();
  detectIfLose();
  }

void detectEdges()
  {
  if (vars.x_ball <= 0) invertTrajectory(axis_y,2);
  else if (vars.x_ball+8 >= 480) invertTrajectory(axis_y,1);
  if (vars.y_ball <= 0) invertTrajectory(axis_x,2);
  else if (vars.y_ball >= 272 && game_think_state == running)
    {
    goToThinkState(lose);
    }
  else if (vars.y_ball >= 272)
    {
    vars.x_ball = 480/2-8/2;
    vars.y_ball = 272+1;
    }
  if (vars.x_ball+8 >= vars.x_racket && vars.x_ball <= vars.x_racket+96) // Raquette
    {
    if ((vars.x_ball+8 >= vars.x_racket && vars.x_ball <= vars.x_racket+5) &&
        (vars.y_ball+8 >= vars.y_racket && vars.y_ball <= vars.y_racket+10)) // Gauche
      {
      invertTrajectory(axis_y,1);
      }
    else if ((vars.x_ball+8 >= vars.x_racket+96-5 && vars.x_ball <= vars.x_racket+96) &&
             (vars.y_ball+8 >= vars.y_racket && vars.y_ball <= vars.y_racket+10)) // Droite
      {
      invertTrajectory(axis_y,2);
      }
    if (vars.y_ball+8 >= vars.y_racket && vars.y_ball+8 <= vars.y_racket+5) // Haut
      {
      vars.d_ball=(vars.x_ball+3-vars.x_racket)*110/96-55;
      if (vars.d_ball < 0) vars.d_ball += 360; // Correction si l'angle est négatif
      vars.f_ball=abs(((vars.x_ball-vars.x_racket)*5.1/96)-2.05)+3.7;
      }
    }
  }

void detectBricksEdges()
  {
  int i;
  Brick* lvl = getLevelPointer(1);
  for (i=0; i!=getLevelSize(1); i++)
    {
    if (lvl[i].d_brick && getLevelsMoveState()==none)
      {
      if ((vars.x_ball+8 >= lvl[i].tx_brick &&
           vars.x_ball <= lvl[i].tx_brick+lvl[i].xd_brick) &&
          (vars.y_ball+8 >= lvl[i].ty_brick &&
           vars.y_ball+8 <= lvl[i].ty_brick+5)) // Haut
        {
        invertTrajectory(axis_x,1);
        brickDestruction(i,0);
        }
      else if ((vars.x_ball+8 >= lvl[i].tx_brick &&
                vars.x_ball <= lvl[i].tx_brick+lvl[i].xd_brick) &&
               (vars.y_ball <= lvl[i].ty_brick+lvl[i].yd_brick &&
                vars.y_ball+5 >= lvl[i].ty_brick+lvl[i].yd_brick)) // Bas
        {
        invertTrajectory(axis_x,2);
        brickDestruction(i,0);
        }
      else if ((vars.y_ball+8 >= lvl[i].ty_brick &&
                vars.y_ball <= lvl[i].ty_brick+lvl[i].yd_brick) &&
               (vars.x_ball+8 >= lvl[i].tx_brick &&
                vars.x_ball+8 <= lvl[i].tx_brick+5)) // Gauche
        {
        invertTrajectory(axis_y,1);
        brickDestruction(i,0);
        }
      else if ((vars.y_ball+8 >= lvl[i].ty_brick &&
                vars.y_ball <= lvl[i].ty_brick+lvl[i].yd_brick) &&
               (vars.x_ball <= lvl[i].tx_brick+lvl[i].xd_brick &&
                vars.x_ball+5 >= lvl[i].tx_brick+lvl[i].xd_brick)) // Droite
        {
        invertTrajectory(axis_y,2);
        brickDestruction(i,0);
        }
      else if ((vars.y_ball+8 >= lvl[i].ty_brick &&
                vars.y_ball <= lvl[i].ty_brick+lvl[i].yd_brick) &&
               (vars.x_ball <= lvl[i].tx_brick+lvl[i].xd_brick &&
                vars.x_ball+8 >= lvl[i].tx_brick)) // Intérieur
        {
        brickDestruction(i,0);
        }
      }
    }
  }

void brickDestruction(int number, char annihilate)
  {
  Brick* lvl = getLevelPointer(1);
  if (lvl[number].t_brick==20 && lvl[number].d_brick==2)
    {
    lvl[number].d_brick=1;
    lvl[number].e_brick=10;
    }
  if (lvl[number].t_brick!=20 && lvl[number].d_brick!=0)
    {
    if (annihilate) lvl[number].d_brick=0;
    else if (!annihilate && lvl[number].t_brick!=10)  lvl[number].d_brick--;
    }
  }

void destroyExplosiveBrick(int number)
  {
  int i;
  Brick* lvl = getLevelPointer(1);
  lvl[number].d_brick=0;
  for (i=0; i != getLevelSize(1); i++)
    {
    if (number!=i)
      {
      if ((lvl[i].ty_brick+lvl[i].yd_brick >= lvl[number].ty_brick-15 &&
           lvl[i].ty_brick <= lvl[number].ty_brick+lvl[number].yd_brick+15) &&
          (lvl[i].tx_brick+lvl[i].xd_brick >= lvl[number].tx_brick-50 &&
           lvl[i].tx_brick <= lvl[number].tx_brick+lvl[number].xd_brick+50))
        {
      	brickDestruction(i,1);
        }
      }
    }
  }

void moveItFromTrajectory()
  {
  vars.x_ball += vars.f_ball*sinf(vars.d_ball*3.14159F/180);
  vars.y_ball -= vars.f_ball*cosf(vars.d_ball*3.14159F/180);
  }

void invertTrajectory(char axis, char test)
  {
  int oldTrajectory = vars.d_ball;
  if (axis == axis_y)
    {
    vars.d_ball = 360-vars.d_ball;
    if ((test==1 && vars.d_ball<180) || 
        (test==2 && vars.d_ball>180))
      {
      vars.d_ball = oldTrajectory;
      }
    }
  else if (axis == axis_x)
    {
    vars.d_ball = 180+360*(vars.d_ball>180)-vars.d_ball;
    if ((test==1 && (vars.d_ball > 90 && vars.d_ball < 270)) ||
        (test==2 && (vars.d_ball < 90 || vars.d_ball > 270)))
      {
      vars.d_ball = oldTrajectory;
      }
    }
  }

void detectIfWin()
  {
  int i, win_i=0;
  for (i=0; i!=getLevelSize(1); i++)
    {
    if (!getLevelPointer(1)[i].d_brick || (getLevelPointer(1)[i].t_brick == 10 || getLevelPointer(1)[i].t_brick == 20)) win_i++;
    }
  if (win_i == getLevelSize(1)) goToThinkState(win);
  }

void detectIfLose()
  {
  if (!game_balls_left) goToThinkState(retry);
  }
