//all libs to run
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<time.h>
//deploy--------------
static char* ip_address;
//---------------------
void mapping_services(int port){
	switch(port){
		case 20: printf("ftp-data"); break;
		//case 21: printf("ftp-control"); break;
		case 22: printf("ssh"); break;
		case 23: printf("telnet"); break;
		case 25: printf("smtp"); break;
		case 43: printf("whois"); break;
		case 53: printf("dns"); break;
		case 63: printf("whois++"); break;
		case 69: printf("tftp"); break;
		case 80: printf("http"); break;
		case 107: printf("rtelnet"); break;
		case 109: printf("pop2"); break;
		case 110: printf("pop3"); break;
		case 115: printf("sftp"); break;
		case 118: printf("sql-services"); break;
		case 143: printf("imap"); break;
		case 443: printf("https"); break;
		case 161: printf("snmp"); break;
		case 546: printf("dhcp-client"); break;
		case 547: printf("dhcp-server"); break;
		default: printf("unknow");
	}
}

void* scanning(void* port){
	//---------------------	
	int n = *((int*)port);
	pthread_detach(pthread_self());
	int sockfd, check;
	struct sockaddr_in target;
	sockfd = socket(AF_INET, SOCK_STREAM,0);
	bzero((char*)&target, sizeof(target));
	target.sin_family = AF_INET;
	target.sin_addr.s_addr = inet_addr(ip_address);
	target.sin_port = htons(n);
	check = connect(sockfd, (struct sockaddr*)&target, sizeof(target));
	//----------------	
	if(check != -1){
		printf("  %d\t open\t   ",n);
		mapping_services(n);
		printf("\n");
	}
	close(sockfd);
	return NULL;
}
//----------------------
int main(int argc, char *argv[]){
	//---------------------
	printf("\n+----------------------------------------------------+\n");	
	printf("|THE SIMPLE PORT SCANNER - USING C LANGUAGE - @dienpv  |\n");
	printf("+----------------------------------------------------+\n");
	printf("\nUSAGE\n");
	printf("+--------------------------------------------------------------+\n");
	printf("|./scanner <ip-adress> (default)                               |\n");
	printf("|./scanner <ip-address> <port-start> <port-end> (set port-range)|\n");
	printf("+--------------------------------------------------------------+\n\n");
	if(argc < 2 || argc > 4 || argc == 3){
		perror("Argument is wrong  !!!\n");
		exit(0);
	}

	ip_address = argv[1];
	if(argc == 4){
		int start, end, *port;
		start = atoi(argv[2]);
		end = atoi(argv[3]);
		printf("start: %d  end : %d\n",start,end);
		printf("-PORT-\t-STATE-\t-SERVICE-\n");
		port = &start;
		pthread_t thread_id;
		for(start; start <= end; start++){
			pthread_create(&thread_id, NULL, &scanning,(void*)port);
			sleep(1);
		}
		port = NULL;
	}
	else{
		int i, length, *port;
		pthread_t thread_id;
		int port_list[]={20,21,22,23,25,43,53,63,69,80,107,109,110,115,118,143,161,443,546,547};
		port = port_list;
		length = sizeof(port_list) / sizeof(int);
		printf("-PORT-\t-STATE-\t-SERVICE-\n");
		for(i = 0; i < length; i++){
			pthread_create(&thread_id, NULL, &scanning,(void*)(port+i));
			sleep(1);
		}
		port = NULL;
	}
//-------------------
	return 0;
}




