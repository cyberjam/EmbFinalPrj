/* Jae Min Kim 20130642 
 *
 * I'm Server 
 *
 * Writing Date
 * 2018.11.29
 * 2018.12.07 uts complete!
 * 2018.12.08 info complete! -- TODO: may be need to function
 * 2018.12.09 conf part
 * 2018.12.12 commit
 * 2018.12.13 printf
 * 2018.12.19 printf2
 */


#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<string.h>

#include<sys/utsname.h> // for uts
#include<sys/sysinfo.h> // for info

#include<unistd.h> //for conf

//5. define app protocol
#define CT_ADD 0x01  //cmd type = CT, value :16
//-------------------uts structure-----------------
#define CT_OSNAME 0x02
#define CT_NODENAME 0x03
#define CT_RELEASE 0x04
#define CT_VERSION 0x05
#define CT_MACHINE 0x06
//-------------------info structure----------------
#define CT_UPTIME 0x10
#define CT_LOADS_1 0x11
#define CT_LOADS_5 0x12
#define CT_LOADS_15 0x13
#define CT_TOTALRAM 0x14
#define CT_FREERAM 0x15
#define CT_SHAREDRAM 0x16
#define CT_BUFFERRAM 0x17
#define CT_TOTALSWAP 0X18
#define CT_FREESWAP 0X19
#define CT_PROCS 0X20
#define CT_BLANK 0x21 // blank
//------------------conf structure---------------------
#define CT_ARG 0x30 
#define CT_CHILD 0x31 
#define CT_HOSTNAME 0x32 
#define CT_LOGINNAME 0x33 
#define CT_NGROUPS 0x34 
#define CT_CLKTCK 0x35 
#define CT_OPEN 0x36 
#define CT_PAGESIZE 0x37 
#define CT_REDUP 0x38 
#define CT_STREAM 0x39 
#define CT_SYMLOOP 0x40 
#define CT_TTYNAME 0x41 
#define CT_TZNAME 0x42
#define CT_CONFVERSION 0x43 

#define CT_GETHOSTNAME 0x50


#define CT_DIS 0x98  //disconnect
#define CT_QUIT 0x99

typedef struct tagPacket{
	int cmd_type;
	int arg1;
	int arg2;
	int arg3; //result value
	long arg4; // Using struct 1. info
	char msg[256]; //Using 1.uts, 2.gethostname(40)
}NPACKET;

void * thread_service(void * param);

int main()
{
	int sd; //socket discripter
	
	struct sockaddr_in server_addr;
	struct sockaddr_in connect_addr;
    
	//step1. crate a socket.
	sd = socket(AF_INET, SOCK_STREAM, 0);//address family
	if(sd<0)
	{
		perror("socket: "); //what a error
	}


	//step2. bind - IP, PORT Number to the socket
	//bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port =htons(9712);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");


	if(bind(sd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in))<0){
		perror("bind: ");
	
	}

	//step 3 listen - the maximum number of clients
	if(listen(sd,5)<0){
		perror("listen");
	}

	//step4 accept client and waiting
	while(1){
		int connect_sd;
		int size;
		pthread_t tid;
		connect_sd = accept(sd, (struct sockaddr *) &connect_addr,&size);
		if(connect_sd <0)
		{
			perror("accept ");
			//return 0;
			return -1;
		}
		printf("connected!\n");
		
		pthread_create(&tid, NULL, thread_service, (void *)connect_sd);

	
	}
	return 0;
}


