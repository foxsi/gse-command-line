#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>

int frame;
unsigned short int telembuf[1024],frameno,framehi,chksum,*bufptr;

main(argc,argv)
int  argc;
char *argv[ ];
{
	int i,j;
	if( (frame = open(argv[1],O_RDWR  | O_NDELAY)) < 0)
	{
        printf("Error opening file %s  (if disk file must exist)\n" ,argv[1]);
        exit(-1);
	}
	while (read(frame,(void *) telembuf,2048) == 2048)
	{
		bufptr = &telembuf[0];
		for(i = 0; i < 4; i++)
		{
			frameno = *(bufptr+5);
			framehi = *(bufptr+4);
			for(j=0,chksum=0; j < 256; j++)
			{
				chksum ^= *(bufptr++);
			}
			if(chksum != 0)
			{
				printf("At frame %04x%04x chksum =  %04x\n",framehi,frameno,chksum);
			}
		}
	}
}

