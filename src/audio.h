/*
 * audio.h
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

#ifndef AUDIO_H_
#define AUDIO_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef USE_ALSA
#include <sys/asoundlib.h>
#endif

#ifdef USE_ESD
#include <esd.h>
#endif

#ifndef SEEK_CUR
#define SEEK_CUR 1
#endif

#define AUDIO_NOFILE	1	// File not found
#define AUDIO_BROKEN	2	// File is broken
#define AUDIO_BADFMT	3	// Bad format
#define AUDIO_UNSUPP	4	// Unsupported format

#define BSIZE 4096		// transfer size at a time

class Audio {
private:
  char* m_audiodev;
  int m_audiofd;
#ifdef USE_ALSA
  snd_pcm_t *m_audiopcm;
  snd_pcm_format_t m_audioformat;
#endif
#ifdef USE_ESD
  esd_format_t m_audioformat;
  int m_audiorate;
#endif

protected:
  enum { EN_NONE = 0, EN_ULAW, EN_ALAW, EN_LINEAR };

  int getLWord(FILE* fp);	// 4 bytes of little endian
  int getBWord(FILE* fp);	// 4 bytes of big endian
  int skipWord(FILE* fp);

  short getLHalf(FILE* fp);	// 2 bytes of little endian
  short getBHalf(FILE* fp);	// 2 bytes of big endian
  int skipHalf(FILE* fp);

protected:
  int openDevice();
  int closeDevice();
  int setFormat(int bits, int encoding = EN_NONE);
  int setChannels(int channels);
  int setSamplingRate(int rate);
  int output(char* buf, int size);
  int outputStream(FILE* fp, int size);

public:
  Audio(char* audiodev) : m_audiodev(audiodev), m_audiofd(-1) {}

  virtual int Play(char* file);
};

#endif // AUDIO_H_
