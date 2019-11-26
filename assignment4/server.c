// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <seccomp.h>
#include <linux/seccomp.h>
//#define PORT 8080  // defines default port
#define MAXCHAR 1024

int PORT;
int server_fd, new_socket, valread; 
struct sockaddr_in address; 
int opt = 1; 
int addrlen = sizeof(address); 
char buffer[1024] = {0}; 
pid_t child_pid;

void messageExchange(char *filepath){
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
        char portStr[12];
        sprintf(portStr, "%d", PORT);
        char * ls_args[] = { "./server" ,portStr, filepath ,str, NULL};
        printf("\n re-executing \n");
        chroot(filepath);
        execvp(ls_args[0], ls_args);
    }
    if(child_pid<0){
        perror("accept"); 
        exit(EXIT_FAILURE); 
    }
    
}
void childProcess(char *filename, int new_socket){
        setuid(getuid());	//dropping privileges
		if(setuid(-1) == 0)
		{
			exit(EXIT_FAILURE);
		}
        //exchange of messages
        valread = read( new_socket , buffer, 1024); 
        printf("%s\n",buffer );
        FILE *fp;
        char str[MAXCHAR];
        fp = fopen("sample.txt", "r");
        if (fp == NULL){
            printf("Could not open file %s");
        }
        while (fgets(str, MAXCHAR, fp) != NULL)
            printf("%s", str);
        fclose(fp);
        send(new_socket , str , strlen(str) , 0 ); 
        printf("Message sent\n");
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

    scmp_filter_ctx ctx;
    ctx = seccomp_init(SCMP_ACT_TRAP);

    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(open),0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(ioctl),0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(close),0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(access),0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(bsdthread_register),0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(sysctlbyname),0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(issetugid),0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(ioctl),0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(mprotect),0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(mprotect),0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(mprotect),0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(mprotect),0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(mprotect),0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(mprotect),0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(mprotect),0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(mprotect),0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(getentropy),0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(getpid),0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(stat64),0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(csops),0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(proc_info),0);
	seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(csops),0);


    seccomp_load(ctx);


    PORT = atoi(argv[1]);
    char *filepath = argv[2];
    // argv[3] indicates if its a child Process
    if(!argv[3]){
        socketConnection();
        messageExchange(filepath);
    }
    else{
        int new_socket_val;
        new_socket_val = atoi(argv[3]);
        childProcess(filepath, new_socket_val);
    }
   return 0;
}