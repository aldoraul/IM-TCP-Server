/* CS 423 Project 2
 UDP_SERVER
 Aldo Anaya
*/

//#include<stdio.h>
#include<cstdlib>
#include<cstdio>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
//#include<sys/socket.h>
//#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<sys/wait.h>
#include<signal.h>
#include<iostream>
#include<list>
//#include"cipher.h"
#include"functions.h"

// get sockaddr, IPv4 or IPv6

#define MYPORT "34567"
#define MAXBUFFLEN 100
#define BACKLOG 10	 // how many pending connections queue will hold

//	#define ADDR_LEN 50
std::list<active_user> users;
struct active_user clientUser;
void sendReply(struct active_user);
void sendPlusMsg(struct active_user);
int main(void){

	int sockfd;
	//	 int new_sockfd;
	struct addrinfo hints, *servinfo, *p;
	int numbytes;
	int rv;
/*	Code from slides - Iterative - Connetion


	socklent_t sin_size;
	struct sigaction sa;
	End of structs from slides */

	
	//active_user("", their_addr);
	
	//	char strptr[ADDR_LEN];
	//	socklen_t addr_len;
	int yes = 1;
	
	memset(&hints, 0, sizeof hints);
	printf("memset cleared\n");
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	//hints.ai_flags = AI_PASSIVE;
	//char s[100];
	if((rv = getaddrinfo("192.168.10.200", MYPORT, &hints, &servinfo)) != 0){
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
	
	for(p = servinfo;p != NULL; p=p->ai_next){
		if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("TCP_Server: socket");
			continue;
		}
		if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
			perror("setsockopt");
			exit(1);
		}
		if(bind(sockfd, p->ai_addr, p->ai_addrlen) == -1){
			close(sockfd);
			perror("TCP_Server: bind");
			continue;
		}
		break;
	}
	printf("now listening\n");

	if(p == NULL){
		fprintf(stderr, "UDP_Server: failed to bind socket\n");
		exit(1);
	}
//	inet_ntop(servinfo->ai_family,get_in_addr((sockaddr *)servinfo),  s, sizeof s);

	//printf("server: got connection from %s\n", s);

	freeaddrinfo(servinfo);
	
	if(listen(sockfd, BACKLOG)==-1){
		perror("listen");
		exit(1);
		}

	fd_set master;
	fd_set read_fds;  
	FD_ZERO(&master);    
	FD_ZERO(&read_fds);
	FD_SET(sockfd, &master);
	
	int fdmax = sockfd;
	int newfd; 
	struct timeval tv;
	tv.tv_sec = 2;
	tv.tv_usec = 500000;
	
	while(1){
		read_fds = master;
			if(select(fdmax+1,&read_fds, NULL, NULL, &tv) == -1){
			perror("select");
			exit(4);
		}
		struct sockaddr_storage remoteaddr;	
		socklen_t addrlen;
		for (int i = 0; i <= fdmax; i++) {			
			
			char buf[MAXBUFFLEN];
			if (FD_ISSET(i, &read_fds)) { // we got one conncetion!!	
				if (i == sockfd) {// handle new connections
					addrlen = sizeof remoteaddr;
					newfd = accept(sockfd, (struct sockaddr *)&remoteaddr, &addrlen);
					if(newfd == -1){
						perror("accept");
						}
					else{
						FD_SET(newfd, &master);  // add to master
						if(newfd>fdmax)		// keep track of the max
							fdmax=newfd;					
					}
				}else{
					if((numbytes=recv(i,buf, sizeof buf,0))<=0){
						if(numbytes ==0){
							printf("select server: socket %d hung up\n",i);
						}
						else{
							perror("recv");
							close(i);
							FD_CLR(i, &master);	// remove from master	
							}
						
					}else{
						std::string dMessage = "";
						std::string user = "";
						buf[numbytes] = '\0';
					 	dMessage = decryptMessage(buf);	
						//std::cout<< buf  << std::endl;	
						int k = 2;
						while(dMessage[k] != ';'){
							user += dMessage[k];
							k++;
						}
						//std::cout<<user<<std::endl;
						//std::cout<<getAddr(i)<<std::endl;
						//std::cout<<getPort(dMessage)<<std::endl;s
						clientUser = active_user(user, getAddr(i), getPort(dMessage));
					 	std::cout<< clientUser.user << std::endl<<clientUser.addr<<std::endl<<clientUser.port<<std::endl;		
						//users.push_back(newUser);	// getUser creates an new active_user
						
						close(i);
						FD_CLR(i, &master);
						sendReply(clientUser);
						
							}
							
						}
	}}}
	return 0;
}

