#include<iostream>
#include<cstring>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
const int PORT=8888;
const int BUFSIZE=1024;
int main() { 
    int client_fd = socket(AF_INET,SOCK_STREAM,0);
    if (client_fd < 0) {
        perror("socket fail");
        return -1;
    }
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET,"100.0.0.1",&addr.sin_addr) <= 0) {
        perror("inet_pton fail");
        close(client_fd);
        return -1;
    }
    
    if (connect(client_fd,(sockaddr*)&addr,sizeof(addr)) < 0) {
        perror("connect fail");
        close(client_fd);
        return -1;
    }
    std::cout << "connect success" << std::endl;
    char buf[BUFSIZE];
    while (1) {
        memset(buf,0,sizeof(buf));
        std::string input;
        std::cin>> input;
        if (input == "exit") {
            break;  
        }
        send(client_fd,input.c_str(),input.size(),0);
        read(client_fd,buf,sizeof(buf));
        std::cout << "recv from server: " << buf << std::endl;

    }
    close(client_fd);
    return 0;

}