#include "game.h"
#include "menu.h"
#include "editor.h"

char game_initialized = 0,
     menu_initialized = 0,
     editor_initialized = 0;

/* Initialisation du jeu. */

void game_init()
  {
  initVars();
  game_levels_init();
  game_initialized = 1;
  }

void menu_init()
  {
  menu_initialized = 1;
  }

void editor_init()
  {
  defineTabsWidth();
  }

/* Mainloop */

void game_mainloop()
  {
  if (!game_initialized) game_init();

  do_game_controls();
  do_game_level();
  do_game_think();
  do_game_graphics();
  }

void menu_mainloop()
  {
  if (!menu_initialized) menu_init();
  do_menu_controls();
  do_menu_disp();
  }

void editor_mainloop()
  {
  if (!editor_initialized) editor_init();
  do_editor_controls();
  do_editor_disp();
  }

/* Destruction */

void game_destroy()
  {
  game_initialized = 0;
  }

void menu_destroy()
  {
  menu_initialized = 0;
  }

void editor_destroy()
  {
  editor_initialized = 0;
  }

void destroy_all()
  {
  game_destroy();
  menu_destroy();
  editor_destroy();
  }

