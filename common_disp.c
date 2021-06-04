#include <pspkernel.h>
#include <pspgu.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <pspiofilemgr.h>
#include <string.h>
#include <stdio.h>

#include "./librairies/intraFont.h"

#include "common.h"
#include "game.h"
#include "editor.h"

intraFont* ifl[2];
Sprites sprites;
Matrix themes;
int themenbr, themechoosed, loaded=0;

int getBrickType(int nbr)
  {
  char types[NBRTYPES] = {0,1,2,10,20};
  return types[nbr];
  }

void common_disp_init()
  {
  listThemes();
  loadTheme(themes.list[themechoosed]);
  }

void loadImg(Image** ptr, char* name, char* file)
  {
  if (loaded) freeImage(*ptr);
  char buffer[128];
  sprintf(buffer,"./graphics/%s/%s",name,file);
  *ptr = loadImage(buffer);
  } 

void loadTheme(char* name)
  {
  loadImg(&sprites.background,name,"background.png");
  loadImg(&sprites.racket,name,"racket.png");
  loadImg(&sprites.ball,name,"ball.png");
  loadImg(&sprites.brick,name,"brick.png");
  loadImg(&sprites.b_brick[0],name,"broken_brick_1.png");
  loadImg(&sprites.b_brick[1],name,"broken_brick_2.png");
  loadImg(&sprites.explosion,name,"explosion.png");
  loadImg(&sprites.geecko,name,"geecko.png");
  loadImg(&sprites.breakout4psp,name,"breakout4psp.png");
  loaded=1;

  themechoosed=0;
  int i;
  for (i=0; i!=themenbr; i++) 
    {
    if (strstr(themes.list[i],name)!=NULL) themechoosed=i;
    }
  }

void listThemes()
  {
  SceUID dir = sceIoDopen("graphics");
  SceIoDirent dirEnt;
  memset(&dirEnt, 0, sizeof(SceIoDirent));
  themenbr=0;
  while(sceIoDread(dir, &dirEnt)>0)
    {
    if (strstr(dirEnt.d_name,".")==NULL)
      {
      strcpy(themes.list[themenbr],dirEnt.d_name);
      themenbr++;
      }
    }
  sceIoDclose(dir);

  char choosed[32];
  FILE* configfile = fopen("config.hcf","r");
  if (configfile==NULL) return;
  fscanf(configfile,"%*s theme=<%[^>]>\n",choosed);
  fclose(configfile);
  themechoosed=0;
  int i;
  for (i=0; i!=themenbr; i++) 
    {
    if (strstr(themes.list[i],choosed)!=NULL) themechoosed=i;
    }
  }

Matrix getThemeList()
  {
  return themes;
  }

int getThemeNbr()
  {
  return themenbr;
  }

int getThemeChoosed()
  {
  return themechoosed;
  }

void intraFont_init()
  {
  intraFontInit();
  ifl[0] = intraFontLoad("flash0:/font/ltn8.pgf",INTRAFONT_CACHE_MED);
  ifl[1] = intraFontLoad("flash0:/font/ltn0.pgf",INTRAFONT_CACHE_MED);
  intraFontSetEncoding(ifl[0],INTRAFONT_STRING_UTF8);
  intraFontSetEncoding(ifl[1],INTRAFONT_STRING_UTF8);
  }

void startDraw()
  {
  sceKernelDcacheWritebackInvalidateAll();
  guStart();
  clearScreen(BLACK);
  sceGuColor(0xFFFFFFFF);
  }

void finishDraw()
  {
  sceGuFinish();
  sceGuSync(0,0);
  sceDisplayWaitVblankStart();
  flipScreen();
  }

void intraFontRestoreDraw() // Permet l'affichage correct des images
  {
  sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
  }

void drawLR()
  {
  intraFontSetStyle(ifl[0],1.0f,BLACK,(testButton(PSP_CTRL_LTRIGGER,0))?RED:0,INTRAFONT_ALIGN_LEFT);
  intraFontPrint(ifl[0],5,12,"<L");
  intraFontSetStyle(ifl[0],1.0f,BLACK,(testButton(PSP_CTRL_RTRIGGER,0))?RED:0,INTRAFONT_ALIGN_RIGHT);
  intraFontPrint(ifl[0],475,12,"R>");
  }

