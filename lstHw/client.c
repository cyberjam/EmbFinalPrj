/* Jae Min Kim
 *
 * I'm Client 
 *
 * Writing Date
 * 2018.12.05 Calc Func ADD SUB - client complete!
 * 2018.12.07 uts complete!
 * 2018.12.08 info complete! TODO: menu, casees's printf byte %&
 *
 */
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

#define CT_ADD 0x01  //cmd type = CT, value : 16X
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

#define CT_DIS 0x98  //disconnect
#define CT_QUIT 0x99

typedef struct tagPacket{
	int cmd_type;
	int arg1;
	int arg2;
	int arg3; //result value
	long arg4; // Using Structure : 1. info
	char msg[256]; //Using Structure : 1 .uts
}NPACKET;

void printmenu(){
	printf("================Command=====================\n");
	printf("1.ADD\n");
	printf("2.OSname(uts.sysname)\n");
	printf("4.nodename(uts.nodename)\n");
	printf("5.Server OS version (uts.version)\n");
	printf("6.Server OS Architecture (uts.machine)\n");
	printf("3.Quit\n");
}

int main(int agrc, char *argv[])
{
	int sd; //socket discripter
	
	struct sockaddr_in s_addr;
	NPACKET packet;

	memset(&packet, 0 ,sizeof(packet));

	//struct sockaddr_in connect_addr;
	//step1. crate a socket.
	sd = socket(AF_INET, SOCK_STREAM, 0);//IPv4, TCP address family
	if(sd<0)
	{
		perror("socket: "); //what a error
	}


	//bzero(&server_addr, sizeof(server_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_port =htons(9712);
	s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	//step2. connect
	if(connect(sd,(struct sockaddr *) &s_addr, sizeof(struct sockaddr_in)) == -1){
		perror("connect fail: ");
		close(sd);
		exit(1);
	}
	int quit=0;
	int opcode=0;
	while(!quit){
		int sendbytes = 0;
		int receivedbytes = 0;

		printmenu();
		printf("Input command: ");
		scanf("%d",&opcode);
		printf("\n");
		memset(&packet, 0, sizeof(NPACKET)); //if error , look at this
		packet.arg1 = 3;
		packet.arg2 = 4;

		switch(opcode){
			case 1:
				packet.cmd_type = CT_ADD;
				strcpy(packet.msg, "hello, I'm a client!");
				if((sendbytes = send(sd, (char *)&packet, sizeof(NPACKET), 0)) < 0){
					perror("send fail:");
				}
				printf("sendbyte :%d\n", sendbytes);
				if((receivedbytes = recv(sd, (char *)&packet, sizeof(NPACKET), 0)) <0){
					perror("send fail: ");
				}
				printf("receivedbytes:%d\n", receivedbytes);
				printf("result: %d \n", packet.arg3);
				sleep(1);
				break;
			case 2:
				packet.cmd_type = CT_OSNAME;
				//strcpy(packet.msg, "hello, I'm a client!");
				if((sendbytes = send(sd, (char *)&packet, sizeof(NPACKET), 0)) < 0){
					perror("send fail:");
				}
				//printf("sendbyte :%d\n", sendbytes);
				if((receivedbytes = recv(sd, (char *)&packet, sizeof(NPACKET), 0)) <0){
					perror("send fail: ");
				}
				//printf("receivedbytes:%d\n", receivedbytes);
				printf("SERVER > My OS name is %s \n", packet.msg);
				printf("\n");
				sleep(1);
				break;
			case 3:
				packet.cmd_type = CT_NODENAME;
				if((sendbytes = send(sd, (char *)&packet, sizeof(NPACKET), 0)) < 0){
					perror("send fail:");
				}
				if((receivedbytes = recv(sd, (char *)&packet, sizeof(NPACKET), 0)) <0){
					perror("send fail: ");
				}
				printf("SERVER > My host name is %s \n", packet.msg);
				printf("\n");
				sleep(1);
				break;

			case 4:
				packet.cmd_type = CT_RELEASE;
				if((sendbytes = send(sd, (char *)&packet, sizeof(NPACKET), 0)) < 0){
					perror("send fail:");
				}
				if((receivedbytes = recv(sd, (char *)&packet, sizeof(NPACKET), 0)) <0){
					perror("send fail: ");
				}
				printf("SERVER > My OS release number is %s \n", packet.msg);
				printf("\n");
				sleep(1);
				break;
			case 5:
				packet.cmd_type = CT_VERSION;
				if((sendbytes = send(sd, (char *)&packet, sizeof(NPACKET), 0)) < 0){
					perror("send fail:");
				}
				if((receivedbytes = recv(sd, (char *)&packet, sizeof(NPACKET), 0)) <0){
					perror("send fail: ");
				}
				printf("SERVER > My OS version is %s \n", packet.msg);
				printf("\n");
				sleep(1);
				break;
		
			case 6:
				packet.cmd_type = CT_MACHINE;
				if((sendbytes = send(sd, (char *)&packet, sizeof(NPACKET), 0)) < 0){
					perror("send fail:");
				}
				if((receivedbytes = recv(sd, (char *)&packet, sizeof(NPACKET), 0)) <0){
					perror("send fail: ");
				}
				printf("SERVER > My OS HW Architecture is %s \n", packet.msg);
				printf("\n");
				sleep(1);
				break;
			case 7:
				packet.cmd_type = CT_UPTIME;
				if((sendbytes = send(sd, (char *)&packet, sizeof(NPACKET), 0)) < 0){
					perror("send fail:");
				}
				if((receivedbytes = recv(sd, (char *)&packet, sizeof(NPACKET), 0)) <0){
					perror("send fail: ");
				}
				printf("SERVER > I'm running to %ld/s \n", packet.arg4);
				printf("\n");
				sleep(1);
				break;
			case 8:
				packet.cmd_type = CT_LOADS_1;
				if((sendbytes = send(sd, (char *)&packet, sizeof(NPACKET), 0)) < 0){
					perror("send fail:");
				}
				if((receivedbytes = recv(sd, (char *)&packet, sizeof(NPACKET), 0)) <0){
					perror("send fail: ");
				}
				printf("SERVER > %ld \n", packet.arg4);
				printf("\n");
				sleep(1);
				break;
			case 9:
				packet.cmd_type = CT_LOADS_5;
				if((sendbytes = send(sd, (char *)&packet, sizeof(NPACKET), 0)) < 0){
					perror("send fail:");
				}
				if((receivedbytes = recv(sd, (char *)&packet, sizeof(NPACKET), 0)) <0){
					perror("send fail: ");
				}
				printf("SERVER > %ld \n", packet.arg4);
				printf("\n");
				sleep(1);
				break;
		
			case 10:
				packet.cmd_type = CT_LOADS_15;
				if((sendbytes = send(sd, (char *)&packet, sizeof(NPACKET), 0)) < 0){
					perror("send fail:");
				}
				if((receivedbytes = recv(sd, (char *)&packet, sizeof(NPACKET), 0)) <0){
					perror("send fail: ");
				}
				printf("SERVER > %ld \n", packet.arg4);
				printf("\n");
				sleep(1);
				break;

			case 11:
				packet.cmd_type = CT_TOTALRAM;
				if((sendbytes = send(sd, (char *)&packet, sizeof(NPACKET), 0)) < 0){
					perror("send fail:");
				}
				if((receivedbytes = recv(sd, (char *)&packet, sizeof(NPACKET), 0)) <0){
					perror("send fail: ");
				}
				printf("SERVER > %ld \n", packet.arg4);
				printf("\n");
				sleep(1);
				break;

			case 12:
				packet.cmd_type = CT_FREERAM;
				if((sendbytes = send(sd, (char *)&packet, sizeof(NPACKET), 0)) < 0){
					perror("send fail:");
				}
				if((receivedbytes = recv(sd, (char *)&packet, sizeof(NPACKET), 0)) <0){
					perror("send fail: ");
				}
				printf("SERVER > %ld \n", packet.arg4);
				printf("\n");
				sleep(1);
				break;

			case 13:
				packet.cmd_type = CT_SHAREDRAM;
				if((sendbytes = send(sd, (char *)&packet, sizeof(NPACKET), 0)) < 0){
					perror("send fail:");
				}
				if((receivedbytes = recv(sd, (char *)&packet, sizeof(NPACKET), 0)) <0){
					perror("send fail: ");
				}
				printf("SERVER > %ld \n", packet.arg4);
				printf("\n");
				sleep(1);
				break;


			case 14:
				packet.cmd_type = CT_BUFFERRAM;
				if((sendbytes = send(sd, (char *)&packet, sizeof(NPACKET), 0)) < 0){
					perror("send fail:");
				}
				if((receivedbytes = recv(sd, (char *)&packet, sizeof(NPACKET), 0)) <0){
					perror("send fail: ");
				}
				printf("SERVER > %ld \n", packet.arg4);
				printf("\n");
				sleep(1);
				break;


			case 15:
				packet.cmd_type = CT_TOTALSWAP;
				if((sendbytes = send(sd, (char *)&packet, sizeof(NPACKET), 0)) < 0){
					perror("send fail:");
				}
				if((receivedbytes = recv(sd, (char *)&packet, sizeof(NPACKET), 0)) <0){
					perror("send fail: ");
				}
				printf("SERVER > %ld \n", packet.arg4);
				printf("\n");
				sleep(1);
				break;

			case 16:
				packet.cmd_type = CT_FREESWAP;
				if((sendbytes = send(sd, (char *)&packet, sizeof(NPACKET), 0)) < 0){
					perror("send fail:");
				}
				if((receivedbytes = recv(sd, (char *)&packet, sizeof(NPACKET), 0)) <0){
					perror("send fail: ");
				}
				printf("SERVER > %ld \n", packet.arg4);
				printf("\n");
				sleep(1);
				break;

			case 17:
				packet.cmd_type = CT_PROCS;
				if((sendbytes = send(sd, (char *)&packet, sizeof(NPACKET), 0)) < 0){
					perror("send fail:");
				}
				if((receivedbytes = recv(sd, (char *)&packet, sizeof(NPACKET), 0)) <0){
					perror("send fail: ");
				}
				printf("SERVER > %ld \n", packet.arg4);
				printf("\n");
				sleep(1);
				break;
//------------------------------info----------------------------------------------------

			case 20:
				packet.cmd_type = CT_ARG;
				if((sendbytes = send(sd, (char *)&packet, sizeof(NPACKET), 0)) < 0){
					perror("send fail:");
				}
				if((receivedbytes = recv(sd, (char *)&packet, sizeof(NPACKET), 0)) <0){
					perror("send fail: ");
				}
				printf("SERVER > %ld \n", packet.arg4);
				printf("\n");
				sleep(1);
				break;
			case 21:
				packet.cmd_type = CT_CHILD;
				if((sendbytes = send(sd, (char *)&packet, sizeof(NPACKET), 0)) < 0){
					perror("send fail:");
				}
				if((receivedbytes = recv(sd, (char *)&packet, sizeof(NPACKET), 0)) <0){
					perror("send fail: ");
				}
				printf("SERVER > %ld \n", packet.arg4);
				printf("\n");
				sleep(1);
				break;
			case 22:
				packet.cmd_type = CT_HOSTNAME;
				if((sendbytes = send(sd, (char *)&packet, sizeof(NPACKET), 0)) < 0){
					perror("send fail:");
				}
				if((receivedbytes = recv(sd, (char *)&packet, sizeof(NPACKET), 0)) <0){
					perror("send fail: ");
				}
				printf("SERVER > %ld \n", packet.arg4);
				printf("\n");
				sleep(1);
				break;
			case 23:
				packet.cmd_type = CT_LOGINNAME;
				if((sendbytes = send(sd, (char *)&packet, sizeof(NPACKET), 0)) < 0){
					perror("send fail:");
				}
				if((receivedbytes = recv(sd, (char *)&packet, sizeof(NPACKET), 0)) <0){
					perror("send fail: ");
				}
				printf("SERVER > %ld \n", packet.arg4);
				printf("\n");
				sleep(1);
				break;
			case 24:
				packet.cmd_type = CT_NGROUPS;
				if((sendbytes = send(sd, (char *)&packet, sizeof(NPACKET), 0)) < 0){
					perror("send fail:");
				}
				if((receivedbytes = recv(sd, (char *)&packet, sizeof(NPACKET), 0)) <0){
					perror("send fail: ");
				}
				printf("SERVER > %ld \n", packet.arg4);
				printf("\n");
				sleep(1);
				break;
			case 25:
				packet.cmd_type = CT_CLKTCK;
				if((sendbytes = send(sd, (char *)&packet, sizeof(NPACKET), 0)) < 0){
					perror("send fail:");
				}
				if((receivedbytes = recv(sd, (char *)&packet, sizeof(NPACKET), 0)) <0){
					perror("send fail: ");
				}
				printf("SERVER > %ld \n", packet.arg4);
				printf("\n");
				sleep(1);
				break;
			case 26:
				packet.cmd_type = CT_OPEN;
				if((sendbytes = send(sd, (char *)&packet, sizeof(NPACKET), 0)) < 0){
					perror("send fail:");
				}
				if((receivedbytes = recv(sd, (char *)&packet, sizeof(NPACKET), 0)) <0){
					perror("send fail: ");
				}
				printf("SERVER > %ld \n", packet.arg4);
				printf("\n");
				sleep(1);
				break;
			case 27:
				packet.cmd_type = CT_PAGESIZE;
				if((sendbytes = send(sd, (char *)&packet, sizeof(NPACKET), 0)) < 0){
					perror("send fail:");
				}
				if((receivedbytes = recv(sd, (char *)&packet, sizeof(NPACKET), 0)) <0){
					perror("send fail: ");
				}
				printf("SERVER > %ld \n", packet.arg4);
				printf("\n");
				sleep(1);
				break;
			case 28:
				packet.cmd_type = CT_REDUP;
				if((sendbytes = send(sd, (char *)&packet, sizeof(NPACKET), 0)) < 0){
					perror("send fail:");
				}
				if((receivedbytes = recv(sd, (char *)&packet, sizeof(NPACKET), 0)) <0){
					perror("send fail: ");
				}
				printf("SERVER > %ld \n", packet.arg4);
				printf("\n");
				sleep(1);
				break;
			case 29:
				packet.cmd_type = CT_STREAM;
				if((sendbytes = send(sd, (char *)&packet, sizeof(NPACKET), 0)) < 0){
					perror("send fail:");
				}
				if((receivedbytes = recv(sd, (char *)&packet, sizeof(NPACKET), 0)) <0){
					perror("send fail: ");
				}
				printf("SERVER > %ld \n", packet.arg4);
				printf("\n");
				sleep(1);
				break;
			case 30:
				packet.cmd_type = CT_SYMLOOP;
				if((sendbytes = send(sd, (char *)&packet, sizeof(NPACKET), 0)) < 0){
					perror("send fail:");
				}
				if((receivedbytes = recv(sd, (char *)&packet, sizeof(NPACKET), 0)) <0){
					perror("send fail: ");
				}
				printf("SERVER > %ld \n", packet.arg4);
				printf("\n");
				sleep(1);
				break;
			case 31:
				packet.cmd_type = CT_TTYNAME;
				if((sendbytes = send(sd, (char *)&packet, sizeof(NPACKET), 0)) < 0){
					perror("send fail:");
				}
				if((receivedbytes = recv(sd, (char *)&packet, sizeof(NPACKET), 0)) <0){
					perror("send fail: ");
				}
				printf("SERVER > %ld \n", packet.arg4);
				printf("\n");
				sleep(1);
				break;
			case 32:
				packet.cmd_type = CT_TZNAME;
				if((sendbytes = send(sd, (char *)&packet, sizeof(NPACKET), 0)) < 0){
					perror("send fail:");
				}
				if((receivedbytes = recv(sd, (char *)&packet, sizeof(NPACKET), 0)) <0){
					perror("send fail: ");
				}
				printf("SERVER > %ld \n", packet.arg4);
				printf("\n");
				sleep(1);
				break;
			case 33:
				packet.cmd_type = CT_CONFVERSION;
				if((sendbytes = send(sd, (char *)&packet, sizeof(NPACKET), 0)) < 0){
					perror("send fail:");
				}
				if((receivedbytes = recv(sd, (char *)&packet, sizeof(NPACKET), 0)) <0){
					perror("send fail: ");
				}
				printf("SERVER > %ld \n", packet.arg4);
				printf("\n");
				sleep(1);
				break;
			//case 40:
				

		}
	}
	/*
	//step2. bind - IP, PORT Number to the socket
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
	*/
	return 0;
}

/*
void * thread_service(void * param)
{
	int conn_sd;
	conn_sd = (int) param;
	while(1)
	{
		NPACKET packet;
		memset(&packet, 0, sizeof(NPACKET));

		if(recv(conn_sd, (char *)&packet, sizeof(NPACKET), 0)<0)
		{
			perror("recv fail:");
		}
		printf("[thread %d] recived!",(int) pthread_self());

		switch(packet.cmd_type) //command
		{
			case CT_ADD:
				packet.arg3 = packet.arg1 + packet.arg2; //receive?

				send(conn_sd, (char *)&packet, sizeof(NPACKET),0);
				break;
			case CT_SUB:
				break;
			case CT_DIS:
				break;
			case CT_QUIT:
				break;
		}



	}
}
*/
