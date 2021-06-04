#include <pspctrl.h>
#include <pspgu.h>
#include <stdio.h>
#include <math.h>

#include "./librairies/graphics.h"
#include "./librairies/intraFont.h"

#include "common.h"
#include "menu.h"
#include "main.h"

extern intraFont* ifl[2];
extern Sprites sprites;
extern Text text;
int nbritem=0, choicenbritem=0, where=in_menu, status=0;

#define DRAWSUBMENU()\
  intraFontSetStyle(ifl[0],1.00f,TBLACK,(testButton(PSP_CTRL_CIRCLE,0))\
                                        ? LITERED : 0,\
                                        INTRAFONT_ALIGN_CENTER);\
  intraFontPrint(ifl[0],240,260,text.menu.sub_back);
void do_menu_disp()
  {
  if (status==quit && getFadeValue()==255) setGameState(quit);

  startDraw();

  blitImagePartToScreen(0,0,480,272,sprites.background,0,0);
  int i;
  char tmpbuffer[64];
  switch (where)
    {
    case in_menu:
      blitImageToScreen(270,68,sprites.breakout4psp,480/2-272/2,0);
      for (i=0; i!=NBR; i++)
        {
        intraFontSetStyle(ifl[1],(i==0)?1.00f:0.70f,BLACK,
                          (nbritem==i)?(testButton(PSP_CTRL_CROSS,0))?LITERED:WHITE:0,
                          INTRAFONT_ALIGN_CENTER);
        intraFontPrint(ifl[1],480/2,START_Y+i*GAP_Y,text.menu.choice[i]);
        }
    break;
    case in_credits:
      blitImageToScreen(100,100,sprites.geecko,480-100,0);
      intraFontSetStyle(ifl[0],1.10f,BLACK,0,INTRAFONT_ALIGN_CENTER);
      intraFontPrintf(ifl[0],480/2,70,text.credits.thx);
      DRAWSUBMENU()
    break;
    case in_options:
      for (i=0; i!=NBRCHOICE; i++)
        {
        if (i==0) sscanf(getList().list[getNbrChoosed()],"%[^.].hsf",tmpbuffer);
        else if (i==1) sprintf(tmpbuffer,getThemeList().list[getThemeChoosed()]);

        intraFontSetStyle(ifl[1],0.75f,BLACK,(i==choicenbritem)?WHITE:0,INTRAFONT_ALIGN_RIGHT);
        intraFontPrint(ifl[1],480/2-30,START_Y+i*GAP_Y,text.options.choice[i]);
        intraFontSetStyle(ifl[1],0.75f,BLACK,(i==choicenbritem && testButton(PSP_CTRL_LEFT,0))?RED:0,INTRAFONT_ALIGN_LEFT);
        intraFontPrint(ifl[1],480/2-20,START_Y+i*GAP_Y,"<");
        intraFontSetStyle(ifl[1],0.75f,BLACK,0,INTRAFONT_ALIGN_LEFT);
        intraFontPrint(ifl[1],480/2,START_Y+i*GAP_Y,tmpbuffer);
        intraFontSetStyle(ifl[1],0.75f,BLACK,(i==choicenbritem && testButton(PSP_CTRL_RIGHT,0))?RED:0,INTRAFONT_ALIGN_LEFT);
        intraFontPrint(ifl[1],480/2+intraFontMeasureText(ifl[1],tmpbuffer)+10,START_Y+i*GAP_Y,">");
        }
      DRAWSUBMENU()
    break;
    }
  drawTopPlace();
  drawFade();

  finishDraw();
  }

#define ANGLE_GAP 5
int angle_info=0, place_choosed=0;
void drawTopPlace()
  {
  char tmpbuffer[16]={0};
  if (where!=in_menu) place_choosed=nbritem;
  if (where!=in_menu && angle_info<90) angle_info+=ANGLE_GAP;
  else if (where==in_menu && angle_info>0) angle_info-=ANGLE_GAP;
  if (angle_info!=0)
    {
    sprintf(tmpbuffer," > %s",text.menu.choice[place_choosed]);
    intraFontSetStyle(ifl[0],1.00f,RGBA(0,0,0,angle_info*255/90),0,INTRAFONT_ALIGN_LEFT);
    intraFontPrint(ifl[0],intraFontMeasureText(ifl[0],tmpbuffer)*sinf(angle_info*3.14159F/180)-intraFontMeasureText(ifl[0],tmpbuffer),12,tmpbuffer);
    }
  }

void changeNbrItem(char number)
  {
  nbritem += number;
  if      (nbritem==-1)  nbritem=NBR-1;
  else if (nbritem==NBR) nbritem=0;  
  }

void changeChoiceNbrItem(char number)
  {
  choicenbritem += number;
  if      (choicenbritem==-1)  choicenbritem=NBRCHOICE-1;
  else if (choicenbritem==NBRCHOICE) choicenbritem=0;  
  }

char getNbrItem()
  {
  return nbritem;
  }

char getChoiceNbrItem()
  {
  return choicenbritem;
  }

void goTo(char name)
  {
  where = name;
  }

char whereAreWe()
  {
  return where;
  }

void setStatus(char name)
  {
  status=name;
  if (status==quit) setFade(1,NULL);
  }
