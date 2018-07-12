
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <termios.h>
#include <signal.h>
#include <sys/ioctl.h>

int fsercmd,ttyout,devicefile,actelvalue,cmdvalue,threshvalue,asicvalue;
unsigned char cmd[4];
struct termios sertty;
struct stat mystat;

void syntaxmsg()
{
	printf("Syntax is \n\n");
	printf("              /dev/tty.KeySerial1      (0 - 6)   (0-1)    (value)\n ");
	printf("./setthresh    serialinterface        detector   asic     threshold \n\n");
}


main(argc,argv)
int  argc;
char *argv[ ];
{
	int status;

        if (argc < 2) 
        {
                printf("No file specified\n");
				syntaxmsg();
                exit(-1);
        }
        if( (fsercmd = open(argv[1],O_RDWR | O_NOCTTY | O_NDELAY)) < 0)
        {
                printf("Error opening file %s  (if disk file must exist)\n" ,argv[1]);
				syntaxmsg();
                exit(-1);
        }
        fstat(fsercmd,&mystat);
        if( S_IFCHR & mystat.st_mode )
        {
                devicefile = 1;
                tcgetattr(fsercmd,&sertty); /* get serial line properties */
                sertty.c_iflag = IGNBRK;
                sertty.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG); /* raw input */
                sertty.c_oflag &= ~OPOST;
                cfsetospeed(&sertty,B1200);
                cfsetispeed(&sertty,B1200);
                sertty.c_cflag = (sertty.c_cflag & ~CSIZE) | CS8; /* 8 bits */
                sertty.c_cflag &= ~( CRTSCTS | PARENB | PARODD | CSTOPB); /*no CTS, no parity, 1 stop */
                sertty.c_cflag |= (CLOCAL | CREAD | CSTOPB); /* ok, 2 stop for safety */
                tcsetattr(fsercmd,TCSANOW,&sertty);
//      printf(" %d  %x \n", devicefile, mystat.st_mode);
				ioctl(fsercmd, TIOCMGET, &status);
			    status |= TIOCM_LE;
			    status |= TIOCM_DTR;
			    ioctl(fsercmd, TIOCMSET, &status);
        }
//      fstat(fileno(stdout),&mystat);
//      if((S_IFREG & mystat.st_mode) == 0) ttyout = 1;
        ttyout = isatty(fileno(stdout));
        if( argc > 2) // parse value
        {
	        sscanf(argv[2],"%u",&actelvalue);
        }
        else
        {
			printf(" No actel given \n");
			syntaxmsg();
			exit(-1);
        }
        if( actelvalue > 6)
        {
			printf (" ACTEL greater than 6, too large %d \n",actelvalue);
			exit(-1);
		}
        if( argc > 3) // parse value
        {
	        sscanf(argv[3],"%u",&asicvalue);
        }
        else
        {
			printf(" No asic given \n");
			syntaxmsg();
			exit(-1);
        }
        if( asicvalue > 1)
        {
			printf (" ASIC number not 0 or 1 %d \n",asicvalue);
			syntaxmsg();
			exit(-1);
		}
        if( argc > 4) // parse value
        {
	        sscanf(argv[4],"%u",&threshvalue);
        }
        else
        {
			printf(" No threshold given \n");
			syntaxmsg();
			exit(-1);
        }
        if( threshvalue > 31)
        {
			printf (" Threshold greater than 31, too large %d \n",threshvalue);
			syntaxmsg();
			exit(-1);
		}
		if(asicvalue == 0)
		{
			cmdvalue = 0x80;
		}
		else
		{
			cmdvalue = 0xC0;
		}
		cmdvalue |= threshvalue;
		printf("Commanding ACTEL %u ,ASIC %u threshold to %u  , 0x%x \n",actelvalue,asicvalue,threshvalue,threshvalue);
		cmd[0] = 0xc0;
		cmd[1] = 0;
		cmd[1] |= actelvalue;
		cmd[2] = (unsigned char) (cmdvalue &0xff);
		cmd[3] = 0x0;
		cmd[3] ^= cmd[0];
		cmd[3] ^= cmd[1];
		cmd[3] ^= cmd[2];
		printf("Sending bytes %02x %02x %02x %02x \n",cmd[0],cmd[1],cmd[2],cmd[3]);
		if(devicefile != 1) printf("Writing to file, not serial port\n");
		write(fsercmd,&cmd[0],4);
		sleep(1);

}

