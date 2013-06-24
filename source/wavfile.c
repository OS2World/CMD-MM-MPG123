/* Geez, why are WAV RIFF headers are so secret?  I got something together,
   but wow...  anyway, I hope someone will find this useful.
   - Samuel Audet <guardia@cam.org> */

#include <stdlib.h>
#include <stdio.h>
#include "mpg123.h"

struct
{
   char R1;
   char I2;
   char F3;
   char F4;
   unsigned long WAVElen;
   struct
   {
      char W1;
      char A2;
      char V3;
      char E4;
      char f1;
      char m2;
      char t3;
      char space;
      unsigned long fmtlen;
      struct
      {
         unsigned short FormatTag;
         unsigned short Channels;
         unsigned long SamplesPerSec;
         unsigned long AvgBytesPerSec;
         unsigned short BlockAlign;
         unsigned short BitsPerSample; /* format specific for PCM */
      } fmt;
      struct
      {
         char d1;
         char a2;
         char t3;
         char a4;
         unsigned long datalen;
         /* from here you insert your PCM data */
      } data;
   } WAVE;
} RIFF;

FILE *wavfp = NULL;

void wav_struct_init()
{
   RIFF.R1 = 'R';
   RIFF.I2 = 'I';
   RIFF.F3 = 'F';
   RIFF.F4 = 'F';
   RIFF.WAVElen = sizeof(RIFF.WAVE);
   RIFF.WAVE.W1 = 'W';
   RIFF.WAVE.A2 = 'A';
   RIFF.WAVE.V3 = 'V';
   RIFF.WAVE.E4 = 'E';
   RIFF.WAVE.f1 = 'f';
   RIFF.WAVE.m2 = 'm';
   RIFF.WAVE.t3 = 't';
   RIFF.WAVE.space = ' ';
   RIFF.WAVE.fmtlen = sizeof(RIFF.WAVE.fmt);
   RIFF.WAVE.fmt.FormatTag = 1; /* standard MS PCM, and its format specific
                                   is BitsPerSample */
   RIFF.WAVE.data.d1 = 'd';
   RIFF.WAVE.data.a2 = 'a';
   RIFF.WAVE.data.t3 = 't';
   RIFF.WAVE.data.a4 = 'a';
   RIFF.WAVE.data.datalen = 0; /* we don't know yet */
}

FILE *wav_open(struct audio_info_struct *ai, char *wavfilename)
{
   wav_struct_init();

   if(ai->format == AUDIO_FORMAT_SIGNED_16)
      RIFF.WAVE.fmt.BitsPerSample = 16;
   else if(ai->format == AUDIO_FORMAT_UNSIGNED_8)
      RIFF.WAVE.fmt.BitsPerSample = 8;
   else
   {
      fprintf(stderr,"Format not supported.");
      return 0;
   }

   if(ai->rate < 0) ai->rate = 44100;

   RIFF.WAVE.fmt.Channels = ai->channels;
   RIFF.WAVE.fmt.SamplesPerSec = ai->rate;
   RIFF.WAVE.fmt.AvgBytesPerSec =
         RIFF.WAVE.fmt.Channels *
         RIFF.WAVE.fmt.SamplesPerSec *
         RIFF.WAVE.fmt.BitsPerSample / 8;
   RIFF.WAVE.fmt.BlockAlign =
         RIFF.WAVE.fmt.Channels *
         RIFF.WAVE.fmt.BitsPerSample / 8;

   wavfp = fopen(wavfilename,"wb+");
   if(!wavfp)
      perror("wav");
   else
      fwrite(&RIFF, sizeof(RIFF),1,wavfp);

   return wavfp;
}

int wav_write(struct audio_info_struct *ai,unsigned char *buf,int len)
{
   int temp;
   if(!wavfp) return 0;

   temp = fwrite(buf, 1, len, wavfp);

   rewind(wavfp);

   RIFF.WAVE.data.datalen += temp;
   RIFF.WAVElen += temp;
   fwrite(&RIFF, sizeof(RIFF),1,wavfp);

   fseek(wavfp,0,SEEK_END);

   return temp;
}

int wav_close(struct audio_info_struct *ai)
{
   if(!wavfp) return 0;

   return fclose(wavfp);
}
