// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <seccomp.h>
#include <linux/seccomp.h>
#define PORT 8080 
   
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
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(socket),0);
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(connect),0);
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(getrlimit),0);
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(fstat64),0);
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(ioctl),0);



    seccomp_load(ctx);
    struct sockaddr_in address; 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char *hello = "Hello from client"; 
    char buffer[1024] = {0}; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    memset(&serv_addr, '0', sizeof(serv_addr)); 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 
    send(sock , hello , strlen(hello) , 0 ); 
    printf("Hello message sent\n"); 
    valread = read( sock , buffer, 1024); 
    printf("%s\n",buffer ); 
    return 0; 
} 
