#include <iostream>
#include <string>  
#include <string.h>  

#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h>    

#include <arpa/inet.h>     
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <chrono>
#include <cstdint>

#include "Message.h"

#define PORT 8887

uint64_t timeSinceEpochMillisec() {
	using namespace std::chrono;
	return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

// A lot of the code in this assignment is taken from the slides/provided examples
int main(int argc, char * argv[]) {

	int opt = true;
	struct sockaddr_in address;

	address.sin_family = AF_INET;  
    address.sin_port = htons( PORT );  

	int sock = socket(AF_INET, SOCK_STREAM, 0);

	if(inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) <= 0){
		std::cerr << "Invalid address/Address not supported" << std::endl;
		return EXIT_FAILURE;
	}

	int status;
	if((status = connect(sock, (struct sockaddr*)&address, sizeof(address)))< 0) {
		std::cout << "Connection Failed : " << status << std::endl;
		return EXIT_FAILURE;
	}

	small_world::SM_Event test;
	test.set_event_type("tick");

	char buffer[1025];  //data buffer of 1K + one byte for a NUL terminator 
	int valread = 0;
	std::string serial;
	char* cstr;
	valread = read(sock, buffer, 1024);
	if(valread > 0){
		std::string buf_str(buffer);
		std::cout << buf_str << std::endl;
	}

	//send new connection greeting message 
	while(true){
		test.set_event_time(std::to_string(timeSinceEpochMillisec()));
		test.SerializeToString(&serial);
		cstr = serial.data();
		if( send(sock, cstr, serial.size() + 1, 0) != serial.size() + 1)  {  
			std::cout << " error sending" << std::endl; 
			return EXIT_FAILURE; 
		}
		usleep(500000);
	}

	close(sock);

	return EXIT_SUCCESS;
}

