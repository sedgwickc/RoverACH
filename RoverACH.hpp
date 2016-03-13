/*
 *
 */

#ifndef _ROVERACH_H_
#define _ROVERACH_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <inttypes.h>
#include <Ach.hpp>

namespace rover{

#define PBUFF_SIZE 4096
#define NAME_SIZE 64

class RoverACH{
public:
	//rover_ach_opts();

	RoverACH();

	int publish();
	int subscribe();

	char pbuffer[PBUFF_SIZE];
	ach::Channel* chnl;

	/* options */
	int opt_msg_size;
	int opt_msg_cnt;
	char opt_chan_name[NAME_SIZE];
	int opt_pub;
	int opt_sub;

	FILE *fin;
	FILE *fout;
};
}

/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/* Local Variables:                          */
/* mode: c++                                 */
/* indent-tabs-mode:  nil                    */
/* c-basic-offset: 4                         */
/* c-file-offsets: ((innamespace . 0))       */
/* End:                                      */
#endif
