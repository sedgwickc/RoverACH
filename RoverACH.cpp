/*
 *
 */

#include "Rover_ACH.hpp"

Rover_ACH::Rover_ACH(){
	opt_msg_size = 256;
	opt_msg_cnt = 10;
	opt_chan_name = NULL;
	opt_pub = 0;
	opt_sub = 0;
	fin = NULL;
	fout = NULL;
}

int Rover_ACH::publish() {
	chan = this.chan;
    int r=0;
    while(1) {
        char *fr;
        /* get size */
        fr = fgets( pbuffer, (int)sizeof(pbuffer), fin );
        if( !fr ) break;
        assert( pbuffer == fr );
        /* put data */
        r = chan->put( pbuffer, strlen(pbuffer) );
        if( r != ACH_OK ) break;
    }
    chan->close( );
    return r;
}


int Rover_ACH::subscribe() {
	chan = this.chan;
    ach_status_t r;
    int t0 = 1;
    std::vector<uint8_t> buf;
    while(1) {
        size_t frame_size = 0;
        size_t fr;
        r  = chan->get ( &buf, 0, &frame_size, NULL, 0,
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
    chan->close( );
    return r;
}
