/*
 * audio_wav.cc
 *
 * Copyright (C) 1995-2000 Kenichi Kourai
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with qvwm; see the file COPYING.  If not, write to
 * the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <errno.h>
#include <stdio.h>
#include "audio_wav.h"

//
// Wave format:
//   "RIFF", len(4)
//   "WAVE"
//   "fmt ", len(4)
//   wFormatTag(2)        WAVE_FORMAT_PCM(0x0001), etc.
//   wChannels(2)         1: mono, 2: stereo
//   dwSamplesPerSec(4)   sampling rate
//   dwAvgBytesPerSec(4)
//   wBlockAlign(2)       
//   wBitsPerSample(2)    sampling bits per sample
//   ...
//   "data", len(4)
//   ...
//
int AudioWav::Play(char* file)
{
  FILE* fp;
  char id[4], format[8];
  WavHdr hdr;
  int len, ret;

  fp = fopen(file, "r");
  if (fp == NULL)
    return AUDIO_NOFILE;

  // check a magic
  if (fread(id, 4, 1, fp) < 1) {
    fclose(fp);
    return AUDIO_BROKEN;
  }
  if (memcmp(id, "RIFF", 4) != 0) {
    fclose(fp);
    return AUDIO_BADFMT;
  }

  // read a size; ignored
  skipWord(fp);
  
  // check a format
  if (fread(format, 8, 1, fp) < 1) {
    fclose(fp);
    return AUDIO_BROKEN;
  }
  if (memcmp(format, "WAVEfmt ", 8) != 0) {
    fclose(fp);
    return AUDIO_BADFMT;
  }

  // read a size
  len = getLWord(fp);
  if (len < 16) {
    fclose(fp);
    return AUDIO_BROKEN;
  }

  // read a format type
  hdr.m_wFormatTag = getLHalf(fp);
  if (hdr.m_wFormatTag < 0) {
    fclose(fp);
    return AUDIO_BROKEN;
  }

  // read stereo or mono
  hdr.m_wChannels = getLHalf(fp);
  if (hdr.m_wChannels < 0) {
    fclose(fp);
    return AUDIO_BROKEN;
  }
  else if (hdr.m_wChannels != 1 && hdr.m_wChannels != 2) {
    fclose(fp);
    return AUDIO_BADFMT;
  }

  // read a sampling rate
  hdr.m_dwSamplesPerSec = getLWord(fp);
  if (hdr.m_dwSamplesPerSec < 0) {
    fclose(fp);
    return AUDIO_BROKEN;
  }
  
  // read average bytes per second; ignored
  if (skipWord(fp) < 0) {
    fclose(fp);
    return AUDIO_BROKEN;
  }

  // read block alignment; ignored
  if (skipHalf(fp) < 0) {
    fclose(fp);
    return AUDIO_BROKEN;
  }

  // read sampling bits per sample
  hdr.m_wBitsPerSample = getLHalf(fp);
  if (hdr.m_wBitsPerSample < 0) {
    fclose(fp);
    return AUDIO_BROKEN;
  }

  if (fseek(fp, len - 16, SEEK_CUR) < 0) {
    fclose(fp);
    return AUDIO_BROKEN;
  }

#ifdef DEBUG
  printf("format type: %d\n", hdr.m_wFormatTag);
  printf("stereo: %d\n", (hdr.m_wChannels == 2) ? 1 : 0);
  printf("sampling rate: %d\n", hdr.m_dwSamplesPerSec);
  printf("sampling bits: %d\n", hdr.m_wBitsPerSample);
#endif

  // search a starting point of data area
  while (1) {
    if (fread(id, 4, 1, fp) < 1) {
      fclose(fp);
      return AUDIO_BROKEN;
    }
    len = getLWord(fp);
    if (len < 0) {
      fclose(fp);
      return AUDIO_BROKEN;
    }

    if (memcmp(id, "data", 4) == 0)
      break;
    else {
      if (fseek(fp, len, SEEK_CUR) < 0) {
	fclose(fp);
	return AUDIO_BROKEN;
      }
    }
  }
  
#ifdef DEBUG
  printf("data len: %d\n", len);
#endif

  // open an audio device
  if (openDevice() < 0) {
    ret = -errno;
#ifdef DEBUG
    perror("openDevice");
#endif
    fclose(fp);
    return ret;
  }

  // set the data format
  if (setFormat(hdr.m_wBitsPerSample) < 0) {
    ret = -errno;
#ifdef DEBUG
    perror("setFormat");
#endif
    fclose(fp);
    closeDevice();
    return ret;
  }

  // set the number of channels
  if (setChannels(hdr.m_wChannels) < 0) {
    ret = -errno;
#ifdef DEBUG
    perror("setChannels");
#endif
    fclose(fp);
    closeDevice();
    return ret;
  }

  // set a sampling rate
  if (setSamplingRate(hdr.m_dwSamplesPerSec) < 0) {
    ret = -errno;
#ifdef DEBUG
    perror("setSamplingRate");
#endif
    fclose(fp);
    closeDevice();
    return ret;
  }
  
  ret = outputStream(fp, len);

  fclose(fp);
  closeDevice();
  
  return ret;
}
