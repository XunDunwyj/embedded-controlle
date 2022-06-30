#include <stdint.h>
#include <sys/io.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



void show_help(void)
{
	printf("show this help:\n");
	printf("  io -r addr\n");
	printf("  io -w addr value\n");
	printf("for example:\n");
	printf("  io -r 0x62\n");
	printf("  io -w 0x62 0x66\n");
}

static void init(int port)
{

	
	if ((ioperm(port, 1, 1)) != 0)
    	{
    		printf("ioperm(0x%02x, 1, 1))",port);
        	perror(":");
       		exit(1);
    	}	

	
}

int translat(char c)
{
	if(c<='9'&&c>='0') return c-'0';
	if(c>='a' && c<='f') return c-87;
	if(c>='A' && c<='F') return c-55;
	return -1;
}

// 字符串转16进制
int strtohex(char *str)
{
	int len = strlen(str);
	int i,n=0;
	int stat = 0;
	if(0 == len)
		return 0;
	
	for(i=0; i<len; i++)
	{
		stat = translat(str[i]);
		if(stat >= 0)
			n = n*16+stat;
	}
	return n;
}

static void wait_ec(const uint32_t port, const uint32_t flag, const char value)
{
    uint8_t data;
    int i;

    i = 0;
    data = inb(port);

    while ( (((data >> flag) & 0x1) != value) && (i++ < 100) )
    {
        usleep(1000);
        data = inb(port);
    }

    if (i >= 100)
    {
        fprintf(stderr, "wait_ec error on port 0x%x, data=0x%x, flag=0x%x, value=0x%x\n", port, data, flag, value);
        exit(1);
    }
}

static uint8_t read_ec(const uint32_t port)
{
    uint8_t value;
    wait_ec(port, 1, 0);
    value = inb(port);
    return value;
}


static void write_ec(const uint32_t port, const uint8_t value)
{      
    
    wait_ec(port, 1, 0);
    outb(value, port);
    wait_ec(port, 1, 0);
}

int main(int argc, char *argv[])
{
        
	int addr,value;
	if (argc > 1 && strcmp(argv[1], "-h") == 0)
	{
		show_help();
	}
	else if (argc > 1 && strcmp(argv[1], "-r") == 0)
	{ // read
		addr= strtohex(argv[2]);
		

		if(addr > 0xfff){
			perror("addr error!\n");
			return -1;
		}
		init(addr);
		usleep(1000);
		value=read_ec(addr);
		printf("0x%02x:%02x\n ",addr, value);
   	}
    	else if (argc > 2 && strcmp(argv[1], "-w") == 0)
	{ // read
		addr= strtohex(argv[2]);
		value= strtohex(argv[3]);

		if(addr > 0xfff){
			perror("addr error!\n");
			return -1;
		}
		init(addr);
		usleep(1000);
		printf("0x%02x:%02x\n ",addr, read_ec(addr));
		outb(value, addr);
		write_ec(addr, value);
		usleep(1000);
		printf("0x%02x:%02x\n ",addr, read_ec(addr));
   	}
       else
	{
		show_help();
	}
       
    
       
       return 0;
}
