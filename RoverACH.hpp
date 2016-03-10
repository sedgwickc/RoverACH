/*
 *
 */

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
#include "Ach.hpp"

class RoverACH{
public:
	//rover_ach_opts();

	RoverACH();

	int publish();
	int subscribe();

	char pbuffer[4096];
	ach::Channel* chan;

	/* options */
	int opt_msg_size;
	int opt_msg_cnt;
	char *opt_chan_name;
	int opt_pub;
	int opt_sub;

	FILE *fin;
	FILE *fout;
};

/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/* Local Variables:                          */
/* mode: c++                                 */
/* indent-tabs-mode:  nil                    */
/* c-basic-offset: 4                         */
/* c-file-offsets: ((innamespace . 0))       */
/* End:                                      */
