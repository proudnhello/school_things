#define PORT 8887 

#include <iostream>
#include <string>  

#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h>    

#include <arpa/inet.h>     
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <string.h> 
#include "Message.h"
#include "RobotState.h"
#include "Tickable.h"

// A lot of the code in this assignment is taken from the slides/provided examples

int main(int argc, char * argv[]) {

	int opt = true;
	struct sockaddr_in address;

	// use these strings to indicate the state transitions
	// the robot progresses through.  Do not modify these strings

	std::string robot_waiting = "The robot is waiting";
	std::string robot_moving = "The robot is moving";

	std::string robot_finished_waiting = "The robot finished waiting";
	std::string robot_finished_moving = "The robot finished moving";

	std::string robot_began_waiting = "The robot began waiting";
	std::string robot_begin_moving = "The robot begain moving";

	int server_fd;

	// Constructing State machine and states
	std::shared_ptr<StateMachine> sm = std::make_shared<StateMachine>();

	std::shared_ptr<RobotState> s0 = std::make_shared<RobotState>();
	s0->set_start(robot_began_waiting);
	s0->set_current(robot_waiting);
	s0->set_end(robot_finished_waiting);

	std::shared_ptr<RobotState> s1 = std::make_shared<RobotState>();
	s1->set_start(robot_begin_moving);
	s1->set_current(robot_moving);
	s1->set_end(robot_finished_moving);

	s0->add_next_state("done", s1);
	s1->add_next_state("done", s0);
	sm->set_current_state(s0);

	// Create the socket we will be using for our server
	if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("Socket failed");
		exit(EXIT_FAILURE);
	}

	// Set options that will allow us to reuse the socket if need be
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (char *)&opt, sizeof(opt));

	// Define the type of socket to be used
	address.sin_family = AF_INET;  
    address.sin_addr.s_addr = INADDR_ANY;  
    address.sin_port = htons( PORT );  

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)  {  
		std::cerr << "bind failed" << std::endl;  
        exit(EXIT_FAILURE);  
    }

	// listen for a connection
	if (listen(server_fd, 3) < 0) {  
	std::cerr << "error listening" << std::endl;  
        exit(EXIT_FAILURE);  
    }  
         
    int addrlen = sizeof(address);  
    std::cout << "Waiting for connections ..." << std::endl; 
    
	// Initialize socket to 0
	int new_socket = 0;
	fd_set readfds;
	int max_sd;
	int activity;
	while(true){

		//clear the socket set 
        FD_ZERO(&readfds);
		max_sd = 0;
     
        //add master socket to set 
        FD_SET(server_fd, &readfds);  
        max_sd = server_fd; 

		// If the client socket is valid, add it to the set
		if(new_socket > 0){
			FD_SET(new_socket, &readfds);
			if(new_socket > max_sd){
				max_sd = new_socket;
			}
		}

		// Wait until there is activity on one of the sockets
		activity = select( max_sd+1, &readfds , NULL , NULL , NULL);  
       
        if ((activity < 0) && (errno!=EINTR))  {  
			std::cout << "select error" << errno << std::endl;
			exit(EXIT_FAILURE);
        } 
		// If the activity was on the master socket, connect to the new socket
		if(FD_ISSET(server_fd, &readfds)){
			if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)  {  
				std::cout << "accept error" << std::endl;  
				exit(EXIT_FAILURE);  
			}  

			//inform user of socket number - used in send and receive commands 
			std::cout << "New connection , socket fd, ip port " << new_socket  << "," << inet_ntoa(address.sin_addr)  << ntohs(address.sin_port) << std::endl;  
			
			std::string message = "Hello from the server!";	
			char * cstr = message.data();

			//send new connection greeting message 
			if(send(new_socket, cstr, message.size() + 1, 0) != message.size() + 1)  { 
				std::cout << " error sending" << std::endl;  
			}
						
			std::cout << "Welcome message sent successfully" << std::endl; 
		}
		// Otherwise, it must have been on the other socket
		if(FD_ISSET(new_socket, &readfds)){
			char buffer[1025];  //data buffer of 1K + one byte for a NUL terminator 
			int valread = read(new_socket, buffer, 1024);
			// If something was read in, print it
			if(valread > 0){
				std::string buf_str(buffer);
				small_world::SM_Event input;
				input.ParseFromString(buf_str);
				sm->tick(input);
			}else{ // Otherwise, the client disconnedted
				getpeername(new_socket , (struct sockaddr*)&address , (socklen_t*)&addrlen);  
		    	std::cout << "Host disconnected :"  << inet_ntoa(address.sin_addr) << "," << ntohs(address.sin_port) << std::endl;   
                // Close the socket
                close( new_socket );
				new_socket = 0;
			}
		}

	} 

	close(new_socket);
	shutdown(server_fd, SHUT_RDWR);

	return EXIT_SUCCESS;
}

