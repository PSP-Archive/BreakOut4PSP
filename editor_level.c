#include <pspiofilemgr.h>
#include <pspctrl.h>
#include <stdio.h>
#include <math.h>

#include "editor.h"
#include "common.h"
#include "game.h"

struct Old
  {
  int type;
  int xd;
  int yd;
  } last = {0,BRICK_WIDTH,BRICK_HEIGHT};

int  brickchosed=0;
char editor_levelnumber=0,
     editor_state=Echoosing;

int getChosedBrick()
  {
  return brickchosed;
  }

char getNumberTypes()
  {
  return NBRTYPES;
  }

char getState()
  {
  return editor_state;
  }

void setState(char state)
  {
  editor_state=state;
  }

char getEditorLevelNumber()
  {
  if (!editor_levelnumber) editor_levelnumber=getNol()+1;
  return editor_levelnumber;
  }

void newLevel()
  {
  clearLevel(3);
  editor_levelnumber=getNol()+1;
  last.xd=BRICK_WIDTH;
  last.yd=BRICK_HEIGHT;
  last.type=0;
  }

void loadEditorLevel(char lvlnbr)
  {
  loadLevel(3,lvlnbr);
  editor_levelnumber=lvlnbr;
  brickchosed=0;
  last.xd=BRICK_WIDTH;
  last.yd=BRICK_HEIGHT;
  last.type=0;
  }

void saveLevel()
  {
  if (getLevelSize(3)==0) return;
  char levelpath[32] = {0};
  int i;
  sprintf(levelpath,"levels/level%d.hlf",editor_levelnumber);
  FILE* levelfile = fopen(levelpath,"w");
  Brick* tmplevel = getLevelPointer(3);
  if (levelfile!=NULL)
    {
    for (i=0; i!=getLevelSize(3); i++)
      {
      fprintf(levelfile,"%d %d %d %d %d %d %d;\n",tmplevel[i].x_brick,tmplevel[i].y_brick,tmplevel[i].t_brick,tmplevel[i].xm_brick,tmplevel[i].ym_brick,tmplevel[i].xd_brick,tmplevel[i].yd_brick);
      }
    fclose(levelfile);
    }
  if (editor_levelnumber>getNol()) setNol(+1);
  if      (editor_levelnumber==getNol() && getLevelNumber()==1) getModifiedP()[0]=1;
  else if (editor_levelnumber==getLevelNumber()-1) getModifiedP()[0]=1;
  else if (editor_levelnumber==getLevelNumber()) getModifiedP()[1]=1;
  else if (editor_levelnumber==getLevelNumber()+1) getModifiedP()[2]=1;
  else if (editor_levelnumber==1 && getLevelNumber()==getNol()) getModifiedP()[2]=1;
  }

void deleteLevel()
  {
  if (!editor_levelnumber || editor_levelnumber>getNol()) return;
  char levelpath[24] = {0};
  sprintf(levelpath,"./levels/level%d.hlf",editor_levelnumber);
  remove(levelpath);
  int i=0;
  char oldname[24]={0}, newname[24]={0};
  if (editor_levelnumber!=getNol())
    {  
    while (i!=getNol()-editor_levelnumber)
      {
      sprintf(oldname,"./levels/level%d.hlf",editor_levelnumber+i+1);
      sprintf(newname,"./levels/level%d.hlf",editor_levelnumber+i);
      rename(oldname,newname);
      i++;
      }
    }
  setNol(-1);
  editor_levelnumber=getNol()+1;
  getModifiedP()[0]=2;
  getModifiedP()[1]=2;
  getModifiedP()[2]=2;
  }

void createBrick()
  {
  if (getLevelSize(3)>=MAXLEVELSIZE) return;
  Brick* tmplevel = getLevelPointer(3);
  int tmpsize = getLevelSize(3);
  tmplevel[tmpsize].x_brick=SCREEN_WIDTH/2-last.xd/2;
  tmplevel[tmpsize].y_brick=0;
  tmplevel[tmpsize].tx_brick=0;
  tmplevel[tmpsize].ty_brick=0;
  tmplevel[tmpsize].t_brick=last.type;
  tmplevel[tmpsize].d_brick=0;
  tmplevel[tmpsize].xm_brick=0;
  tmplevel[tmpsize].ym_brick=0;
  tmplevel[tmpsize].xd_brick=last.xd;
  tmplevel[tmpsize].yd_brick=last.yd;
  tmplevel[tmpsize].e_brick=0;
  brickchosed=tmpsize;
  changeLevelSize(3,getLevelSize(3)+1);
  }

