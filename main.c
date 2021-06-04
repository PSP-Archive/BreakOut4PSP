 /*********************************************************************
 *                                                                    *
 *          BreakOut4PSP by Geecko, 2009                              *
 *                                                                    *
 *********************************************************************/

#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <string.h>

#include "main.h"
#include "common.h"
#include "mainloop.h"

#include "./librairies/graphics.h"

PSP_MODULE_INFO("BreakOut4PSP-stable",0,1,1);

  /* Prototype(s) */
void initEssential(); // Initialisation de base du jeu.

  /* Callbacks */
int exit_callback(int arg1, int arg2, void *common)
	{
	sceKernelExitGame();
  return 0;
	}
int CallbackThread(SceSize args, void *argp)
	{
	int cbid;
  cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
  sceKernelRegisterExitCallback(cbid);
  sceKernelSleepThreadCB();
  return 0;
  }
int SetupCallbacks()
  {
  int thid = 0;
  thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
  if(thid >= 0) sceKernelStartThread(thid, 0, 0);
  return thid;
  }

  /* Déclarations */
char gamestate=menu;

/*--------------------------------------------------------------*
 |                             Main                             |              
 *--------------------------------------------------------------*/ 

int main(int argc, char *argv[])
  {
  SetupCallbacks();
  initEssential();
  /* Mainloop */
  while (gamestate != quit)
    {
    switch (gamestate)
      {
      case menu:
        menu_mainloop();
      break;
      case play:
        game_mainloop();
      break;
      case editor:
        editor_mainloop();
      break;
      }
    }
  sceKernelExitGame();
  return 0; 
  }

void initEssential()
  {
  pspDebugScreenInit();

  /* Contrôles */
  sceCtrlSetSamplingCycle(0);
  sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

  /* Police de caractères */
  intraFont_init();

  /* Langage */
  initLanguages();
  loadLanguage(getList().list[getNbrChoosed()]);

  /* Niveaux */
  countNol();

  /* GU */
  common_disp_init();
  initGraphics();
  }

void setGameState(char state)
  {
  extern Sprites sprites;
  if (state!=quit) setFade(0,sprites.background);
  gamestate=state;
  }
