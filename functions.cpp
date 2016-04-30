/* Functions.cpp
   contain function calls used
   in main.cpp
*/

//#include<stdio.h>
//#include<netinet/in.h>
#include"functions.h"
#include<iostream>
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
