#include <pspiofilemgr.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "common.h"

// Trois buffers pour la partie jeu, un pour l'éditeur.
Brick levels[4][MAXLEVELSIZE];
int levelsize[4] = {0};
char levelmodified[3] = {0};
char nol;

char* getModifiedP()
  {
  return levelmodified;
  }

void countNol()
  {
  nol=0;
  SceUID dir = sceIoDopen("./levels");
  SceIoDirent dirEnt;
  memset(&dirEnt, 0, sizeof(SceIoDirent));
  while(sceIoDread(dir, &dirEnt)>0) nol++;
  sceIoDclose(dir);

  char levelpath[32] = {0};
  FILE* tmp = NULL;
  sprintf(levelpath,"levels/level%d.hlf",nol);
  while(tmp == NULL)
    {
    nol--;
    sprintf(levelpath,"levels/level%d.hlf",nol);
    tmp = fopen(levelpath,"r");
    }
  fclose(tmp);
  }

char getNol()
  {
  return nol;
  }

void setNol(char number)
  {
  nol += number;
  }

void loadLevel(int targetnbr, char levelnbr)
  {
  char levelpath[64] = {0};
  int i=0;

  clearLevel(targetnbr);
  if (levelnbr>nol) levelnbr=1;
  else if (levelnbr<1) levelnbr=nol;

  sprintf(levelpath,"levels/level%d.hlf",levelnbr);
  FILE* levelfile = fopen(levelpath,"r");
  if (levelfile!=NULL)
    {
    while (fscanf(levelfile,"%d%d%d%d%d%d%d;", &levels[targetnbr][i].x_brick,
      &levels[targetnbr][i].y_brick, &levels[targetnbr][i].t_brick,
      &levels[targetnbr][i].xm_brick, &levels[targetnbr][i].ym_brick,
      &levels[targetnbr][i].xd_brick, &levels[targetnbr][i].yd_brick) != EOF)
      {
      levels[targetnbr][i].tx_brick = levels[targetnbr][i].x_brick;
      levels[targetnbr][i].ty_brick = levels[targetnbr][i].y_brick;
      i++;
      }
    levelsize[targetnbr] = i;
    fclose (levelfile);
    }
  }

void clearLevel(int levelnbr)
  {
  int i;
  for (i=0; i!=MAXLEVELSIZE; i++)
    memset(&levels[levelnbr][i],0,sizeof(levels[levelnbr][i]));
  levelsize[levelnbr]=0;
  }

Brick* getLevelPointer(int number)
  {
  return levels[number];
  }

char getLevelSize(int number)
  {
  return levelsize[number];
  }

void changeLevelSize(int buffernbr, char new)
  {
  levelsize[buffernbr]=new;
  }
