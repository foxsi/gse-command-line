#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>
int framefd,frameerrs,crcerrs,frames;
unsigned short int telembuf[1024],frameno,framehi,chksum,*bufptr;
unsigned int frame,frameold,nsroc1,nsroc2,nsroc3;

main(argc,argv)
int  argc;
char *argv[ ];
{
	int i,j;
	if( (framefd = open(argv[1],O_RDWR  | O_NDELAY)) < 0)
	{
        printf("Error opening file %s  (if disk file must exist)\n" ,argv[1]);
        exit(-1);
	}
	while (read(framefd,(void *) telembuf,518) == 518)
	{
		bufptr = &telembuf[0];
		frames += 1;
		
		{
			frameno = *(bufptr+8);
			framehi = *(bufptr+7);
			nsroc1 = *(bufptr +1);
			nsroc2 = *(bufptr + 2);
			nsroc3 = *(bufptr + 3);
			frameold = frame;
			frame = ( ( (unsigned int) framehi)<<16) & 0xffff0000;
			frame = frame | ( (unsigned int) frameno) & 0xffff;
			printf(" frame %08d (0x%08x), NSROC= 0x%04x 0x%04x 0x%04x\r",frame,frame,nsroc1,nsroc2,nsroc3);
			if( (frame != frameold +1) && (frameold != 0) ){
				 printf("\nSequence problem , new frame = %d (0x%x), old frame = %d (0x%x)\n",
					frame,frame,frameold,frameold);
				frameerrs++;
			}
			chksum = *bufptr; // sync2
			bufptr += 4; // advance past NSROC data
			for(j=0; j < 255; j++)
			{
				chksum ^= *(bufptr++);
			}
			if(chksum != 0)
			{
				printf("\nAt frame %04x%04x chksum =  %04x\n",framehi,frameno,chksum);
				crcerrs++;
			}
		}
	}
	printf("\n  %d frame errors, %d crc errors with %d frames \n",frameerrs,crcerrs,frames);
}

