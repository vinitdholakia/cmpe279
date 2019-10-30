// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 8080 

int server_fd, new_socket, valread; 
struct sockaddr_in address; 
int opt = 1; 
int addrlen = sizeof(address); 
char buffer[1024] = {0}; 
char *hello = "Hello from server"; 
pid_t child_pid;

void messageExchange(){
    //accept new connection and then fork new process
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0) 
    {
        perror("accept"); 
        exit(EXIT_FAILURE); 
    }
    child_pid = fork();
    if(child_pid == 0 ){
        char str[12];
        sprintf(str, "%d", new_socket);
        char * ls_args[] = { "./server" , "child_true", str, NULL};
        // printf("%s\n", ls_args[1]);
        execvp(ls_args[0], ls_args);
    }
    if(child_pid<0){
        perror("accept"); 
        exit(EXIT_FAILURE); 
    }
    
}
void childProcess(int new_socket){
		setuid(getuid());	//dropping privileges
		if(setuid(-1) == 0)
		{
			exit(EXIT_FAILURE);
		}
        //exchange of messages
        valread = read( new_socket , buffer, 1024); 
        printf("%s\n",buffer ); 
        send(new_socket , hello , strlen(hello) , 0 ); 
        printf("Hello message sent\n");
}

//establishing socket connection
void socketConnection(){
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
      &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        // exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 

    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
       sizeof(address))<0) 
    { 
        perror("bind failed");
        exit(EXIT_FAILURE); 
    }
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    }
}
int main(int argc, char const *argv[]) 
{
    if(!argv[1]){
        socketConnection();
        messageExchange();
    }
    else{
        int new_socket_val;
        new_socket_val = atoi(argv[2]);
        childProcess(new_socket_val);
    }
   return 0;
}