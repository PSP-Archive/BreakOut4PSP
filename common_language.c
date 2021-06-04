#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <pspiofilemgr.h>

#include "common.h"
#include "menu.h"
#include "editor.h"

Text text;
Matrix matrix;
FILE* speechfile = NULL;
int listnbr, nbrchoosed=0;

#define THX "\nThanks to Mateo21 for his C tutorials,\n\nto www.devsgen.com for help and support,\n\nparticularly to Spimort and AurelAlex :)\n\nUsing intraFont by BenHur.\n\n\nGeecko, 2010"

void parseString(char* name, char** pointer)
  {
  if (speechfile == NULL) return;
  char tmp[128];
  sprintf(tmp,"%s=<%%[^>]>\n",name);
  *pointer = realloc(*pointer,512*sizeof(char));
  fscanf(speechfile,tmp,*pointer);
  *pointer = realloc(*pointer,(strlen(*pointer)+1)*sizeof(char));
  }

void initLanguages()
  {
  memset(&text,0,sizeof(text));
  SceUID dir = sceIoDopen("languages");
  SceIoDirent dirEnt;
  memset(&dirEnt, 0, sizeof(SceIoDirent));
  listnbr=0;
  while(sceIoDread(dir, &dirEnt)>0)
    {
    if (strstr(dirEnt.d_name,".hsf")!=NULL && (strstr(dirEnt.d_name,".hsf~")==NULL && strstr(dirEnt.d_name,".bak")==NULL))
      {
      strcpy(matrix.list[listnbr],dirEnt.d_name);
      listnbr++;
      }
    }
  sceIoDclose(dir);

  char choosed[32];
  FILE* configfile = fopen("config.hcf","r");
  if (configfile==NULL) return;
  fscanf(configfile,"language=<%[^>]>\n",choosed);
  fclose(configfile);
  nbrchoosed=0;
  int i;
  for (i=0; i!=listnbr; i++) 
    {
    if (strstr(matrix.list[i],choosed)!=NULL) nbrchoosed=i;
    }
  }

void loadLanguage(char* name)
  {
  char tmpbuffer[32]={0};
  sprintf(tmpbuffer,"languages/%s",name);
  speechfile = fopen(tmpbuffer,"r");
  if (speechfile==NULL) return;

  text.credits.thx = THX;
  parseString("choice1",&text.menu.choice[0]);
  parseString("choice2",&text.menu.choice[1]);
  parseString("choice3",&text.menu.choice[2]);
  parseString("choice4",&text.menu.choice[3]);
  parseString("choice5",&text.menu.choice[4]);
  parseString("sub_back",&text.menu.sub_back);
  parseString("sub_ball",&text.game.sball);
  parseString("ball_left",&text.game.ballleft);
  parseString("balls_left",&text.game.ballsleft);
  parseString("title_lose",&text.game.tlose);
  parseString("sub_lose",&text.game.slose);
  parseString("sub_retry",&text.game.sretry);
  parseString("title_pause",&text.game.tpause);
  parseString("sub_pause",&text.game.spause);
  parseString("title_win",&text.game.twin);
  parseString("sub_win1",&text.game.swin1);
  parseString("sub_win2",&text.game.swin2);
  parseString("title_invalidlvl",&text.game.invalidlvl);
  parseString("Lchoice1",&text.editor.Lchoice[0]);
  parseString("Lchoice2",&text.editor.Lchoice[1]);
  parseString("Lchoice3",&text.editor.Lchoice[2]);
  parseString("Lchoice4",&text.editor.Lchoice[3]);
  parseString("Lchoice5",&text.editor.Lchoice[4]);
  parseString("Rchoice1",&text.editor.Rchoice[0]);
  parseString("Rchoice2",&text.editor.Rchoice[1]);
  parseString("Rchoice3",&text.editor.Rchoice[2]);
  parseString("Rchoice4",&text.editor.Rchoice[3]);
  parseString("Rchoice5",&text.editor.Rchoice[4]);
  parseString("Rchoice6",&text.editor.Rchoice[5]);
  parseString("Rchoice7",&text.editor.Rchoice[6]);
  parseString("Rchoice8",&text.editor.Rchoice[7]);
  parseString("yes",&text.editor.yes);
  parseString("no",&text.editor.no);
  parseString("Ochoice1",&text.options.choice[0]);
  parseString("Ochoice2",&text.options.choice[1]);

  fclose(speechfile);

  nbrchoosed=0;
  int i;
  for (i=0; i!=listnbr; i++) 
    {
    if (strstr(matrix.list[i],name)!=NULL) nbrchoosed=i;
    }
  defineTabsWidth();
  }

Matrix getList()
  {
  return matrix;
  }

int getListNbr()
  {
  return listnbr;
  }

int getNbrChoosed()
  {
  return nbrchoosed;
  }