void * thread_service(void * param)
{
	int conn_sd;

	//1. uts struct
	struct utsname uts;
	uname(&uts);
	
	//2. info struct
	struct sysinfo info;
	sysinfo(&info);

	//3. conf no struct

	conn_sd = (int) param;
	//printf("OS name : %s \n",uts.sysname);
	//char osname = uts.sysname;
	//printf("in char OS name : %s \n",uts.sysname);
	while(1)
	{
		NPACKET packet;
		memset(&packet, 0, sizeof(NPACKET));
		int recvbytes = 0;
		if((recvbytes=recv(conn_sd, (char *)&packet, sizeof(NPACKET), 0))<0)
		{
			perror("recv fail:");
		}
		printf("[thread %d] recived! %d byte\n",(int) pthread_self(),recvbytes);

		switch(packet.cmd_type) //command
		{
			case CT_ADD:
				packet.arg3 = packet.arg1 + packet.arg2; //receive?
				
				send(conn_sd, (char *)&packet, sizeof(NPACKET),0);
				break;

//-----------------------------uts----------------------------------------------------
			case CT_OSNAME:
				//packet.arg3 = packet.arg1 - packet.arg2; //receive?
				strcpy(packet.msg,uts.sysname);
				//packet.arg3 = osname;
				send(conn_sd, (char *)&packet, sizeof(NPACKET),0);
				break;
			case CT_NODENAME:
				strcpy(packet.msg,uts.nodename);
				send(conn_sd, (char *)&packet, sizeof(NPACKET),0);
				break;
			case CT_RELEASE:
				strcpy(packet.msg,uts.release);
				send(conn_sd, (char *)&packet, sizeof(NPACKET),0);
				break;
			case CT_VERSION:
				strcpy(packet.msg,uts.version);
				send(conn_sd, (char *)&packet, sizeof(NPACKET),0);
				break;
			case CT_MACHINE:
				strcpy(packet.msg,uts.machine);
				send(conn_sd, (char *)&packet, sizeof(NPACKET),0);
				break;
//------------------------------info----------------------------------------------------
			case CT_UPTIME:
				packet.arg4 = info.uptime;
				send(conn_sd, (char *)&packet, sizeof(NPACKET),0);
				break;
			case CT_LOADS_1:
				packet.arg4 = info.loads[0];
				send(conn_sd, (char *)&packet, sizeof(NPACKET),0);
				break;
			case CT_LOADS_5:
				packet.arg4 = info.loads[1];
				send(conn_sd, (char *)&packet, sizeof(NPACKET),0);
				break;
			case CT_LOADS_15:
				packet.arg4 = info.loads[2];
				send(conn_sd, (char *)&packet, sizeof(NPACKET),0);
				break;
			case CT_TOTALRAM:
				packet.arg4 = info.totalram;
				send(conn_sd, (char *)&packet, sizeof(NPACKET),0);
				break;
			case CT_FREERAM: 
				packet.arg4 = info.freeram;
				send(conn_sd, (char *)&packet, sizeof(NPACKET),0);
				break;
			case CT_SHAREDRAM:
				packet.arg4 = info.sharedram;
				send(conn_sd, (char *)&packet, sizeof(NPACKET),0);
				break;
			case CT_BUFFERRAM:
				packet.arg4 = info.bufferram;
				send(conn_sd, (char *)&packet, sizeof(NPACKET),0);
				break;
			case CT_TOTALSWAP:
				packet.arg4 = info.totalswap;
				send(conn_sd, (char *)&packet, sizeof(NPACKET),0);
				break;
			case CT_FREESWAP:
				packet.arg4 = info.freeswap;
				send(conn_sd, (char *)&packet, sizeof(NPACKET),0);
				break;
			case CT_PROCS:
				packet.arg4 = info.procs;
				send(conn_sd, (char *)&packet, sizeof(NPACKET),0);
				break;


//------------------------------conf----------------------------------------------------
			
			
			case CT_ARG:
				packet.arg4 = sysconf(_SC_ARG_MAX);
				send(conn_sd, (char *)&packet, sizeof(NPACKET),0);
				break;


			case CT_CHILD:
				packet.arg4 = sysconf(_SC_CHILD_MAX);
				send(conn_sd, (char *)&packet, sizeof(NPACKET),0);
				break;

			case CT_HOSTNAME:
				packet.arg4 = sysconf(_SC_HOST_NAME_MAX);
				send(conn_sd, (char *)&packet, sizeof(NPACKET),0);
				break;

			case CT_LOGINNAME:
				packet.arg4 = sysconf(_SC_LOGIN_NAME_MAX);
				send(conn_sd, (char *)&packet, sizeof(NPACKET),0);
				break;

			case CT_NGROUPS:
				packet.arg4 = sysconf(_SC_NGROUPS_MAX);
				send(conn_sd, (char *)&packet, sizeof(NPACKET),0);
				break;

			case CT_CLKTCK:
				packet.arg4 = sysconf(_SC_CLK_TCK);
				send(conn_sd, (char *)&packet, sizeof(NPACKET),0);
				break;

			case CT_OPEN:
				packet.arg4 = sysconf(_SC_OPEN_MAX);
				send(conn_sd, (char *)&packet, sizeof(NPACKET),0);
				break;

			case CT_PAGESIZE:
				packet.arg4 = sysconf(_SC_PAGESIZE);
				send(conn_sd, (char *)&packet, sizeof(NPACKET),0);
				break;

			case CT_REDUP:
				packet.arg4 = sysconf(_SC_RE_DUP_MAX);
				send(conn_sd, (char *)&packet, sizeof(NPACKET),0);
				break;

			case CT_STREAM:
				packet.arg4 = sysconf(_SC_STREAM_MAX);
				send(conn_sd, (char *)&packet, sizeof(NPACKET),0);
				break;

			case CT_SYMLOOP:
				packet.arg4 = sysconf(_SC_SYMLOOP_MAX);
				send(conn_sd, (char *)&packet, sizeof(NPACKET),0);
				break;

			case CT_TTYNAME:
				packet.arg4 = sysconf(_SC_TTY_NAME_MAX);
				send(conn_sd, (char *)&packet, sizeof(NPACKET),0);
				break;

			case CT_TZNAME:
				packet.arg4 = sysconf(_SC_TZNAME_MAX);
				send(conn_sd, (char *)&packet, sizeof(NPACKET),0);
				break;

			case CT_CONFVERSION:
				packet.arg4 = sysconf(_SC_VERSION);
				send(conn_sd, (char *)&packet, sizeof(NPACKET),0);
				break;

			case CT_GETHOSTNAME:
				gethostname(packet.msg, sizeof(packet.msg));
				send(conn_sd, (char *)&packet, sizeof(NPACKET),0);
				break;

			case CT_DIS:
				break;


			case CT_QUIT:
				break;
		}



	}
}

