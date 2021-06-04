#include <math.h>
#include <pspctrl.h>
#include <pspgu.h>
#include <stdio.h>
#include <malloc.h>

#include "./librairies/graphics.h"
#include "./librairies/intraFont.h"

#include "common.h"
#include "editor.h"

extern intraFont* ifl[2];
extern Sprites sprites;
extern Text text;

#define ANGLEGAP 5
#define TABWIDTH_MIN 100
#define BORDERWIDTH 2
#define LTRIGO(var)\
  Ltabwidth*sinf(var*3.14159F/180)
#define RTRIGO(var)\
  Rtabwidth*sinf(var*3.14159F/180)
short Ltabwidth=0, Rtabwidth=0,
      angleL=0, angleEL=0, angleR=0, angleER=0,
      tabstate=0, extend=0, estate=0;
void drawTabs()
  {
  if (tabstate==0)
    {
    if (angleL-ANGLEGAP>=0) angleL-=ANGLEGAP;
    if (angleEL-ANGLEGAP>=0) angleEL-=ANGLEGAP;
    if (angleR-ANGLEGAP>=0) angleR-=ANGLEGAP;
    //if (angleER-ANGLEGAP>=0) angleER-=ANGLEGAP;
    }
  else if (tabstate==1)
    {
    if (angleL+ANGLEGAP<=90) angleL+=ANGLEGAP;
    if (angleEL+ANGLEGAP<=90 && extend) angleEL+=ANGLEGAP;
    if (angleEL-ANGLEGAP>=0 && !extend) angleEL-=ANGLEGAP;    
    if (angleR-ANGLEGAP>=0 ) angleR-=ANGLEGAP;
    //if (angleER-ANGLEGAP>=0) angleER-=ANGLEGAP;
    }
  else if (tabstate==2)
    {
    if (angleL-ANGLEGAP>=0 ) angleL-=ANGLEGAP;
    if (angleEL-ANGLEGAP>=0) angleEL-=ANGLEGAP;
    if (angleR+ANGLEGAP<=90) angleR+=ANGLEGAP;
    /*if (angleER+ANGLEGAP<=90 && extend) angleER+=ANGLEGAP;
    if (angleER-ANGLEGAP>=0 && !extend) angleER-=ANGLEGAP;*/
    }
  sceGuDisable(GU_DEPTH_TEST);
  if (angleL!=0)
    {
    fillScreenRect(LITEBLUE,0,0,LTRIGO(angleL)+LTRIGO(angleEL),272);
    fillScreenRect(TBLACK,LTRIGO(angleL)-BORDERWIDTH+1,0,BORDERWIDTH,272);
    if (angleEL!=0)
      {
      fillScreenRect(TBLACK,LTRIGO(angleL)+LTRIGO(angleEL)-BORDERWIDTH+1,0,BORDERWIDTH,272);
      }
    }
  if (angleR!=0)
    {
    /*if (angleER!=0)
      {
      fillScreenRect(LITEBLUE,480-TRIGO(angleR)-TRIGO(angleER),0,TRIGO(angleER)+1,272);
      fillScreenRect(TBLACK,480-TRIGO(angleR)-TRIGO(angleER),0,BORDERWIDTH,272);
      }*/
    fillScreenRect(LITEBLUE,480-RTRIGO(angleR),0,RTRIGO(angleR)+1,272);
    fillScreenRect(TBLACK,480-RTRIGO(angleR),0,BORDERWIDTH,272);
    }
  }

void defineTabsWidth()
  {
  short i, width=0;
  Ltabwidth = TABWIDTH_MIN;
  Rtabwidth = TABWIDTH_MIN;
  for (i=1; i!=Lnumber; i++)
    {
    width = 5*2+BORDERWIDTH+intraFontMeasureText(ifl[1],text.editor.Lchoice[i]);
    if (width > Ltabwidth) Ltabwidth = width;
    }
  for (i=1; i!=Rnumber; i++)
    {
    width = 5*3+BORDERWIDTH+intraFontMeasureText(ifl[1],text.editor.Rchoice[i]);
    if (width > Rtabwidth) Rtabwidth = width;
    }
  }

char getAngle(char nbr)
  {
  char tmp=0;
  if      (nbr==0) tmp=angleL;
  else if (nbr==1) tmp=angleR;
  return tmp; 
  }

void switchTabExtend()
  {
  extend=!extend;
  }

