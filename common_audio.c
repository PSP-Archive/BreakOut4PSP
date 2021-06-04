/*#include <pspkernel.h>
#include <AL/alut.h>
#include <ogg/ogg.h>
#include <tremor/ivorbisfile.h>
#include <malloc.h>
#include <string.h>

#include "common.h"

#define MAX_LOADED_SOUNDS 32
#define MAX_PLAYING_SOUNDS 2
#define BUFFER_SIZE 16384

typedef struct
  {
  char nameID[64];
  ALuint buffer;
  } Audio;

typedef struct
  {
  ALuint source;
  short ID;
  } Source;

Source sarray[MAX_PLAYING_SOUNDS];
Audio* array[MAX_LOADED_SOUNDS];

void playAudio(char* nameID)
  {
  ALint state;
  short i, i2;

  for (i2=0; i2!=MAX_LOADED_SOUNDS; i2++)
    {
    if (strstr(array[i2]->nameID,nameID) != NULL) break;
    }
  if (i2 == MAX_LOADED_SOUNDS) return;

  for (i=0; i!=MAX_PLAYING_SOUNDS; i++)
    {
    if (sarray[i].ID == -1) break;
    alGetSourcei(sarray[i].source,AL_SOURCE_STATE,&state);
    if (state == AL_STOPPED) break;
    }
  if (i == MAX_PLAYING_SOUNDS)
    {
    for (i=0; i!=MAX_PLAYING_SOUNDS; i++)
      {
      if (strstr(array[sarray[i].ID]->nameID,nameID) != NULL) break;
      }
    if (i == MAX_PLAYING_SOUNDS) return;
    else if (state == AL_PLAYING || state == AL_PAUSED)
      alSourceStop(sarray[i].source);
    }
  
  sarray[i].ID = i2;
  alSourcei(sarray[i].source,AL_BUFFER,array[i2]->buffer);
  alSourcePlay(sarray[i].source);
  }

void loadAudio(char* nameID, char* file)
  {
  FILE* f;
  OggVorbis_File oggFile;
  vorbis_info *pInfo;
  ALenum format;
  ALsizei freq;

  f = fopen(file,"rb");
  if (f == NULL) return;
  if (ov_open(f,&oggFile,NULL,0)) return;
  pInfo  = ov_info(&oggFile, -1);
  format = (pInfo->channels-1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
  freq = pInfo->rate;

  int i;
  for (i=0; i!=MAX_LOADED_SOUNDS; i++) if (array[i]==NULL) break;
  array[i] = malloc(sizeof(Audio));

  alGenBuffers(1,&array[i]->buffer);

  int size = 0;
  int section;
  int result = 0;
  char* data = NULL;

  do
    {
    data = realloc(data,(size+BUFFER_SIZE)*sizeof(char));
    result = ov_read(&oggFile,data+size,BUFFER_SIZE-size,&section);
    size += result;
    }
  while (result > 0);

  if (!size) return;
  alBufferData(array[i]->buffer,format,data,size,freq);
  free(data);

  strncpy(array[i]->nameID,nameID,63);
  array[i]->nameID[63] = '\0';
  }

void freeAudioArray()
  {
  int i;
  for (i=0; i!=MAX_LOADED_SOUNDS; i++)
    {
    if (array[i] != NULL)
      {
      free(array[i]);
      array[i] = NULL;
      }
    }
  }

void initAudio()
  {
  alutInit(NULL,0);
  alDopplerFactor(0);

  int i;
  for (i=0; i!=MAX_PLAYING_SOUNDS; i++)
    {
    sarray[i].ID = -1;
    alGenSources(1,&sarray[i].source);
    alSourceStop(sarray[i].source);
    }
  for (i=0; i!=MAX_LOADED_SOUNDS; i++) array[i] = NULL;

  loadAudio("boom","sound/brick_explosion.ogg");
  loadAudio("break","sound/brick_break.ogg");
  loadAudio("rebound","sound/rebound.ogg");
  }*/
