#include<iostream>
#include<cstring>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

const int PORT=8888;
const int BUFSIZE=1024;
int main() {
    int server_fd = socket(AF_INET,SOCK_STREAM,0);
    if (server_fd < 0) {
        perror("socket fail");
        return -1;

    }
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);

    if (bind(server_fd,(sockaddr*)&addr,sizeof(addr)) < 0) {
        perror("bind fail");
        close(server_fd);
        return -1;
    }
    if (listen(server_fd,5) < 0) {
        perror("listen fail");
        close(server_fd);
        return -1;
    }
    sockaddr_in client_addr{};
    socklen_t client_len = sizeof(client_addr);
    int conn_fd = accept(server_fd,(sockaddr*)&client_addr,&client_len);
    if (conn_fd < 0) {
        perror("conn fail");
        close(server_fd);
        return -1;
    }
    std::cout << "Client connected: " << inet_ntoa(client_addr.sin_addr) << std::endl;
    char buffer[BUFSIZE];
    while (true) {
        memset(buffer,0,sizeof(buffer));
        ssize_t n = recv(conn_fd,buffer,BUFSIZE-1,0);
        if (n<=0) {
            perror("connection closed or error");
            break;
        } 
        std::cout << "Received: " << buffer << std::endl;
        std::string reply = "server got: " + std::string(buffer);
        send(conn_fd, reply.c_str(), reply.size(), 0);
    }

    close(conn_fd);
    close(server_fd);
    return 0;
}