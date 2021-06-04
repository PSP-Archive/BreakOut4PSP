TARGET = breakout
OBJS = main.o mainloop.o\
       common_controls.o common_disp.o common_level.o common_language.o common_audio.o\
       game_controls.o game_disp.o game_think.o game_level.o\
       menu_controls.o menu_disp.o\
       editor_controls.o editor_disp.o editor_level.o\
       ./librairies/libccc.o ./librairies/intraFont.o ./librairies/graphics.o ./librairies/framebuffer.o
LIBS = -lpng -lz -lpspgu -lpsprtc -lm -lpsprtc

CFLAGS = -O3 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = BreakOut4PSP v0.8
PSP_EBOOT_ICON = icon0.png

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak 
