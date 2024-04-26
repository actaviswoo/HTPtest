/*
Создайте простой TCP сервер на C++, который принимает сообщения от клиента в циклический буфер 
и отправляет обратно подтверждение о получении.
*/

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

constexpr int PORT = 12345;
constexpr int BUFFER_SIZE = 1024;

int createSocket() {
    int server_fd;
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    return server_fd;
}

void setSocketOptions(int server_fd) {
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
}

void bindSocket(int server_fd, struct sockaddr_in &address) {
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
}

int acceptConnection(int server_fd, struct sockaddr_in &address, int &addrlen) {
    int new_socket;
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    return new_socket;
}

void handleClient(int new_socket) {
    char buffer[BUFFER_SIZE] = {0};
    int valread;
    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        valread = read(new_socket, buffer, BUFFER_SIZE);
        if (valread == 0) {
            std::cout << "Client disconnected" << std::endl;
            break;
        } else if (valread < 0) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        std::cout << "Received message: " << buffer << std::endl;
        const char* ack = "Message received";
        if (send(new_socket, ack, strlen(ack), 0) < 0) {
            perror("send");
            exit(EXIT_FAILURE);
        }
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    server_fd = createSocket();
    setSocketOptions(server_fd);

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bindSocket(server_fd, address);

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (true) {
        new_socket = acceptConnection(server_fd, address, addrlen);
        handleClient(new_socket);
        close(new_socket);
    }

    close(server_fd);
    return 0;
}