/*Authur Hamad Ullah
 * NetId hullah
 * Summer 2020
 * Date jul 6
 */

#define RETSIGTYPE void
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <pcap.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#ifndef setsignal_h
#define setsignal_h

RETSIGTYPE (*setsignal(int, RETSIGTYPE (*)(int)))(int);
#endif

char cpre580f98[] = "netdump";

void raw_print(u_char *user, const struct pcap_pkthdr *h, const u_char *p);

int packettype;

char *program_name;

int bits[4];

void calculateBits(u_int16_t hex);

int ICMP=0, TCP=0,IPv4=0,ARP=0,DNS=0,SMTP=0,POP=0,HTTP=0,IMAP=0;

/* SMTP decode function*/
void acii_conv(int start, int end, const u_char *p);


/* Externs */
extern void bpf_dump(const struct bpf_program *, int);

extern char *copy_argv(char **);

/* Forwards */
 void program_ending(int);

/* Length of saved portion of packet. */
int snaplen = 1500;;

static pcap_t *pd;

extern int optind;
extern int opterr;
extern char *optarg;
int pflag = 0, aflag = 0;


struct bits
{
	unsigned four:4;
	unsigned six:6;
};
int
main(int argc, char **argv)
{
	int cnt, op, i, done = 0;
	bpf_u_int32 localnet, netmask;
	char *cp, *cmdbuf, *device;
	struct bpf_program fcode;
	 void (*oldhandler)(int);
	u_char *pcap_userdata;
	char ebuf[PCAP_ERRBUF_SIZE];

	cnt = -1;
	device = NULL;
	
	if ((cp = strrchr(argv[0], '/')) != NULL)
		program_name = cp + 1;
	else
		program_name = argv[0];

	opterr = 0;
	while ((i = getopt(argc, argv, "pa")) != -1)
	{
		switch (i)
		{
		case 'p':
			pflag = 1;
		break;
		case 'a':
			aflag = 1;
		break;
		case '?':
		default:
			done = 1;
		break;
		}
		if (done) break;
	}
	if (argc > (optind)) cmdbuf = copy_argv(&argv[optind]);
		else cmdbuf = "";

	if (device == NULL) {
		device = pcap_lookupdev(ebuf);
		if (device == NULL)
			error("%s", ebuf);
	}
	pd = pcap_open_live(device, snaplen,  1, 1000, ebuf);
	if (pd == NULL)
		error("%s", ebuf);
	i = pcap_snapshot(pd);
	if (snaplen < i) {
		warning("snaplen raised from %d to %d", snaplen, i);
		snaplen = i;
	}
	if (pcap_lookupnet(device, &localnet, &netmask, ebuf) < 0) {
		localnet = 0;
		netmask = 0;
		warning("%s", ebuf);
	}
	/*
	 * Let user own process after socket has been opened.
	 */
	setuid(getuid());

	if (pcap_compile(pd, &fcode, cmdbuf, 1, netmask) < 0)
		error("%s", pcap_geterr(pd));
	
	(void)setsignal(SIGTERM, program_ending);
	(void)setsignal(SIGINT, program_ending);
	/* Cooperate with nohup(1) */
	if ((oldhandler = setsignal(SIGHUP, program_ending)) != SIG_DFL)
		(void)setsignal(SIGHUP, oldhandler);

	if (pcap_setfilter(pd, &fcode) < 0)
		error("%s", pcap_geterr(pd));
	pcap_userdata = 0;
	(void)fprintf(stderr, "%s: listening on %s\n", program_name, device);
	if (pcap_loop(pd, cnt, raw_print, pcap_userdata) < 0) {
		(void)fprintf(stderr, "%s: pcap_loop: %s\n",
		    program_name, pcap_geterr(pd));
		exit(1);
	}
	pcap_close(pd);
	exit(0);
}