#define Y_GAP 18
#define TITLE_GAP 20
#define Y_START 17
#define Y_START_LIST 14
int Lnbritem=1, ELnbritem=0, Rnbritem=1, Dnbritem=0;
int listposition=0;
void drawTabsText()
  {
  char buffer[16]={0};
  int i;
  int y=Y_START;
  sprintf(buffer,"Level%d.hlf",getEditorLevelNumber());
  if (angleL!=0)
    {
    for (i=0; i!=Lnumber; i++)
      {
      intraFontSetStyle(ifl[1],(i==0)?1.00f:0.70f,BLACK,(Lnbritem==i && !extend)?(testButton(PSP_CTRL_CROSS,0))?LITERED:WHITE:0,(i==0)?INTRAFONT_ALIGN_CENTER:INTRAFONT_ALIGN_LEFT);
      intraFontPrint(ifl[1],LTRIGO(angleL)-((i==0)?Ltabwidth/2:Ltabwidth-5),y,text.editor.Lchoice[i]);
      y+=(i==0)?TITLE_GAP:Y_GAP;
      }
    intraFontSetStyle(ifl[1],0.70f,BLACK,0,INTRAFONT_ALIGN_CENTER);  
    intraFontPrint(ifl[1],LTRIGO(angleL)-Ltabwidth/2-BORDERWIDTH,267,buffer);
    }
  if (ELnbritem>=getNol()) ELnbritem=getNol()-1;
  y=Y_START_LIST;
  if (angleEL>50)
    {
    if (estate == LOAD)
      {
      for (i=0; i!=getNol(); i++)
        {
        if (i==ELnbritem)
          {
          if (y-listposition>=272) listposition+=6;
          if (y-listposition<=10) listposition-=6;
          }
        if (y-listposition>0 && y-listposition<280)
          {
          sprintf(buffer,"Level%d.hlf",i+1);
          intraFontSetStyle(ifl[1],0.70f,RGBA(0,0,0,(angleEL-50)*255/(90-50)),(ELnbritem==i)?(testButton(PSP_CTRL_CROSS,0))?LITERED:RGBA(255,255,255,(angleEL-50)*255/(90-50)):0, INTRAFONT_ALIGN_LEFT);
          intraFontPrint(ifl[1],LTRIGO(angleL)+LTRIGO(angleEL)-Ltabwidth+5,y-listposition,buffer);
          }
        y+=Y_GAP;
        }
      }
    else if (estate == DELETE)
      {
      sprintf(buffer,"%s ?",text.editor.Lchoice[Lnbritem]);
      intraFontSetStyle(ifl[1],0.75f,RGBA(0,0,0,(angleEL-50)*255/(90-50)),0,INTRAFONT_ALIGN_LEFT);
      intraFontPrint(ifl[1],LTRIGO(angleL)+LTRIGO(angleEL)-Ltabwidth+5,y,buffer);
      y+=Y_GAP*2;
      sprintf(buffer," %s",text.editor.no);
      intraFontSetStyle(ifl[1],0.70f,RGBA(0,0,0,(angleEL-50)*255/(90-50)),(Dnbritem==0)?(testButton(PSP_CTRL_CROSS,0))?LITERED:RGBA(255,255,255,(angleEL-50)*255/(90-50)):0,INTRAFONT_ALIGN_LEFT);
      intraFontPrint(ifl[1],LTRIGO(angleL)+LTRIGO(angleEL)-Ltabwidth+5,y,buffer);
      y+=Y_GAP;
      sprintf(buffer," %s",text.editor.yes);
      intraFontSetStyle(ifl[1],0.70f,RGBA(0,0,0,(angleEL-50)*255/(90-50)),(Dnbritem==1)?(testButton(PSP_CTRL_CROSS,0))?LITERED:RGBA(255,255,255,(angleEL-50)*255/(90-50)):0,INTRAFONT_ALIGN_LEFT);
      intraFontPrint(ifl[1],LTRIGO(angleL)+LTRIGO(angleEL)-Ltabwidth+5,y,buffer);
      }
    }
  y=Y_START;
  if (angleR!=0)
    {
    for (i=0; i!=Rnumber; i++)
      {
      intraFontSetStyle(ifl[1],(i==0)?1.00f:0.70f,BLACK,(Rnbritem==i)?(testButton(PSP_CTRL_CROSS,0))?LITERED:WHITE:0,(i==0)?INTRAFONT_ALIGN_CENTER:INTRAFONT_ALIGN_LEFT);
      intraFontPrint(ifl[1],SCREEN_WIDTH-RTRIGO(angleR)+((i==0)?Rtabwidth/2:5),y,text.editor.Rchoice[i]);
      if (i==getState()+3)
        {
        intraFontSetStyle(ifl[1],0.70f,BLACK,0,INTRAFONT_ALIGN_RIGHT);
        intraFontPrint(ifl[1],SCREEN_WIDTH-RTRIGO(angleR)+Rtabwidth-5,y,"<");
        }
      y += (i==0)?TITLE_GAP:Y_GAP*((i==2)?1.3f:1);
      }
    }
  }

