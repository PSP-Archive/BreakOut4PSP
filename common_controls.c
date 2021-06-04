#include <pspctrl.h>

SceCtrlData pad, oldPad;

char testButton(unsigned int button, char test)
  {
  char tmp=0;
  if      (test==0)  tmp = pad.Buttons & button && 1;                          // Si le bouton est appuyé
  else if  (test==1)  tmp = pad.Buttons & button && !(oldPad.Buttons & button);  // Si le bouton vient d'être appuyé
  else if (test==2)  tmp = !(pad.Buttons & button) && oldPad.Buttons & button;  // Si le bouton vient d'être relâché
  else if (test==3) tmp = pad.Buttons & button && oldPad.Buttons & button;    // Si le bouton est toujours appuyé  
  return tmp;
  }

void updateControls()
  {
  oldPad=pad;
  sceCtrlPeekBufferPositive(&pad,1);
  }

SceCtrlData* getPad()
  {
  return &pad;
  }