void sendReply(struct active_user user){
	struct addrinfo hints, *servinfo;
	char *addr;
	char *port;
	int sock1, rv;
	int yes = 1;
	std::string msg = "";
	std::string emsg = "";
	char *eMessage;
	memset(&hints, 0, sizeof hints);
        
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        //hints.ai_flags = AI_PASSIVE;
	//std::cout <<" addr at sendReply is " << user.addr << std::endl;
	addr = new char[user.addr.length()+1];
	strcpy(addr, user.addr.c_str());
	
	port = new char[user.port.length()+1];
	strcpy(port, user.port.c_str());
	
	
        if((rv = getaddrinfo(addr, port, &hints, &servinfo)) != 0){
                fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
                exit(1);
        }

	 if((sock1 = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) {
                        perror("TCP_Server: socket");
                        //continue;
                }
         if(setsockopt(sock1, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
                        perror("setsockopt");
                        exit(1);
                }
         if(connect(sock1, servinfo->ai_addr, servinfo->ai_addrlen) == -1){
                        close(sock1);
                        perror("TCP_Server: connect");
                        //continue;
                }
	
	if(users.size() == 0){
		std::string none = "4;0#";
		char *enone = new char[none.length()+1];
		strcpy(enone, encryptMessage(none).c_str());
		send(sock1, enone,strlen(enone), 0);
		close(sock1);
	}else{

		
		msg = "4;" +std::to_string( users.size())+";\n";
		for(std::list<active_user>::iterator it =users.begin();it!=users.end();++it){
			msg += it->user + ";" + it->addr + ";" + it->port + "\n";
			struct active_user oldUser = active_user(it->user, it->addr, it->port);
			sendPlusMsg(oldUser);	
		}
		msg += "#";
		emsg = encryptMessage(msg);
		eMessage = new char[msg.length() +1];
		strcpy(eMessage, emsg.c_str());
		if (send(sock1, eMessage, strlen(eMessage), 0) < 0) {		
			std::cout << "Send Failed" << std::endl;					
			exit(1);										
		}
	}
	users.push_back(user);
}
void sendPlusMsg(struct active_user user){

	struct addrinfo hints, *servinfo;
	char *addr;
	char *port;
	int sock1, rv;
	int yes = 1;
	std::string msg = "";
	std::string emsg = "";
	char *eMessage;
	memset(&hints, 0, sizeof hints);
        
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        //hints.ai_flags = AI_PASSIVE;
	//std::cout <<" addr at sendReply is " << user.addr << std::endl;
	addr = new char[user.addr.length()+1];
	strcpy(addr, user.addr.c_str());
	
	port = new char[user.port.length()+1];
	strcpy(port, user.port.c_str());
	
	
        if((rv = getaddrinfo(addr, port, &hints, &servinfo)) != 0){
                fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
                exit(1);
        }

	 if((sock1 = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) {
                        perror("TCP_Server: socket");
                        //continue;
                }
         if(setsockopt(sock1, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
                        perror("setsockopt");
                        exit(1);
                }
         if(connect(sock1, servinfo->ai_addr, servinfo->ai_addrlen) == -1){
                        close(sock1);
                        perror("TCP_Server: connect");
                        //continue;
                }
	msg = "4;new;\n" + user.user + ";" + user.addr + ";" + user.port + "#";
	eMessage = new char[msg.length()+1];
	strcpy(eMessage, encryptMessage(msg).c_str());
	if(send(sock1, eMessage, strlen(eMessage), 0) < 0) {
		std::cout << "Send old client Failed" << std::endl;
		exit(1);
	}
}


/* Original iterative server code	
	while(1){
		printf("working or not\n");
		printf("\n");
		addr_len = sizeof their_addr;
		if((numbytes = recvfrom(sockfd, buf, MAXBUFFLEN-1, 0, (struct sockaddr *)&their_addr, &addr_len))== -1){
			perror("\trecvfrom");
			exit(1);
		}
		buf[numbytes] = '\0';
		// new_sockfd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_len);
		
		printf("\tUDP_Server: got packet from %s\n", inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr),strptr, addr_len));
		printf("\tUDP_Server: packet is %d bytes long\n", numbytes);
		buf[numbytes]='\0';
		printf("\tUDP_Server:packet contains \"%s\"\n", buf);
		if((numbytes = sendto(sockfd, "Server", 6, 0, (struct sockaddr *)&their_addr, addr_len)) == -1) {
			perror("\tUDP_Server: sendto error");
			exit(1);
		}
		printf("and here\n");
	}
*/

		
