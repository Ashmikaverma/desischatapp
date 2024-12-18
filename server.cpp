#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

using namespace std;

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

   
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    cout << "Server listening on port " << PORT << "..." << endl;

    
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                             (socklen_t *)&addrlen)) < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    
    char username[BUFFER_SIZE] = {0};
    cout << "Enter your username: ";
    cin.getline(username, BUFFER_SIZE);

    
    send(new_socket, username, strlen(username), 0);

    
    recv(new_socket, buffer, BUFFER_SIZE, 0);
    string client_username(buffer);
    cout << "Connected to client: " << client_username << endl;

    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        
        int bytes_read = recv(new_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_read <= 0) {
            cout << "Client disconnected." << endl;
            break;
        }
        cout << client_username << ": " << buffer << endl;

        
        cout << username << ": ";
        cin.getline(buffer, BUFFER_SIZE);
        send(new_socket, buffer, strlen(buffer), 0);
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
