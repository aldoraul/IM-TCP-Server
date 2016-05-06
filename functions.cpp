/* Functions.cpp
   contain function calls used
   in main.cpp
*/

//#include<stdio.h>
//#include<netinet/in.h>
#include<arpa/inet.h>
#include"functions.h"
#include<iostream>
#include <sys/socket.h>
void *get_in_addr(struct sockaddr *sa){
	if(sa->sa_family == AF_INET){
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

std::string decryptMessage(std::string buf){
	int recv_size = buf.length();
	std::cout<<recv_size<<std::endl;
	int next = 0;
	for(int i = 0; i < recv_size; i++){
		if(buf[i] == ';'){
			next = ++i;
			break;
		}
				
	}
	for(int j = next;j < recv_size; j++){
		buf[j] = decrypt(buf[j]);
	}
	return buf;
}

std::string encryptMessage(std::string buf){
	int recv_size = buf.length();
	
	int next = 0;
	for(int i = 0; i < recv_size; i++){
		if(buf[i] == ';'){
			next = ++i;
			break;
		}
	}
	for(int j = next; j < recv_size; j++){
		buf[j] = encrypt(buf[j]);
	}
	return buf;
}

std::string getAddr(int fd){
	struct sockaddr_storage addr;
	socklen_t len = sizeof(addr);
	char str[INET6_ADDRSTRLEN];
	std::string clientAddr = "";

	getpeername(fd, (struct sockaddr*)&addr, &len);
	if(addr.ss_family == AF_INET){
		struct sockaddr_in *s = (struct sockaddr_in *)&addr;
		inet_ntop(AF_INET, &s->sin_addr, str, sizeof str);
	}else{
		struct sockaddr_in6 *s = (sockaddr_in6 *)&addr;
		inet_ntop(AF_INET6, &s->sin6_addr, str, sizeof str);
	}
	clientAddr = str;
	return clientAddr;
}

std::string getPort(std::string buf){
	std::string port = "";
	int start = 2;
	while(buf[start] != ';')
        	start++;
	start++;
	while(buf[start] != '#'){
		port += buf[start];
		start++;
	}
	return port;
}