void drawTopInfo(int number1, int number2, char mode)
  {
  char info[12];
  sprintf(info,"%i/%i",number1,number2);
  intraFontSetStyle(ifl[0],1.0f,BLACK,WHITE,INTRAFONT_ALIGN_CENTER);
  intraFontPrint(ifl[0],480/2,12,info);
  int size = intraFontMeasureText(ifl[0],info);
  if (mode)
    {
    intraFontSetStyle(ifl[0],1.0f,BLACK,(testButton(PSP_CTRL_LEFT,0))?RED:0,INTRAFONT_ALIGN_RIGHT);
    intraFontPrint(ifl[0],480/2-size/2,12,"< ");
    intraFontSetStyle(ifl[0],1.0f,BLACK,(testButton(PSP_CTRL_RIGHT,0))?RED:0,INTRAFONT_ALIGN_LEFT);
    intraFontPrint(ifl[0],480/2+size/2,12," >");
    }
  }

#define FADESPEED 7
int fadevalue=255, fademode=0;
Image* fadepointer=NULL;
void drawFade()
  {
  if (!fademode)
    {
    fadevalue-=FADESPEED;
    if (fadevalue<0) fadevalue=0;
    }
  else if (fademode)
    {
    fadevalue+=FADESPEED;
    if (fadevalue>255) fadevalue=255;
    }
  if (fadevalue!=0)
    {
    if (fadepointer!=NULL)
      {
      sceGuTexFunc(GU_TFX_MODULATE,GU_TCC_RGBA);
      sceGuColor(RGBA(255,255,255,fadevalue));
      blitImagePartToScreen(0,0,480,272,fadepointer,0,0);
      }
    else fillScreenRect(RGBA(0,0,0,fadevalue),0,0,480,272);
    }
  }

void setFade(char mode, Image* pointer)
  {
  fademode = mode;
  if (!fademode && fadevalue!=255) fadevalue=255;
  if (fademode && fadevalue!=0)  fadevalue=0;
  fadepointer = pointer;
  }

unsigned char getFadeValue()
  {
  return fadevalue;
  }