/* routine is executed on exit */
void program_ending(int signo)
{
	struct pcap_stat stat;

	if (pd != NULL && pcap_file(pd) == NULL) {
		(void)fflush(stdout);
		putc('\n', stderr);
		if (pcap_stats(pd, &stat) < 0)
			(void)fprintf(stderr, "pcap_stats: %s\n",
			    pcap_geterr(pd));
		else {
			(void)fprintf(stderr, "%d packets received by filter\n",
			    stat.ps_recv);
			(void)fprintf(stderr, "%d packets dropped by kernel\n",
			    stat.ps_drop);
		}
	}
	printf("ICMP= %d,TCP= %d,IPv4= %d,ARP= %d,DNS= %d,SMTP= %d,POP=%d,HTTP=%d,IMAP=%d\n",ICMP,TCP,IPv4,ARP,DNS,SMTP,POP,HTTP,IMAP);
	exit(0);
}

/* Like default_print() but data need not be aligned */
void
default_print_unaligned(register const u_char *cp, register u_int length)
{
	register u_int i, s;
	register int nshorts;

	nshorts = (u_int) length / sizeof(u_short);
	i = 0;
	while (--nshorts >= 0) {
		if ((i++ % 8) == 0)
			(void)printf("\n\t\t\t");
		s = *cp++;
		(void)printf(" %02x%02x", s, *cp++);
	}
	if (length & 1) {
		if ((i % 8) == 0)
			(void)printf("\n\t\t\t");
		(void)printf(" %02x", *cp);
	}
}

/*
 * By default, print the packet out in hex.
 */
void
default_print(register const u_char *bp, register u_int length)
{
	register const u_short *sp;
	register u_int i;
	register int nshorts;

	if ((long)bp & 1) {
		default_print_unaligned(bp, length);
		return;
	}
	sp = (u_short *)bp;
	nshorts = (u_int) length / sizeof(u_short);
	i = 0;
	while (--nshorts >= 0) {
		if ((i++ % 8) == 0)
			(void)printf("\n\t");
		(void)printf(" %04x", ntohs(*sp++));
	}
	if (length & 1) {
		if ((i % 8) == 0)
			(void)printf("\n\t");
		(void)printf(" %02x", *(u_char *)sp);
	}
}

/*
insert your code in this routine

*/

