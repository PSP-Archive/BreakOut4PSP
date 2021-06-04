#include <pspctrl.h>

#include "main.h"
#include "common.h"
#include "editor.h"

void do_editor_controls()
  {
  updateControls();

  if      (testButton(PSP_CTRL_LTRIGGER,0) && !testButton(PSP_CTRL_RTRIGGER,0)) setTabState(1);
  else if (testButton(PSP_CTRL_RTRIGGER,0) && !testButton(PSP_CTRL_LTRIGGER,0)) setTabState(2);
  else    setTabState(0);

  if (getTabState()==0 && (!getAngle(0) && !getAngle(1)))
    {
    if (testButton(PSP_CTRL_SELECT,1)) switchGrid();
    if (testButton(PSP_CTRL_SQUARE,1)) reInitBrick();
    if (testButton(PSP_CTRL_CIRCLE,2)) setGameState(menu);
    if (getLevelSize(3))
      {
      if (getState()==Echoosing)
        {
        if      (testButton(PSP_CTRL_LEFT,2)) chooseBrick(-1);
        else if (testButton(PSP_CTRL_RIGHT,2)) chooseBrick(+1);
        }
      else if (getState()==Edisplacing)
        {
        if      (testButton(PSP_CTRL_LEFT,0)) displaceBrick(-1,0);
        else if (testButton(PSP_CTRL_RIGHT,0)) displaceBrick(+1,0);
        if      (testButton(PSP_CTRL_UP,0)) displaceBrick(0,-1);
        else if (testButton(PSP_CTRL_DOWN,0)) displaceBrick(0,+1);
        }
      else if (getState()==Emodifying)
        {
        if      (testButton(PSP_CTRL_LEFT,2)) modifyBrick(-1);
        else if (testButton(PSP_CTRL_RIGHT,2)) modifyBrick(+1);
        }
      else if (getState()==Emoving)
        {
        if      (testButton(PSP_CTRL_LEFT,0)) moveBrick(-1,0);
        else if (testButton(PSP_CTRL_RIGHT,0)) moveBrick(+1,0);
        if      (testButton(PSP_CTRL_UP,0)) moveBrick(0,-1);
        else if (testButton(PSP_CTRL_DOWN,0)) moveBrick(0,+1);
        }
      else if (getState()==Eresizing)
        {
        if      (testButton(PSP_CTRL_LEFT,0)) resizeBrick(-1,0);
        else if (testButton(PSP_CTRL_RIGHT,0)) resizeBrick(+1,0);
        if      (testButton(PSP_CTRL_UP,0)) resizeBrick(0,-1);
        else if (testButton(PSP_CTRL_DOWN,0)) resizeBrick(0,+1);
        }
      }
    }
  else if (getTabState()==0) // Si les deux boutons sont relâchés, mais que les onglets sont toujours là
    {
    if (getAngle(0)>getAngle(1))
      {
      if (testButton(PSP_CTRL_CROSS,2)) doTabStuff(0);
      }
    else if (getAngle(1)>getAngle(0))
      {
      if (testButton(PSP_CTRL_CROSS,2)) doTabStuff(1);
      }
    }
  else if (getTabState()==1) // L
    {
    if      (testButton(PSP_CTRL_UP,1)) changeTabNbrItem(0,-1);
    else if (testButton(PSP_CTRL_DOWN,1)) changeTabNbrItem(0,+1);
    if      (testButton(PSP_CTRL_CROSS,2)) doTabStuff(0);
    if      (testButton(PSP_CTRL_CIRCLE,2) && getExtendState())
      {
      switchTabExtend();
      }
    }
  else if (getTabState()==2) // R
    {
    if      (testButton(PSP_CTRL_UP,1)) changeTabNbrItem(1,-1);
    else if (testButton(PSP_CTRL_DOWN,1)) changeTabNbrItem(1,+1);
    if      (testButton(PSP_CTRL_CROSS,2)) doTabStuff(1);
    }
  }
