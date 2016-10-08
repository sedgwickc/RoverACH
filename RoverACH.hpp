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
#define NAVDATA_SIZE 320
#define NAME_SIZE 64

/* message type for publish method */
#define TYPE_NAV_DATA 0
#define TYPE_STRING 1


/* This structure is used to send navigation data */
typedef struct Navigation_Data{
    float gyro_x;
    float gyro_y;
    float gyro_z;
	float mag_x;
	float mag_y;
	float mag_z;
	float acc_x;
	float acc_y;
	float acc_z;
	float pressure;
	float temperature;
}Nav_Data;


class RoverACH{
public:
	//rover_ach_opts();

	RoverACH();

	int publish(int data_type);
	int subscribe();

	char pbuffer[PBUFF_SIZE];
	Nav_Data curr_nav_data;
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

} // rover namespace

/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/* Local Variables:                          */
/* mode: c++                                 */
/* indent-tabs-mode:  nil                    */
/* c-basic-offset: 4                         */
/* c-file-offsets: ((innamespace . 0))       */
/* End:                                      */
#endif
