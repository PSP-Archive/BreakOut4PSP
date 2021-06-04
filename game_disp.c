#include <pspdebug.h>
#include <pspctrl.h>
#include <pspgu.h>
#include <stdio.h>

#include "game.h"
#include "common.h"

#include "./librairies/graphics.h"
#include "./librairies/intraFont.h"

#define DEBUG 0

char flashes; // Pour limiter le nombre de flashs par image (explosions)
extern intraFont* ifl[2];
extern Sprites sprites;
extern Text text;

void do_game_graphics()
  {
  startDraw();

  ThinkVars* tmp = getThinkPointer();

  blitImagePartToScreen(0,0,480,272,sprites.background,0,0); // Fond d'ecran
  blitImageToScreen(8,8,sprites.ball,tmp->x_ball,tmp->y_ball); // Balle
  blitImagePartToScreen(0,0,96,10,sprites.racket,tmp->x_racket,tmp->y_racket); // Raquette
  drawAllBricks(); // Briques
  if (getThinkState()==pause) fillScreenRect(TBLACK,0,0,480,272);
  drawLR(); // <L et R>
  if (getLevelsMoveState()!=none) drawLevelInfo(); // Info sur le level en cours
  drawIntraFontStuff(); // IntraFont
  if (DEBUG) drawDebug(); // Debug
  intraFontRestoreDraw();
  drawExplosions(); // Explosions
  
  finishDraw();
  }

void drawAllBricks()
  {
  sceGuEnable(GU_DEPTH_TEST);
  if (getLevelsPosition() > 0) fastDrawGameBricks(-480,0);
  fastDrawGameBricks(0,1);
  if (getLevelsPosition() < 0) fastDrawGameBricks(480,2);
  sceGuDisable(GU_DEPTH_TEST);
  }

void drawLevelInfo()
  {
  char tmpnbr=0;
  if      (getLevelsMoveState()==left  && getLevelsPosition()> 0) tmpnbr=getLevelNumber();
  else if (getLevelsMoveState()==left  && getLevelsPosition()<=0) tmpnbr=getLevelNumber()+1;
  else if (getLevelsMoveState()==right && getLevelsPosition()< 0) tmpnbr=getLevelNumber();
  else if (getLevelsMoveState()==right && getLevelsPosition()>=0) tmpnbr=getLevelNumber()-1;
  if      (tmpnbr<1) tmpnbr=getNol();
  else if (tmpnbr>getNol()) tmpnbr=1;
  drawTopInfo(tmpnbr,getNol(),0);
  }

#define PRINT(text1,text2,button)\
  if (text1)\
    {\
    intraFontSetStyle(ifl[0],1.25f,color,scolor,INTRAFONT_ALIGN_CENTER);\
    intraFontPrint(ifl[0],480/2,272/2,text1);\
    }\
  intraFontSetStyle(ifl[0],1.00f,color,(button && testButton(button,0))?tcolor:0,INTRAFONT_ALIGN_CENTER);\
  intraFontPrint(ifl[0],240,260,text2);\
  
void drawIntraFontStuff()
  {
  int color=BLACK, scolor=WHITE, tcolor=LITERED;
  switch (getThinkState())
    {
    case running:
      if (getMoveState() == no_move && getLevelsMoveState() == none)
        {
        PRINT("",text.game.sball,0)
        }
    break;
    case pause:
      color=LITEGRAY, scolor=0, tcolor=TRED;
      PRINT(text.game.tpause,text.game.spause,PSP_CTRL_START|PSP_CTRL_CIRCLE)
      color=BLACK, scolor=WHITE, tcolor=LITERED;
    break;
    case win:
      if (getLevelsMoveState() == none)
        {
        PRINT(text.game.twin,(getLevelNumber()!=getNol())?text.game.swin1:text.game.swin2,
              PSP_CTRL_CROSS|PSP_CTRL_CIRCLE)
        }
    break;
    case lose:
      PRINT(text.game.tlose,text.game.slose,PSP_CTRL_CROSS)
      char buffer[20];
      if (getBallsLeft() == 1) sprintf(buffer,"(%d %s)",getBallsLeft(),text.game.ballleft);
      else                     sprintf(buffer,"(%d %s)",getBallsLeft(),text.game.ballsleft);
      intraFontSetStyle(ifl[0], 1.00f, BLACK, WHITE,INTRAFONT_ALIGN_CENTER);
      intraFontPrint(ifl[0],480/2,272/2+20,buffer);
    break;
    case retry:
      PRINT(text.game.tlose,text.game.sretry,PSP_CTRL_CROSS)
    break;
    case invalid_lvl:
      if (getLevelsMoveState() == none)
        {
        PRINT(text.game.invalidlvl,"",0)
        }
    break;
    }
  }

void drawDebug()
  {
  intraFontSetStyle(ifl[0],0.75f,BLACK,0,INTRAFONT_ALIGN_LEFT);
  intraFontPrintf(ifl[0],5,265," ");
  }

void drawExplosions()
  {
  sceGuDisable(GU_DEPTH_TEST);
  Brick* tmplevel; int i,p;
  flashes=0; // Reinitialisation
  tmplevel = getLevelPointer(1);
  for (i=0; i != getLevelSize(1); i++)
    {
    p=0;
    if (tmplevel[i].d_brick==1 || tmplevel[i].d_brick==0)
      {
      if      (tmplevel[i].e_brick>=8 && tmplevel[i].e_brick<=10) flashScreen(); 
      if      (tmplevel[i].e_brick>=7) p=180;
      else if (tmplevel[i].e_brick>=4) p=360;
      if (p!=0 && tmplevel[i].t_brick==20)
        blitImagePartToScreen(p-180,0,180,60,sprites.explosion,
                              tmplevel[i].tx_brick+tmplevel[i].xd_brick/2-180/2+getLevelsPosition(),
                              tmplevel[i].ty_brick+tmplevel[i].yd_brick/2-60/2);
      }
    }
  }

void flashScreen()
  {
  if (flashes<3)
    {
    fillScreenRect(0x64F0F0FF,0,0,480,272);
    flashes++;
    }
  }