void setTabState(char state)
  {
  tabstate=state;
  if (state!=0) reInitControlsChanges();
  }

char getTabState()
  {
  return tabstate;
  }

char getExtendState()
  {
  return extend;
  }

void setEstate(char new)
  {
  if (new==DELETE) Dnbritem=0;
  estate=new;
  }

void changeTabNbrItem(char tab, char number)
  {
  if (tab==0)
    {
    if (!extend)
      {
      Lnbritem += number;
      if      (Lnbritem==0) Lnbritem=Lnumber-1;
      else if (Lnbritem==Lnumber) Lnbritem=1;
      }
    else if (extend && angleEL==90)
      {
      if (estate == LOAD)
        {
        ELnbritem += number;
        if       (ELnbritem<0) ELnbritem=getNol()-1;
        else if (ELnbritem==getNol()) ELnbritem=0;
        }
      else if (estate == DELETE)
        {
        Dnbritem += number;
        if       (Dnbritem<0) Dnbritem=1;
        else if (Dnbritem>1) Dnbritem=0;
        }
      }
    }
  else if (tab==1)
    {
    Rnbritem += number;
    if      (Rnbritem==0)  Rnbritem=Rnumber-1;
    else if (Rnbritem==Rnumber) Rnbritem=1;
    }
  }

void doTabStuff(char tab)
  {
  if (tab==0)
    {
    if (!extend)
      {  
      if      (Lnbritem==1) newLevel();
      else if (Lnbritem==2) 
        {
        setEstate(LOAD);
        switchTabExtend();
        }
      else if (Lnbritem==3 && getEditorLevelNumber()<=getNol())
        {
        setEstate(DELETE);
        switchTabExtend();
        }
      else if (Lnbritem==4) saveLevel();
      }
    else 
      {
      if (estate == LOAD)
        {
        loadEditorLevel(ELnbritem+1);
        }
      if (estate == DELETE)
        {
        if      (Dnbritem==0); // Nothing to do, the tab will be switched
        else if (Dnbritem==1) deleteLevel();
        }
      switchTabExtend();
      }
    }
  else if (tab==1)
    {
    if      (Rnbritem==1) createBrick();
    else if (Rnbritem==2) deleteBrick();
    else setState(Rnbritem-3);
    }
  }

#define CHAIN_LENGTH 4
#define CHAIN_SPEED 0.5f
float chain_displace=0;
void drawBrickSelector()
  {
  Brick* lvl = getLevelPointer(3);
  unsigned int point_x=0, point_y=0,
               br, i, nbrvertices=0;
  Vertex* tmpvertices = NULL;
  nbrvertices=0;
  br = getChosedBrick();
  for (i=0; i!=2*lvl[br].xd_brick+2*lvl[br].yd_brick-4; i++)
    {
    if      (i<lvl[br].xd_brick-1) point_x++;
    else if (i<lvl[br].xd_brick+lvl[br].yd_brick-2) point_y++;
    else if (i<2*lvl[br].xd_brick+lvl[br].yd_brick-3) point_x--;
    else if (i<2*lvl[br].xd_brick+2*lvl[br].yd_brick) point_y--;
    if (floor(floor((i+chain_displace)/CHAIN_LENGTH)/2)==floor((i+chain_displace)/CHAIN_LENGTH)/2)
      {
      nbrvertices++;
      tmpvertices = realloc(tmpvertices,nbrvertices*sizeof(Vertex));
      tmpvertices[nbrvertices-1].x = lvl[br].x_brick+point_x;
      tmpvertices[nbrvertices-1].y = lvl[br].y_brick+point_y;
      tmpvertices[nbrvertices-1].z = 0;
      }
    }
  sceGuDisable(GU_TEXTURE_2D);
  sceGuDisable(GU_BLEND);
  sceGuDisable(GU_DEPTH_TEST);
  sceGuColor(RED);
  sceGuDrawArray(GU_POINTS,GU_VERTEX_16BIT|GU_TRANSFORM_2D,nbrvertices,0,tmpvertices);
  sceGuEnable(GU_TEXTURE_2D);
  sceGuEnable(GU_BLEND);
  free(tmpvertices);
  chain_displace += CHAIN_SPEED;
  if (chain_displace >= 2*CHAIN_LENGTH) chain_displace=0;
  }

  // Affichage des briques de mouvement (éditeur)
