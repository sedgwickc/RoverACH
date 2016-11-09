/* Charles Sedgwick
 * 
 * ver  auth       date       comment
 * ---------------------------------------------------------------------------
 * 100  sedgwickc             Initial creation.
 * 101  sedgwickc  08nov2016  If buffer type set to nav_data then store recieved 
 *                            data in curr_nav_data struct.
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
	opt_buffer_type = TYPE_STRING;
	fin = NULL;
	fout = NULL;
	chnl = new ach::Channel();
}

int RoverACH::publish() {
    int r=0;
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
    /* declare temp buffer to store recieved data */
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

}

/*
 * gets a frame's worth of date and writes to the appropriate buffer
 * TRY: remove use of vector. Detect buffer type set and save recieved
 * data into appropriate buffer.
 *
 */
int RoverACH::getFrame() {
    ach_status_t r;
    /* declare temp buffer to store recieved data */
    std::vector<uint8_t> buf;
    size_t frame_size = 0;
    size_t fr;
    while(frame_size == 0){
    	r  = chnl->get ( &buf, 0, &frame_size, NULL, 0,
                     	 ACH_MASK_OK | ACH_MASK_STALE_FRAMES, ACH_MASK_MISSED_FRAME );
    	if( ACH_OK != r )  {
        	if( ACH_STALE_FRAMES == r ) {
            	usleep(1000);
        	} else {
            	if( ! ( r == ACH_MISSED_FRAME) ){
                	if( ACH_CLOSED != r ) {
                    	fprintf(stderr, "sub: ach_error: %s\n",
                            	ach_result_to_string(r));
                	}
            	}
        	}
    	}
    }
    switch( opt_buffer_type ){
        case TYPE_NAV_DATA:
             //memset(&curr_nav_data, 0, NAVDATA_SIZE);
             memcpy(&curr_nav_data, &buf[0], sizeof(curr_nav_data));
             printf("\nframe_size = %d, buffer size = %d,sizeof(curr_nav_data)=%d,",  
                 frame_size, sizeof(&buf[0]), sizeof(curr_nav_data));
             printf("NAVDATA_SIZE=%d\n", NAVDATA_SIZE);
             return r;
             break;
         case TYPE_STRING:
             memset(pbuffer, 0, PBUFF_SIZE);
             snprintf(pbuffer,PBUFF_SIZE-1,"%s", &buf[0]);
             return r;
             break;
         default:
             return r;
             break;
     }

   // chnl->close( );
}