void raw_print(u_char *user, const struct pcap_pkthdr *h, const u_char *p)
{
        u_int length = h->len;
        u_int caplen = h->caplen;
	u_int16_t e_type;
	struct bits version;
	e_type = p[12] *256 +p[13];
	u_int8_t flag = p[20]; 
	u_int8_t protocol = p[23];
        //default_print(p, caplen);
	putchar('\n');
	printf("==============Decode Ethernet Header===============\n");
	printf("Destination Address= %02X:%02X:%02X:%02X:%02X:%02X\n",p[0],p[1],p[2],p[3],p[4],p[5]);
	printf("Source Address= %02X:%02X:%02X:%02X:%02X:%02X\n",p[6],p[7],p[8],p[9],p[10],p[11]);
	printf("Type = 0x%02X%02X\n", p[12],p[13]);



	//IPV4 starts here 
	//
	//
	//
	//

	if(e_type==0x0800){
		IPv4 = IPv4+1;
		printf("payload =IPv4\n");
		printf("\t===============Decode IP Header============\n");
		int version=(p[14]/16);
		int headlen =(p[14]%16);
		printf("\tVersion number = %d\n", version);
		printf("\tHeader Length = %d\n",headlen*4);
		printf("\tType of Service = 0x%02X\n",p[15]);
		printf("\tTotal length = %d\n",p[16]*256 +p[17]);
		printf("\tID = %04X\n",p[18]*256+p[19]);
		flag = flag/16;
		if(flag == 0x04){
			printf("\tflag = 010\n");
		}
		printf("\tOffset = %d\n", ((p[20]%16)*256)+p[21]);
		printf("\tTTL = %d\n", p[22]);
		if(protocol == 0x01){
			printf("\tprotocol 1 -> ICMP\n");
			}
		else if (protocol == 0x06){
			printf("\tprotocol 6 -> TCP\n");
			}
		else if(protocol == 0x11){
			printf("\tprotocol 17 -> UDP\n");
			}
		printf("\tChecksum = %04X\n", p[24]*256+p[25]);
		printf("\tSource IP = %d.%d.%d.%d\n",p[26],p[27],p[28],p[29]);
		printf("\tDestination IP = %d.%d.%d.%d\n",p[30],p[31],p[32],p[33]);
		printf("\tOptions= ");
		for(int i=34; i<(((headlen*4)-20)+34);i++){
			printf("%d",p[i]);
		}
		printf("\n");
		int cont = ((headlen*4)-20)+34;
		//TCP Header starts here 
		//
		//
		//
	
		if(protocol == 0x06){
			TCP=TCP+1;
			printf("\t\t============= Decode TCP Header ============\n");
			printf("\t\tSource port = %d\n",p[cont]*256+p[cont+1]);
			printf("\t\tDst Port =%d\n",p[cont+2]*256+p[cont+3]);
			//DNS checking 
			if(p[cont+2]*256+p[cont+3]==0x0035){
				DNS=DNS+1;
			}
			printf("\t\tSequence number = 0x%8X\n",p[cont+4]*16777216+p[cont+5]*65536+p[cont+6]*256+p[cont+7]);
			printf("\t\tAcknowledgment number = 0x%2X%2X%2X%2X\n",p[cont+8],p[cont+9],p[cont+10],p[cont+11]);
			printf("\t\tHeader length=%d\n",(p[cont+12]/16)*4);
			int option_tcp =((p[cont+12]/16)*4)-20; 
			if((p[cont+12]%16)*256+(p[cont+13]/16) == 0x01){
				calculateBits(p[47]/16);
				printf("\t\tflag =01%d%d%d%d\n",bits[0],bits[1],bits[2],bits[3]);
			}
			else if((p[cont+12]%16)*256+(p[cont+13]/16) == 0x02){
				calculateBits(p[cont+13]/16);
				printf("\t\tflag = 10%d%d%d%d\n",bits[0],bits[1],bits[2],bits[3]);
			}
			else if((p[cont+12]%16)*256+(p[cont+13]/16) == 0x03){
				calculateBits(p[cont+13]/16);
				printf("\t\tflag = 11%d%d%d%d\n",bits[0],bits[1],bits[2],bits[3]);
			}
			else if((p[cont+12]%16)*256+(p[cont+13]/16) ==0x00){
				calculateBits(p[cont+13]/16);
				printf("\t\tflag =00%d%d%d%d\n",bits[0],bits[1],bits[2],bits[3]);
			}
			printf("\t\twidow size =0x%04X\n",p[cont+14]*256+p[cont+15]);

			printf("\t\tchecksum = 0x%04X\n",p[cont+16]*256+p[cont+17]);
			printf("\t\turgent pointer = 0x%04X\n",p[cont+18]*256+p[cont+19]);
			for(int i=54; i<option_tcp+54;i++){
				printf("\t\toptions flag %02X\n",p[i]);
			}
				//for(int i =length; i<caplen;i++){
				//	printf(" ");
				//	printf("\t\tData = 0x%02X",p[i]);
				//}
				
				length = 54+option_tcp;
				//Checking for the SMTP Packet
				if(p[cont]*256+p[cont+1] == 0x0019 || p[cont+2]*256+p[cont+3]==0x0019){
					SMTP+=1;	
					printf("\t\t\t=========Decode SMTP=======\n");
					acii_conv(length, caplen, p);
				}
				//HTTP Checking
				else if(p[cont]*256+p[cont+1] == 0x0050 || p[cont+2]*256+p[cont+3]==0x0050){
					HTTP+=1;
					printf("\t\t\t===========Decode HTTP=======\n");
					acii_conv(length, caplen, p);
				}
				else if(p[cont]*256+p[cont+1] == 0x006E || p[cont+2]*256+p[cont+3]==0x006E){
					POP+=1;
					printf("\t\t\t============ Decode POP=======\n");
					acii_conv(length, caplen, p);
				}
				//IMAP checking
				else if(p[cont]*256+p[cont+1] == 0x008F || p[cont+2]*256+p[cont+3]==0x008F || p[cont]*256+p[cont+1] == 0x03E1 || p[cont+2]*256+p[cont+3] ==0x03E1){
					IMAP+=1;
					printf("\t\t\t=========Decode IMAP======\n");
					acii_conv(length,caplen, p);
				}
				else{
					printf("\t\tDATA =");
					for(int i=length;i<caplen;i++){
						printf("%x ",p[i]);
					}
				}

			
		}

		//ICMP starts here
		//
		//
		//
		//
		else if(protocol ==  0x01){
			ICMP = ICMP+1;
			printf("\t\t============ Decode ICMP Header ============\n");
			printf("\t\tType = %d\n",p[cont]);
			printf("\t\tCode = %d\n",p[cont+1]);
			printf("\t\tchecksum = 0x%04X\n",p[cont+2]*256+p[cont+3]);
			if(p[cont]==0x00 || p[cont]==0x08 || p[cont]==0x14 || p[cont]==0x13){
				printf("ID = %04X\n",p[cont+4]*256+p[cont+5]);
				printf("Sequence number =%04X\n",p[cont+6]*256+p[cont+7]);
			}	
			if(p[34]==0x05){
				printf("IP Address = %d.%d.%d.%d\n",p[cont],p[cont+1],p[cont+2],p[cont+4]);
			}	
			printf("\t\tData =");
			for(int x=cont+7; x<caplen; x++){
				printf("%02X",p[x]); 
			}

		}
		//Checking for DNS 
		else { 
			printf("\t\t\tDATA = ");
			for(int i=cont; i<caplen; i++){
				printf("%x",p[i]);
			}
			if((p[cont]*256+p[cont+1])==0x0035 || (p[cont+2]*256 +p[cont+3])==0x0035){
				DNS=DNS+1;
			}
		}
	}
	//ARP starts here
	//
	//
	//	
	else if(e_type==0x0806){
		ARP =ARP+1;
		printf("payload = ARP\n");
		printf("\t================ Decode ARP header ========\n");  
		printf("\tHardware type = %d\n",p[14]*256+p[15]);
		printf("\tprotocol type = 0x%04X\n",p[16]*256+p[17]);
		printf("\tHardware length = %d Bytes\n", p[18]);
		printf("\tProtocol Length = %d Bytes\n", p[19]);
		printf("\tOperation = %d\n",p[20]*256+p[21]);
		if(p[14]*256+p[15] == 0x0001){
			printf("\tsender hardware address = %02X:%02X:%02X:%02X:%02X:%02X\n", p[22],p[23],p[24],p[25],p[26],p[27]);
		}
		if(p[19] == 0x04){
			printf("\tSender IP address = %d.%d.%d.%d\n",p[28],p[29],p[30],p[31]);
		}
		if(p[14]*256+p[15] == 0x0001){
			printf("\tTarget Hardware = %02X:%02X:%02X:%02X:%02X:%02X\n",p[32],p[33],p[34],p[35],p[36],p[37]);
		}
		if(p[19] ==0x04){
			printf("\tTarget IP = %d.%d.%d.%d\n",p[38],p[39],p[40],p[41]);
			printf("\t\t\tDATA= ");
			for(int i=42; i<caplen; i++){
				printf("%x",p[i]);
			}
		}
}
	else if (e_type==0x86dd){
		printf("payload = IPv6\n");
	}

	
        putchar('\n');
}

/*coverts hex into bits and save it in an array
 * declared as bits
 */

void calculateBits(u_int16_t hex){
	int value =8;
	for(int x=0;x<4;x++){
		if(hex>=value){
			hex=hex-value;
			bits[x] =1; 
		}
		else{
			bits[x]=0;
		}
		value = value/2;

	}
}


void acii_conv(int start, int end, const u_char *p){
	
	//printf("\t\t\tstart = %d\nend=%d\n ", start,end);

	for(int i=start;i<end;i++){
		printf("%c",p[i]);
	}
}

