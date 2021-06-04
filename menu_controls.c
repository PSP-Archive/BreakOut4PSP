#include <stdio.h>
#include <pspctrl.h>

#include "menu.h"
#include "main.h"
#include "common.h"

void writeConfigFile()
  {
  FILE* configfile = fopen("config.hcf","w");
  fprintf(configfile,"language=<%s>\n",getList().list[getNbrChoosed()]);
  fprintf(configfile,"theme=<%s>\n",getThemeList().list[getThemeChoosed()]);
  fclose(configfile);
  }

void do_menu_controls()
  {
  updateControls();

  if (whereAreWe()==in_menu)
    {
    if  (testButton(PSP_CTRL_CROSS,2))
      {
      switch (getNbrItem())
        {
        case 0:
          setGameState(play);
        break;
        case 1:
          setGameState(editor);
        break;
        case 2:
          goTo(in_options);
        break;
        case 3:
          goTo(in_credits);
        break;
        case 4:
          setStatus(quit);
        break;
        }
      }
    if      (testButton(PSP_CTRL_UP,1))   changeNbrItem(-1);
    else if (testButton(PSP_CTRL_DOWN,1)) changeNbrItem(+1);
    }
  else if (whereAreWe()==in_options)
    {
    if  (testButton(PSP_CTRL_RIGHT,2))
      {
      switch (getChoiceNbrItem())
        {
        case 0:
          loadLanguage(getList().list[(getNbrChoosed()+1<getListNbr())?getNbrChoosed()+1:0]);
        break;
        case 1:
          loadTheme(getThemeList().list[(getThemeChoosed()+1<getThemeNbr())?getThemeChoosed()+1:0]);
        break;
        }
      }
    else if (testButton(PSP_CTRL_LEFT,2))
      {
      switch (getChoiceNbrItem())
        {
        case 0:
          loadLanguage(getList().list[(getNbrChoosed()-1>=0)?getNbrChoosed()-1:getListNbr()-1]);
        break;
        case 1:
          loadTheme(getThemeList().list[(getThemeChoosed()-1>=0)?getThemeChoosed()-1:getThemeNbr()-1]);
        break;
        }
      }
    if      (testButton(PSP_CTRL_UP,1))   changeChoiceNbrItem(-1);
    else if (testButton(PSP_CTRL_DOWN,1)) changeChoiceNbrItem(+1);
    if      (testButton(PSP_CTRL_CIRCLE,2)) 
      {
      writeConfigFile();
      goTo(in_menu);
      }
    }
  else if (testButton(PSP_CTRL_CIRCLE,2)) goTo(in_menu);
  }