int signcolor=0, signstate=1;
void fastDrawBricks(int x, char number, char mode)
  {
  Brick* lvl = getLevelPointer(number);
  int i, i2, nbr;
  tVertex* tVert;
  cVertex* cVert;
  // On dessine d'abord les briques
  sceGuTexImage(0,getNextPower2(sprites.brick->imageWidth),
                  getNextPower2(sprites.brick->imageHeight),
                  sprites.brick->textureWidth,sprites.brick->data);
  tVert = sceGuGetMemory(MAXLEVELSIZE*2*sizeof(tVertex));
  for (i=0, nbr=0; i!=getLevelSize(number); i++)
    {
    if ((mode || lvl[i].d_brick)
      && lvl[i].tx_brick+lvl[i].xd_brick+(!mode)*getLevelsPosition()+x >= 0
      && lvl[i].tx_brick+(!mode)*getLevelsPosition()+x <= 480
      && lvl[i].ty_brick+lvl[i].yd_brick>=0 && lvl[i].ty_brick <= 272)
      {
      tVert[nbr*2].x = lvl[i].tx_brick+(!mode)*getLevelsPosition()+x;
      tVert[nbr*2].y = lvl[i].ty_brick;
      tVert[nbr*2].z = (mode && i==getChosedBrick())?getLevelSize(number):i;
      tVert[nbr*2].u = BRICK_DIRECT_U(i);
      tVert[nbr*2].v = 0;
      tVert[nbr*2+1].x = lvl[i].tx_brick+(!mode)*getLevelsPosition()+lvl[i].xd_brick+x;
      tVert[nbr*2+1].y = lvl[i].ty_brick+lvl[i].yd_brick;
      tVert[nbr*2+1].z = (mode && i==getChosedBrick())?getLevelSize(number):i;
      tVert[nbr*2+1].u = BRICK_DIRECT_U(i)+BRICK_WIDTH;
      tVert[nbr*2+1].v = sprites.brick->imageHeight;
      nbr++;
      }
    }
  sceGuEnable(GU_TEXTURE_2D);
  sceGuEnable(GU_BLEND);
  sceGuDrawArray(GU_SPRITES,GU_TEXTURE_16BIT|GU_VERTEX_16BIT|GU_TRANSFORM_2D,2*nbr,0,tVert);

  // Au tour des bordures 
  cVert = sceGuGetMemory(MAXLEVELSIZE*8*sizeof(cVertex));;
  for (i=0, nbr=0; i!=getLevelSize(number); i++)
    {
    if ((mode || lvl[i].d_brick)
      && (lvl[i].tx_brick+lvl[i].xd_brick>=0 && lvl[i].tx_brick<=480)
      && (lvl[i].ty_brick+lvl[i].yd_brick>=0 && lvl[i].ty_brick<=272))
      {
      Color color = (mode && (lvl[i].xm_brick || lvl[i].ym_brick))
                    ?RGBA(signcolor,signcolor,signcolor,0x8F)
                    :TBLACK;
      cVert[nbr*8].x = lvl[i].tx_brick+(!mode)*getLevelsPosition()+x;
      cVert[nbr*8].y = lvl[i].ty_brick;
      cVert[nbr*8].z = (mode && i==getChosedBrick())?getLevelSize(number):i;
      cVert[nbr*8].color = color;
      cVert[nbr*8+1].x = lvl[i].tx_brick+(!mode)*getLevelsPosition()+lvl[i].xd_brick-1+x;
      cVert[nbr*8+1].y = lvl[i].ty_brick;
      cVert[nbr*8+1].z = (mode && i==getChosedBrick())?getLevelSize(number):i;
      cVert[nbr*8+1].color = color;
      cVert[nbr*8+2]=cVert[nbr*8+1];
      cVert[nbr*8+3].x = lvl[i].tx_brick+(!mode)*getLevelsPosition()+lvl[i].xd_brick-1+x;
      cVert[nbr*8+3].y = lvl[i].ty_brick+lvl[i].yd_brick-1;
      cVert[nbr*8+3].z = (mode && i==getChosedBrick())?getLevelSize(number):i;
      cVert[nbr*8+3].color = color;
      cVert[nbr*8+4]=cVert[nbr*8+3];
      cVert[nbr*8+4].x++;
      cVert[nbr*8+5].x = lvl[i].tx_brick+(!mode)*getLevelsPosition()+x;
      cVert[nbr*8+5].y = lvl[i].ty_brick+lvl[i].yd_brick-1;
      cVert[nbr*8+5].z = (mode && i==getChosedBrick())?getLevelSize(number):i;
      cVert[nbr*8+5].color = color;
      cVert[nbr*8+6]=cVert[nbr*8+5];
      cVert[nbr*8+7]=cVert[nbr*8];
      nbr++;
      }
    }
  signcolor += (signstate*2-1)*10;
  if (signcolor >= 255)
    {
    signcolor = 255;
    signstate = 0;
    }
  else if (signcolor <= 0)
    {
    signcolor = 0;
    signstate = 1;
    }
  sceGuDisable(GU_TEXTURE_2D);
  sceGuDrawArray(GU_LINES,GU_VERTEX_16BIT|GU_COLOR_8888|GU_TRANSFORM_2D,8*nbr,0,cVert);
  sceGuEnable(GU_TEXTURE_2D);
  // Au tour des cassures d'être dessinées
  if (!mode) // Si il n'est pas en mode éditeur
    {
    for (i=0; i!=2; i++)
      {
      Image* target = sprites.b_brick[i];
      tVert = sceGuGetMemory(MAXLEVELSIZE*2*sizeof(tVertex));;
      sceGuTexImage(0,getNextPower2(target->imageWidth),getNextPower2(target->imageHeight),target->textureWidth,target->data);
      for (i2=0, nbr=0; i2!=getLevelSize(number); i2++)
        {
        if (lvl[i2].d_brick
          && (lvl[i2].tx_brick+lvl[i2].xd_brick>=0 && lvl[i2].tx_brick<=480)
          && (lvl[i2].ty_brick+lvl[i2].yd_brick>=0 && lvl[i2].ty_brick<=272)
          && ((i==1 && lvl[i2].t_brick==1 && lvl[i2].d_brick==1)
          || (i==1 && lvl[i2].t_brick==2 && lvl[i2].d_brick==1)
          || (i==0 && lvl[i2].t_brick==2 && lvl[i2].d_brick==2)))
          {
          tVert[nbr*2].x = lvl[i2].tx_brick+x+getLevelsPosition();
          tVert[nbr*2].y = lvl[i2].ty_brick;
          tVert[nbr*2].z = i2;
          tVert[nbr*2].u = 0;
          tVert[nbr*2].v = 0;
          tVert[nbr*2+1].x = lvl[i2].tx_brick+x+getLevelsPosition()+lvl[i2].xd_brick;
          tVert[nbr*2+1].y = lvl[i2].ty_brick+lvl[i2].yd_brick;
          tVert[nbr*2+1].z = i2;
          tVert[nbr*2+1].u = target->imageWidth;
          tVert[nbr*2+1].v = target->imageHeight;
          nbr++;
          }
        }
      sceGuDrawArray(GU_SPRITES,GU_TEXTURE_16BIT|GU_VERTEX_16BIT|GU_TRANSFORM_2D,2*nbr,0,tVert);
      }
    }
  }
