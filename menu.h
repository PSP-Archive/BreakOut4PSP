#ifndef MENU_H
#define MENU_H

/*
* DEFINES, ENUMERATIONS
*/

#define NBR 5
#define NBRCHOICE 2
#define START_Y 100
#define GAP_Y 28

enum { in_menu, in_options, in_credits };

// End (defines, enumerations)



/*
* FUNCTIONS
*/

// Controls
void do_menu_controls();

// Display
void do_menu_disp();
void doFadeStuff();
void drawTopPlace();
void changeNbrItem(char number);
void changeChoiceNbrItem(char number);
char getNbrItem();
char getChoiceNbrItem();
void goTo(char name);
char whereAreWe();
void setStatus(char name);

// End (functions)

#endif
