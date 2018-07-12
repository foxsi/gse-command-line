
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

int fsercmd,ttyout,devicefile,actelvalue,cmdvalue,stripvalue;
unsigned char cmd[4];
struct termios sertty;
struct stat mystat;

void syntaxmsg()
{
	printf("Syntax is \n\n");
	printf("              /dev/tty.KeySerial1      (0 - 6)    Steven's numbers\n ");
	printf("./stripoff     serialinterface        detector        strip# \n\n");
}

main(argc,argv)
int  argc;
char *argv[ ];
{
	int status;

        if (argc < 2) 
        {
                printf("No serial file specified\n");
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
			syntaxmsg();
			exit(-1);
		}
        if( argc > 3) // parse value
        {
	        sscanf(argv[3],"%u",&stripvalue);
        }
        else
        {
			printf(" No command given \n");
			syntaxmsg();
			exit(-1);
        }
        if( stripvalue > 127)
        {
			printf (" Strip number greater than 127, too large %d \n",stripvalue);
			syntaxmsg();
			exit(-1);
		}
// parse strip value Steven's translation (63 -> 0, 0 -> 63) (127 -> 64, 64 -> 127)
		if(stripvalue < 64)
		{
			stripvalue = 63 - stripvalue;
			cmdvalue = 0;
		}
		else
		{	stripvalue = 191 - stripvalue; // 127 - strip# (Steven's translation) + 64 (Lindsay's encoding)
			cmdvalue = 0x40;
		}
		cmdvalue |= stripvalue;
		printf("Commanding ACTEL %u strip %u off , 0x%x \n",actelvalue,stripvalue,stripvalue);
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