void dispMvtBricks()
  {
  Brick* lvl = getLevelPointer(3);
  int chosed = getChosedBrick();
  sceGuTexImage(0,getNextPower2(sprites.brick->imageWidth),
                  getNextPower2(sprites.brick->imageHeight),
                  sprites.brick->textureWidth,sprites.brick->data);
  tVertex* tVert = sceGuGetMemory(MAXLEVELSIZE*2*sizeof(ctVertex));
  tVert[0].x = lvl[chosed].x_brick+lvl[chosed].xm_brick;
  tVert[0].y = lvl[chosed].y_brick+lvl[chosed].ym_brick;
  tVert[0].z = getLevelSize(3);
  tVert[0].u = BRICK_DIRECT_U(chosed);
  tVert[0].v = 0;
  tVert[1].x = lvl[chosed].x_brick+lvl[chosed].xm_brick+lvl[chosed].xd_brick;
  tVert[1].y = lvl[chosed].y_brick+lvl[chosed].ym_brick+lvl[chosed].yd_brick;
  tVert[1].z = getLevelSize(3);
  tVert[1].u = BRICK_DIRECT_U(chosed)+BRICK_WIDTH;
  tVert[1].v = sprites.brick->imageHeight;
  tVert[2].x = lvl[chosed].x_brick-lvl[chosed].xm_brick;
  tVert[2].y = lvl[chosed].y_brick-lvl[chosed].ym_brick;
  tVert[2].z = getLevelSize(3);
  tVert[2].u = BRICK_DIRECT_U(chosed);
  tVert[2].v = 0;
  tVert[3].x = lvl[chosed].x_brick-lvl[chosed].xm_brick+lvl[chosed].xd_brick;
  tVert[3].y = lvl[chosed].y_brick-lvl[chosed].ym_brick+lvl[chosed].yd_brick;
  tVert[3].z = getLevelSize(3);
  tVert[3].u = BRICK_DIRECT_U(chosed)+BRICK_WIDTH;
  tVert[3].v = sprites.brick->imageHeight;
	sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
  sceGuEnable(GU_TEXTURE_2D);
  sceGuEnable(GU_BLEND);
  sceGuColor(0x7FFFFFFF);
  sceGuDrawArray(GU_SPRITES,GU_TEXTURE_16BIT|GU_VERTEX_16BIT|GU_TRANSFORM_2D,4,0,tVert);
  }

#define LEVEL lvl[tmpchosed]
#define FILLRECT(x,y,w,h)\
  fillScreenRect(WHITE,LEVEL.x_brick+x,LEVEL.y_brick+y,w,h);
#define SIDE\
  ((LEVEL.xd_brick>LEVEL.yd_brick)?LEVEL.yd_brick:LEVEL.xd_brick)
