#include<iostream>
#include<./sys/types.h>
#include<unistd.h>
#include<./sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<string.h>
#include<string>

using std::string;
using std::cout;
using std::endl;

int main() {

    // Create A Socket 
    int sock1 = socket(AF_INET, SOCK_STREAM, 0);
    if (sock1 == -1) {

        return 1;
    }
    
    // Creat A Hint structure for the server we're conneting with
    // So here is our 2 variables we are going to use for the hint structure
    int port = 53000;
    // We can use the loop back address of 127.0.0.1 or we can specify the machine's ip
    string ipAddress = "192.168.1.28";
    
    
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    // Connect to the server on the socket
    int connectionStatus = connect(sock1, (sockaddr*)&hint, sizeof(hint));

    if (connectionStatus == -1) {

        return 1;
    }
    

    // Do While Loop:
    char buf[4096];
    string userInput;

    do {
        
        // Enter Lines of Text
        cout << "> ";
        getline(std::cin, userInput);
        

        // Send to server
        int sendRes = send(sock1, userInput.c_str(), userInput.size() +1, 0);
        
        if (sendRes == -1) {
            cout << "Cant Send to Server! Whoops!....\r\n";
            continue;
        }
        
        // Wait for the Response
        memset(buf, 0, 4096);
        int byteRecv = recv(sock1, buf, 4096, 0);
        
        if (byteRecv == -1) {
            
            cout << "There was an Error Getting a Response from the server\r\n";
        }
        else
        {
            // Display the response
            cout << "SERVER> " << string(buf, byteRecv) << "\r\n";
        }

    } while (true);
    
    // Close the socket
    close(sock1);
    return 0;
}