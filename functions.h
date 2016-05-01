/* Functions header file
   includes functions for main.cpp
*/

/*********************************************
   include guard
*********************************************/
#ifndef _FUNCTIONS_H_INCLUDED_
#define _FUNCTIONS_H_INCLUDED_

#include<netinet/in.h>
#include<string>
#include"cipher.h"

void *get_in_addr(struct sockaddr *sa);
std::string decryptMessage(std::string);


struct active_user {
	std::string user;
	std::string addr;
	std::string port;
	active_user(std::string user1, std::string addr1, std::string port1) :
		user(user1), addr(addr1), port(port1) {}

};

std::string getAddr(int);

std::string getPort(std::string);

#endif
