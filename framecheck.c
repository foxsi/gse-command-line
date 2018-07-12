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
unsigned int frame,frameold;

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
	while (read(framefd,(void *) telembuf,2048) == 2048)
	{
		bufptr = &telembuf[0];
		frames += 4;
		for(i = 0; i < 4; i++)
		{
			frameno = *(bufptr+5);
			framehi = *(bufptr+4);
			frameold = frame;
			frame = ( ( (unsigned int) framehi)<<16) & 0xffff0000;
			frame = frame | ( (unsigned int) frameno) & 0xffff;
			printf(" frame %08d (0x%08x)\r",frame,frame);
			if( frame != frameold +1){
				 printf("\nSequence problem , new frame = %d (0x%x), old frame = %d (0x%x)\n",
					frame,frame,frameold,frameold);
				frameerrs++;
			}
			for(j=0,chksum=0; j < 256; j++)
			{
				chksum ^= *(bufptr++);
			}
			if(chksum != 0)
			{
				printf("At frame %04x%04x chksum =  %04x\n",framehi,frameno,chksum);
				crcerrs++;
			}
		}
	}
	printf("  %d frame errors, %d crc errors with %d frames \n",frameerrs,crcerrs,frames);
}

