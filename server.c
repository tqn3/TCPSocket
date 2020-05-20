#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<sys/un.h> 
#include<netdb.h> 
#include<netinet/in.h> 
#include<arpa/inet.h> 
#include<iostream>

using namespace std;

enum IFACE{
	IP_1,
	IP_2,
	IP_3,
	IP_4,
	IP_5,
	IP_6,
	IP_MAX
};

struct Socket{
	char address[60];
	int port;
};

struct Socket *readLocalIP(IFACE addr)
{
	char * var;
	int port;

	Socket *socket;
	socket = (Socket*)malloc(sizeof(struct Socket));

	switch(addr){
		case IP_1: 
			var = getenv("IP_1"); 
			port = 61000;
			break;
		case IP_2: 
			var = getenv("IP_2");
			port = 61001;
			break;
		case IP_3: 
			var = getenv("IP_3"); 
			port = 61002;
			break;
		case IP_4: 
			var = getenv("IP_4");
			port = 61003;
			break;
		case IP_5: 
			var = getenv("IP_5");
			port = 61004;
			break;
		case IP_6: 
			var = getenv("IP_6"); 
			port = 61005;
			break;
		default:
			break;
	}

	if (!var){
		var = "158.185.95.100";
		port = 61000;
	}

	strcpy(socket->address, var);	
	socket->port = port;

	return socket;
}

int init(Socket *sock){
	int socketFd;
	int ret;
	
	socketFd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in localSocket;
	localSocket.sin_family = AF_INET;
	localSocket.sin_addr.s_addr = inet_addr(sock->address);
	localSocket.sin_port = htons(sock->port);

	ret = bind(socketFd, (struct sockaddr*) &localSocket, sizeof(localSocket));

	if (ret == 0){
		cout << "Successful bind on : " << sock->address << endl;
	}
	else{
		cout << "Failed to bind on : " << sock->address << endl;
	}

	return socketFd;
}

void listen(int sockId, Socket *sock, char *clientIp) {
	char msg_send[256], msg_rev[256];
	struct sockaddr_in client;
    socklen_t addr_size; 

    addr_size = sizeof(struct sockaddr_in);

	int acc = accept(sockId, (struct sockaddr*) &client, &addr_size);
	char ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(client.sin_addr), clientIp, INET_ADDRSTRLEN);

    printf("connection established with IP : %s and PORT : %d\n",  
                                            clientIp, ntohs(client.sin_port));

	recv(acc, msg_rev, 256, 0);
	cout << "Client acknowledge " << msg_rev << endl;
	strcpy(msg_send, "Received");
	send(acc, msg_send, 256, 0);
}



int main(int argc, char *argv[]) {
	Socket *sock[IP_MAX];
	int socketFd;
	char *clientIp[IP_MAX];
	int i;

	for (i = 0; i < IP_MAX; i++){

		sock[i] = readLocalIP((IFACE)i);

		if (sock[i]){
			socketFd = init(sock[i]);
		}
		else{
			cout << "failed to create socket " + i << endl;
		}

		listen(socketFd, 5);
	}

	while(1){

	}



	for (int i = 0; i < IP_MAX; i++){
		free(sock[i]);
	}	
	return 0;
}
