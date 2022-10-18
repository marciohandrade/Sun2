
#ifndef _OGG_H_
#define _OGG_H_

#include "PreProcDefines.h"
#include <vorbis/vorbisfile.h>

//Setup the OGG stuff to use cfile
extern ov_callbacks cfile_callbacks;
#ifdef WIN32
extern ov_callbacks mmio_callbacks;
#endif

//Init the ogg system
int OGG_init();

//Similar to the stuff in mmreg.h
#define  OGG_FORMAT_VORBIS		0x3000	/* OGG Files */

#endif // _OGG_H_