void drawAdditionalInfo()
  {
  if (!getLevelSize(3)) return;

  Brick* lvl = getLevelPointer(3);
  int tmpchosed = getChosedBrick();
 
  drawBrickSelector();
  if (getState() == Emoving) dispMvtBricks();
  drawEditorInfo();

  switch (getState())
    {
    case Emoving:
      FILLRECT(LEVEL.xm_brick,LEVEL.ym_brick,SIDE*1/5,1)
      FILLRECT(LEVEL.xd_brick+LEVEL.xm_brick-SIDE*1/5,LEVEL.ym_brick,SIDE*1/5,1)
      FILLRECT(LEVEL.xd_brick+LEVEL.xm_brick-1,LEVEL.ym_brick,1,SIDE*1/5)
      FILLRECT(LEVEL.xd_brick+LEVEL.xm_brick-1,LEVEL.yd_brick+LEVEL.ym_brick-SIDE*1/5,1,SIDE*1/5)
      FILLRECT(LEVEL.xd_brick+LEVEL.xm_brick-SIDE*1/5,LEVEL.yd_brick+LEVEL.ym_brick-1,SIDE*1/5,1)
      FILLRECT(LEVEL.xm_brick,LEVEL.yd_brick+LEVEL.ym_brick-1,SIDE*1/5,1)
      FILLRECT(LEVEL.xm_brick,LEVEL.yd_brick+LEVEL.ym_brick-SIDE*1/5,1,SIDE*1/5)
      FILLRECT(LEVEL.xm_brick,LEVEL.ym_brick,1,SIDE*1/5)
    break;
    case Edisplacing:
      FILLRECT(0,0,SIDE*1/5,1)
      FILLRECT(LEVEL.xd_brick-SIDE*1/5,0,SIDE*1/5,1)
      FILLRECT(LEVEL.xd_brick-1,0,1,SIDE*1/5)
      FILLRECT(LEVEL.xd_brick-1,LEVEL.yd_brick-SIDE*1/5,1,SIDE*1/5)
      FILLRECT(LEVEL.xd_brick-SIDE*1/5,LEVEL.yd_brick-1,SIDE*1/5,1)
      FILLRECT(0,LEVEL.yd_brick-1,SIDE*1/5,1)
      FILLRECT(0,LEVEL.yd_brick-SIDE*1/5,1,SIDE*1/5)
      FILLRECT(0,0,1,SIDE*1/5)
    break;
    case Eresizing:
      FILLRECT(LEVEL.xd_brick-1,LEVEL.yd_brick-SIDE*1/5,1,SIDE*1/5)
      FILLRECT(LEVEL.xd_brick-SIDE*1/5,LEVEL.yd_brick-1,SIDE*1/5,1)
    break;
    }
  }

void drawEditorInfo()
  {
  int type;
  char types[5] = {0,1,2,10,20};
  if (getLevelSize(3)!=0)
    {
    switch (getState())
      {
      case Echoosing:
        drawTopInfo(getChosedBrick()+1,getLevelSize(3),1);
      break;
      case Edisplacing:
        drawTopInfo(getLevelPointer(3)[getChosedBrick()].x_brick,getLevelPointer(3)[getChosedBrick()].y_brick,0);
      break;
      case Emodifying:
        for (type=0; type!=getNumberTypes(); type++)
          if (getLevelPointer(3)[getChosedBrick()].t_brick==types[type]) break;
        drawTopInfo(type+1,getNumberTypes(),1);
      break;
      case Emoving:
        drawTopInfo(getLevelPointer(3)[getChosedBrick()].xm_brick,getLevelPointer(3)[getChosedBrick()].ym_brick,0);
      break;
      case Eresizing:
        drawTopInfo(getLevelPointer(3)[getChosedBrick()].xd_brick,getLevelPointer(3)[getChosedBrick()].yd_brick,0);
      break;
      }
    }
  else drawTopInfo(0,0,0);
  intraFontRestoreDraw();
  }

char gridstate=0;
void switchGrid()
  {
  gridstate=!gridstate;
  }

void drawGrid()
  {
  int i;
  if (gridstate)
    {
    for (i=1; i!=11; i++) fillScreenRect(BLACK,0,20*i,480,1);
    for (i=1; i!=12; i++) fillScreenRect(BLACK,60*i,0,1,220);
    fillScreenRect(RED,0,20*(i-1),480,1);
    for (i=1; i!=12; i++) fillScreenRect(BLACK,60*i,252,1,20);
    fillScreenRect(RED,0,252,480,1);
    }
  }

void fixDisp()
  {
  Brick* lvl = getLevelPointer(3);
  lvl[getChosedBrick()].tx_brick = lvl[getChosedBrick()].x_brick;
  lvl[getChosedBrick()].ty_brick = lvl[getChosedBrick()].y_brick;
  }

void do_editor_disp()
  {
  startDraw();
  
  blitImagePartToScreen(0,0,480,272,sprites.background,0,0);
  fixDisp();
  sceGuEnable(GU_DEPTH_TEST);
  fastDrawEditorBricks();
  sceGuDisable(GU_DEPTH_TEST);
  drawAdditionalInfo();
  drawGrid();
  drawLR();
  drawTabs();
  drawTabsText();
  intraFontRestoreDraw();
  
  finishDraw();
  }
