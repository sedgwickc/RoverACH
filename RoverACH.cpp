/*
 *
 */

#include "RoverACH.hpp"
#include <stdio.h>

using namespace rover;

RoverACH::RoverACH(){
	opt_msg_size = 256;
	opt_msg_cnt = 10;
	opt_pub = 0;
	opt_sub = 0;
	opt_buffer_type = TYPE_NAV_DATA;
	fin = NULL;
	fout = NULL;
	chnl = new ach::Channel();
}

int RoverACH::publish() {
    int r=0;
    //char *fr;
    /* get size */
    //fr = fgets( pbuffer, (int)sizeof(pbuffer), fin );
    //if( !fr ) break;
    //assert( pbuffer == fr );
    /* put data */
    if( opt_buffer_type == TYPE_NAV_DATA ){
        r = chnl->put( &curr_nav_data, NAVDATA_SIZE);
    } else if( opt_buffer_type == TYPE_STRING ){
        r = chnl->put( pbuffer, strlen(pbuffer) );
    }
    return r;
}

/*
 * gets a frames worth of data from channel and writes it fout.
 *
 */
int RoverACH::subscribe() {
    ach_status_t r;
    int t0 = 1;
    std::vector<uint8_t> buf;
    while(1) {
        size_t frame_size = 0;
        size_t fr;
        r  = chnl->get ( &buf, 0, &frame_size, NULL, 0,
                         ACH_MASK_OK | ACH_MASK_STALE_FRAMES, ACH_MASK_MISSED_FRAME );
        if( ACH_OK != r )  {
            if( ACH_STALE_FRAMES == r ) {
                usleep(1000);
            } else {
                if( ! (t0 && r == ACH_MISSED_FRAME) )
                    if( ACH_CLOSED != r ) {
                        fprintf(stderr, "sub: ach_error: %s\n",
                                ach_result_to_string(r));
                    }
                if( !  ( ACH_MISSED_FRAME == r ||
                         ACH_STALE_FRAMES == r ) ) break;
            }

        }

        fr = fwrite( &buf[0], sizeof(char), frame_size, fout );
        if ( fr != frame_size )  {
            r = ACH_OK;
            break;
        }
        fflush(fout);
    }
    /*fprintf(stderr,"end of subscribe\n");*/
    chnl->close( );
    return r;
}
