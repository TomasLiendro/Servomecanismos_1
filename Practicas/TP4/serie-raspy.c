#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include<string.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//gcc serie-raspy.c -o serie-raspy
struct micro2raspi{
    uint8_t head;
    uint16_t adc0;
    uint16_t adc1;
    uint16_t encoder;
    uint16_t i2c;
    uint8_t chksum;

}__attribute__((__packed__));

struct raspi2micro{
    uint8_t head;
    uint16_t pwm;
    uint8_t cheksum;  
}__attribute__((__packed__)); 


int serial_open(char *serial_name, speed_t baud);
void USART0_initialize();




int main()
{
    struct raspi2micro tx_frame;
    struct micro2raspi rx_frame;
    char b;
    tx_frame.head = 0xAA;
    tx_frame.pwm =2000;
    tx_frame.cheksum = 0xAA;
    tx_frame.cheksum += (uint8_t)(tx_frame.pwm & 0x00ff) + (tx_frame.pwm>>8);
   
    
    int fd = serial_open("/dev/ttyUSB0",B115200);
    if (fd < 0)
    {
	

	return -1;
    }
    
    for(int i=0;i<10;i++)
    {
   
    write(fd,(void*)&tx_frame,sizeof(tx_frame));
    int n = read(fd,(void*)&rx_frame,sizeof(rx_frame));
    //bucle controlado {   }
     
     if(n != 10)
        {
         printf("n = %d\n", n); 
         //printf("%d", errno);
         return(0);
        }

     printf("%ux \t %ud\t encoder:%d\n", rx_frame.adc0, rx_frame.adc1, rx_frame.encoder);
     
     
    }
    close(fd);
}




int serial_open(char *serial_name, speed_t baud)
{
  struct termios newtermios;
  int fd;

  fd = open(serial_name,O_RDWR | O_NOCTTY); 
  if(fd<0)
  {
     fprintf(stderr, "No se pudo conectar por: %s (errno=%d)\n", strerror( errno ), errno );
      return fd;
  }

  newtermios.c_cflag= CBAUD | CS8 | CLOCAL | CREAD;
  newtermios.c_iflag=IGNPAR;
  newtermios.c_oflag=0;
  newtermios.c_lflag=0;
  newtermios.c_cc[VMIN]=1;
  newtermios.c_cc[VTIME]=0;

  cfsetospeed(&newtermios,baud);
  cfsetispeed(&newtermios,baud);
 
 if (tcflush(fd,TCIFLUSH)==-1) return -1;
 if (tcflush(fd,TCOFLUSH)==-1) return -1;
  if (tcsetattr(fd,TCSANOW,&newtermios)==-1) return -1;  

  return fd;
}
