#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

using namespace std;

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return -1;
    }

    
   if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        return -1;
    }

    
    char username[BUFFER_SIZE] = {0};
    cout << "Enter your username: ";
    cin.getline(username, BUFFER_SIZE);

    
    recv(sock, buffer, BUFFER_SIZE, 0);
    string server_username(buffer);
    cout << "Connected to server: " << server_username << endl;

    
    send(sock, username, strlen(username), 0);

    while (true) {
        
        cout << username << ": ";
        cin.getline(buffer, BUFFER_SIZE);
        send(sock, buffer, strlen(buffer), 0);

        
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_read = recv(sock, buffer, BUFFER_SIZE, 0);
        if (bytes_read <= 0) {
            cout << "Server disconnected." << endl;
            break;
        }
        cout << server_username << ": " << buffer << endl;
    }

    close(sock);
    return 0;
}
