/*
 * audio.cc
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
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "audio.h"

// sound playback type definition
#if defined(USE_ESD)
#define ESD
#else

#if defined(__linux__) && defined(USE_ALSA)
#define ALSA
#endif

#if (defined(__linux__) && !defined(ALSA)) || defined(__FreeBSD__)
#define OSS
#endif

#if defined(sun) || defined(__NetBSD__) || defined(__OpenBSD__)
#define SUN
#endif

#endif

// include header file
#ifdef ESD
#include <esd.h>
#else

#ifdef __linux__
#ifdef ALSA
#include <sys/asoundlib.h>
#else
#include <linux/soundcard.h>
#endif
#endif

#ifdef __FreeBSD__
#include <machine/soundcard.h>
#endif

#if defined(sun) && !defined(__SVR4)
#include <sun/audioio.h>
#endif

#if (defined(sun) && defined(__SVR4)) || defined(__NetBSD__) || defined(__OpenBSD__)
#include <sys/audioio.h>
#endif

#endif

// extra definition
#ifdef OSS
#ifndef SNDCTL_DSP_CHANNELS
#define SNDCTL_DSP_CHANNELS SOUND_PCM_WRITE_CHANNELS
#endif // !SNDCTL_DSP_CHANNELS
#endif // OSS

// return 0 if success
// return a positive number if file is illegal
// return a negative number if system error
int Audio::Play(char* file)
{
  return 0;
}

// get a word (4 bytes) of little endian
int Audio::getLWord(FILE* fp)
{
  unsigned char data[4];

  if (fread(data, 4, 1, fp) < 1)
    return -1;

  return data[0] + ((int)data[1] << 8) + ((int)data[2] << 16)
    + ((int)data[3] << 24);
}

// get a word (4 bytes) of big endian
int Audio::getBWord(FILE* fp)
{
  unsigned char data[4];

  if (fread(data, 4, 1, fp) < 1)
    return -1;

  return data[3] + ((int)data[2] << 8) + ((int)data[1] << 16)
    + ((int)data[0] << 24);
}

// skip a word (4 bytes)
int Audio::skipWord(FILE* fp)
{
  if (fseek(fp, 4L, SEEK_CUR) < 0)
    return -1;

  return 0;
}

// get a half word (2 bytes) of little endian
short Audio::getLHalf(FILE* fp)
{
  unsigned char data[2];

  if (fread(data, 2, 1, fp) < 1)
    return -1;

  return data[0] + ((short)data[1] << 8);
}

// get a half word (2 bytes) of big endian
short Audio::getBHalf(FILE* fp)
{
  unsigned char data[2];

  if (fread(data, 2, 1, fp) < 1)
    return -1;

  return data[1] + ((short)data[0] << 8);
}

// skip a half word (2 bytes)
int Audio::skipHalf(FILE* fp)
{
  if (fseek(fp, 2L, SEEK_CUR) < 0)
    return -1;

  return 0;
}

// open an audio device
int Audio::openDevice()
{
#ifdef ALSA
  if (snd_pcm_open(&m_audiopcm, 0, 0, SND_PCM_OPEN_PLAYBACK) < 0)
    return -1;

  memset(&m_audioformat, 0, sizeof(m_audioformat));
  return 1;
#elif defined(ESD)
  return 1;
#else
  m_audiofd = open(m_audiodev, O_WRONLY);
  if (m_audiofd < 0)
    return -1;
  
  return m_audiofd;
#endif
}

// close an audio device
int Audio::closeDevice()
{
#ifdef ALSA
  if (snd_pcm_close(m_audiopcm) < 0)
    return -1;
#elif defined(ESD)
  if (!(m_audiofd < 0))
    if (esd_close(m_audiofd) < 0)
      return -1;
#else
  if (close(m_audiofd) < 0)
    return -1;
#endif

  return 0;
}

// set the data format
int Audio::setFormat(int bits, int encoding)
{
#ifdef ALSA
  if (encoding != EN_LINEAR && encoding != EN_NONE)
    return -1;
  if (bits == 8)
    m_audioformat.format = SND_PCM_SFMT_U8;
  else if (bits == 16)
#ifdef SND_LITTLE_ENDIAN
    m_audioformat.format = SND_PCM_SFMT_S16_LE;
#else
    m_audioformat.format = SND_PCM_SFMT_S16_BE;
#endif
  else
    return -1;
#endif
	
#ifdef ESD
  if (encoding != EN_LINEAR && encoding != EN_NONE)
    return -1;
  m_audioformat = m_audioformat & ~ESD_MASK_BITS;
  if (bits == 8)
    m_audioformat |= ESD_BITS8;
  else if (bits == 16)
    m_audioformat |= ESD_BITS16;
  else
    return -1;
#endif

#ifdef OSS
  if (encoding != EN_LINEAR && encoding != EN_NONE)
    return -1;
  // If bits is 8, encoding is AFMT_U8
  // If bits is 16, encoding is AFMT_S16_BE
  if (ioctl(m_audiofd, SNDCTL_DSP_SETFMT, &bits) < 0)
    return -1;
#endif

#ifdef SUN
  struct audio_info_t ainfo;

  AUDIO_INITINFO(&ainfo);

  // select .wav file encoding
  if (encoding == EN_NONE || encoding == EN_LINEAR)
    encoding = bits == 8 ? AUDIO_ENCODING_LINEAR8 : AUDIO_ENCODING_LINEAR;

  ainfo.play.encoding = encoding;
  ainfo.play.precision = bits;

  if (ioctl(m_audiofd, AUDIO_SETINFO, &ainfo) < 0)
    return -1;
#endif

  return 0;
}

// set the number of channels; mono = 1, stereo = 2
int Audio::setChannels(int channels)
{
#ifdef ALSA
  m_audioformat.channels = channels;
#endif

#ifdef ESD
  m_audioformat = m_audioformat & ~ESD_MASK_CHAN;
  if (channels == 1)
    m_audioformat |= ESD_MONO;
  else
    m_audioformat |= ESD_STEREO;
#endif

#ifdef OSS
  if (ioctl(m_audiofd, SNDCTL_DSP_CHANNELS, &channels) < 0)
    return -1;
#endif

#ifdef SUN
  struct audio_info_t ainfo;

  AUDIO_INITINFO(&ainfo);
  ainfo.play.channels = channels;

  if (ioctl(m_audiofd, AUDIO_SETINFO, &ainfo) < 0)
    return -1;
#endif

  return 0;
}

// set a sampling rate per second
int Audio::setSamplingRate(int rate)
{
#ifdef ALSA
  m_audioformat.rate = rate;
#endif

#ifdef ESD
  m_audiorate = rate;
#endif

#ifdef OSS
  if (ioctl(m_audiofd, SNDCTL_DSP_SPEED, &rate) < 0)
    return -1;
#endif

#ifdef SUN
  struct audio_info_t ainfo;

  AUDIO_INITINFO(&ainfo);
  ainfo.play.sample_rate = rate;

  if (ioctl(m_audiofd, AUDIO_SETINFO, &ainfo) < 0)
    return -1;
#endif

  return 0;
}

// output audio data from the buffer
int Audio::output(char* buf, int size)
{
  int ret;

#ifdef ALSA
  // setup playback parameters only once
  if (m_audioformat.rate) {
    if (snd_pcm_playback_format(m_audiopcm, &m_audioformat) < 0)
      return -1;
	m_audioformat.rate = 0;
  }
#endif
  
#ifdef ESD
  // open connection only once
  if (m_audiofd < 0) {
    m_audiofd = esd_play_stream_fallback(m_audioformat | ESD_STREAM | ESD_PLAY,
					m_audiorate, NULL, "qvwm");
	if (m_audiofd < 0)
	  return -1;
  }
#endif

  while (size > 0) {
#ifdef ALSA
	ret = snd_pcm_write(m_audiopcm, buf, size);
#else
    ret = write(m_audiofd, buf, size);
#endif
    if (ret < 0)
      return -1;

    buf += ret;
    size -= ret;
  }

  return 0;
}

// output audio stream from the file
int Audio::outputStream(FILE* fp, int size)
{
  int bytes, ret;
  char buf[BSIZE];

  // output audio data
  while (size > 0) {
    if (size > BSIZE)
      bytes = BSIZE;
    else
      bytes = size;

    if (fread(buf, bytes, 1, fp) < 1)
      return AUDIO_BROKEN;

    if (output(buf, bytes) < 0) {
      ret = -errno;
#ifdef DEBUG
      perror("output");
#endif
      return ret;
    }

    size -= bytes;
  }

  return 0;
}