void reInitBrick()
  {
  Brick* tmplevel = getLevelPointer(3);
  tmplevel[getChosedBrick()].xd_brick=BRICK_WIDTH;
  tmplevel[getChosedBrick()].yd_brick=BRICK_HEIGHT;
  last.xd=BRICK_WIDTH;
  last.yd=BRICK_HEIGHT;
  tmplevel[getChosedBrick()].xm_brick=0;
  tmplevel[getChosedBrick()].ym_brick=0;
  }

void deleteBrick()
  {
  if (getLevelSize(3)==0) return;
  int i;
  for (i=brickchosed+1; i!=getLevelSize(3); i++)
    {
    getLevelPointer(3)[i-1]=getLevelPointer(3)[i];
    }
  changeLevelSize(3,getLevelSize(3)-1);
  if (brickchosed>getLevelSize(3)-1) brickchosed=0;
  }

void chooseBrick(char number)
  {
  brickchosed+=number;
  if (brickchosed<0) brickchosed=getLevelSize(3)-1;
  else if (brickchosed>getLevelSize(3)-1) brickchosed=0;
  }

#define MAXDISPLACE 4
#define DISPLACEGAP 0.2
float displacex=0,displacey=0;
void displaceBrick(char x, char y)
  {
  if (x==1)
    {
    if      (testButton(PSP_CTRL_RIGHT,1)) { displacex=1; displacey=0; } 
    else if (testButton(PSP_CTRL_RIGHT,3) && displacex!=MAXDISPLACE) displacex+=DISPLACEGAP;
    }
  else if (x==-1)
    {
    if      (testButton(PSP_CTRL_LEFT,1)) { displacex=-1; displacey=0; } 
    else if (testButton(PSP_CTRL_LEFT,3) && displacex!=-MAXDISPLACE) displacex-=DISPLACEGAP;
    }
  if (y==1)
    {
    if      (testButton(PSP_CTRL_DOWN,1)) { displacex=0; displacey=1; }
    else if (testButton(PSP_CTRL_DOWN,3) && displacey!=MAXDISPLACE) displacey+=DISPLACEGAP;
    }
  else if (y==-1)
    {
    if      (testButton(PSP_CTRL_UP,1)) { displacex=0; displacey=-1; } 
    else if (testButton(PSP_CTRL_UP,3) && displacey!=-MAXDISPLACE) displacey-=DISPLACEGAP;
    }
  Brick* tmplevel = getLevelPointer(3);
  if (displacex == lrint(displacex) || fabs(displacex) >= 2) tmplevel[brickchosed].x_brick+=displacex;
  if (displacey == lrint(displacey) || fabs(displacey) >= 2) tmplevel[brickchosed].y_brick+=displacey;
  if      (tmplevel[brickchosed].x_brick<0)      tmplevel[brickchosed].x_brick=0;  
  else if (tmplevel[brickchosed].x_brick>SCREEN_WIDTH-tmplevel[brickchosed].xd_brick) tmplevel[brickchosed].x_brick=SCREEN_WIDTH-tmplevel[brickchosed].xd_brick;
  if      (tmplevel[brickchosed].y_brick<0)      tmplevel[brickchosed].y_brick=0;  
  else if (tmplevel[brickchosed].y_brick>SCREEN_HEIGHT-tmplevel[brickchosed].yd_brick) tmplevel[brickchosed].y_brick=SCREEN_HEIGHT-tmplevel[brickchosed].yd_brick;
  }

#define MAXMOVE 2
#define MOVEGAP 0.2
float movex=0, movey=0;
void moveBrick(char x, char y)
  {
  if (x==1)
    {
    if      (testButton(PSP_CTRL_RIGHT,1)) { movex=1; movey=0; } 
    else if (testButton(PSP_CTRL_RIGHT,3) && movex!=MAXMOVE) movex+=MOVEGAP;
    }
  else if (x==-1)
    {
    if      (testButton(PSP_CTRL_LEFT,1)) { movex=-1; movey=0; } 
    else if (testButton(PSP_CTRL_LEFT,3) && movex!=-MAXMOVE) movex-=MOVEGAP;
    }
  if (y==1)
    {
    if      (testButton(PSP_CTRL_DOWN,1)) { movex=0; movey=1; }
    else if (testButton(PSP_CTRL_DOWN,3) && movey!=MAXMOVE) movey+=MOVEGAP;
    }
  else if (y==-1)
    {
    if      (testButton(PSP_CTRL_UP,1)) { movex=0; movey=-1; } 
    else if (testButton(PSP_CTRL_UP,3) && movey!=-MAXMOVE) movey-=MOVEGAP;
    }
  Brick* tmplevel = getLevelPointer(3);
  if (movex == lrint(movex) || fabs(movex) >= 2) tmplevel[brickchosed].xm_brick+=movex;
  if (movey == lrint(movey) || fabs(movey) >= 2) tmplevel[brickchosed].ym_brick+=movey;
  if      (tmplevel[brickchosed].x_brick+tmplevel[brickchosed].xm_brick<0)
    tmplevel[brickchosed].xm_brick=-tmplevel[brickchosed].x_brick;  
  else if (tmplevel[brickchosed].x_brick+tmplevel[brickchosed].xm_brick>SCREEN_WIDTH-tmplevel[brickchosed].xd_brick)
    tmplevel[brickchosed].xm_brick=SCREEN_WIDTH-tmplevel[brickchosed].xd_brick-tmplevel[brickchosed].x_brick;
  if      (tmplevel[brickchosed].y_brick+tmplevel[brickchosed].ym_brick<0)
     tmplevel[brickchosed].ym_brick=-tmplevel[brickchosed].y_brick;  
  else if (tmplevel[brickchosed].y_brick+tmplevel[brickchosed].ym_brick>SCREEN_HEIGHT-tmplevel[brickchosed].yd_brick)
    tmplevel[brickchosed].ym_brick=SCREEN_HEIGHT-tmplevel[brickchosed].yd_brick-tmplevel[brickchosed].y_brick;
  }

