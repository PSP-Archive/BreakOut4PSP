#ifndef EDITOR_H
#define EDITOR_H

#include "common.h"

/*
* DEFINES, ENUMERATIONS
*/

#define Lnumber 5
#define Rnumber 8

enum { Echoosing, Edisplacing, Emodifying, Emoving, Eresizing };
enum { LOAD=1, DELETE=2 }; // Estate

// End (defines, enumerations)



/*
* FUNCTIONS
*/

// Controls
void do_editor_controls();

// Display
void do_editor_disp();
void drawTabs();
void defineTabsWidth();
char getAngle(char nbr);
void switchTabExtend();
void drawTabsText();
void setTabState(char state);
char getTabState();
char getExtendState();
void setEstate(char new);
void changeTabNbrItem(char tab, char number);
void doTabStuff(char tab);
void doExtendedTabStuff();
void drawBrickSelector();
void dispMvtBricks();
void drawAdditionalInfo();
void drawEditorInfo();
void switchGrid();
void drawGrid();

// Levels
int getChosedBrick();
char getNumberTypes();
char getState();
void setState(char state);
char getEditorLevelNumber();
void newLevel();
void loadEditorLevel(char lvlnbr);
void deleteLevel();
void saveLevel();
void createBrick();
void reInitBrick();
void deleteBrick();
void chooseBrick(char number);
void displaceBrick(char x, char y);
void modifyBrick(char number);
void moveBrick(char x, char y);
void resizeBrick(char x, char y);
void reInitControlsChanges();

// End (functions)

#endif