void modifyBrick(char number)
  {
  int i;
  Brick* tmplevel = getLevelPointer(3);
  for (i=0; i!=NBRTYPES; i++) if (tmplevel[brickchosed].t_brick==getBrickType(i)) break;
  i+=number;
  if (i<0) i=NBRTYPES-1;
  else if (i>NBRTYPES-1) i=0;
  tmplevel[brickchosed].t_brick=getBrickType(i);
  last.type=getBrickType(i);
  }

#define MINSIZE 15
#define MAXSIZE 200
#define MAXSIZEMOVE 3
#define SIZEGAP 0.2
float sizex=0, sizey=0;
void resizeBrick(char x, char y)
  {
  if (x==1)
    {
    if      (testButton(PSP_CTRL_RIGHT,1)) { sizex=1; sizey=0; } 
    else if (testButton(PSP_CTRL_RIGHT,3) && sizex!=MAXSIZEMOVE) sizex+=SIZEGAP;
    }
  else if (x==-1)
    {
    if      (testButton(PSP_CTRL_LEFT,1)) { sizex=-1; sizey=0; } 
    else if (testButton(PSP_CTRL_LEFT,3) && sizex!=-MAXSIZEMOVE) sizex-=SIZEGAP;
    }
  if (y==1)
    {
    if      (testButton(PSP_CTRL_DOWN,1)) { sizex=0; sizey=1; }
    else if (testButton(PSP_CTRL_DOWN,3) && sizey!=MAXSIZEMOVE) sizey+=SIZEGAP;
    }
  else if (y==-1)
    {
    if      (testButton(PSP_CTRL_UP,1)) { sizex=0; sizey=-1; } 
    else if (testButton(PSP_CTRL_UP,3) && sizey!=-MAXSIZEMOVE) sizey-=SIZEGAP;
    }
  Brick* tmplevel = getLevelPointer(3);
  if (sizex == lrint(sizex) || fabs(sizex) >= 2) tmplevel[brickchosed].xd_brick+=sizex;
  if (sizey == lrint(sizey) || fabs(sizey) >= 2) tmplevel[brickchosed].yd_brick+=sizey;
  if      (tmplevel[brickchosed].xd_brick<MINSIZE)
    tmplevel[brickchosed].xd_brick=MINSIZE;  
  else if (tmplevel[brickchosed].x_brick+tmplevel[brickchosed].xd_brick>SCREEN_WIDTH)
    tmplevel[brickchosed].xd_brick=SCREEN_WIDTH-tmplevel[brickchosed].x_brick;
  else if (tmplevel[brickchosed].xd_brick>MAXSIZE)
    tmplevel[brickchosed].xd_brick=MAXSIZE;
  if      (tmplevel[brickchosed].yd_brick<MINSIZE)
     tmplevel[brickchosed].yd_brick=MINSIZE;  
  else if (tmplevel[brickchosed].y_brick+tmplevel[brickchosed].yd_brick>SCREEN_HEIGHT)
    tmplevel[brickchosed].yd_brick=SCREEN_HEIGHT-tmplevel[brickchosed].y_brick;
  else if (tmplevel[brickchosed].yd_brick>MAXSIZE)
     tmplevel[brickchosed].yd_brick=MAXSIZE;
  last.xd = tmplevel[brickchosed].xd_brick;
  last.yd = tmplevel[brickchosed].yd_brick;
  }

void reInitControlsChanges()
  {
  displacex=0;
  displacey=0;
  movex=0;
  movey=0;
  sizex=0;
  sizey=0;
  }
