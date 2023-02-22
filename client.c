#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080

// This function is used to receive messages from a client and print them to the console
// in different colors based on whether the message was sent by the server or another client.
// The function is executed in a separate thread.
void *receive_messages(void *arg)
{
    // Cast the argument to an integer to get the socket file descriptor
    int socket = *(int *)arg;
    // Initialize a buffer to store the received message
    char buffer[1024] = {0};

    // Continuously receive messages from the client
    while (1)
    {
        // Receive the message from the client and store it in the buffer
        int read_size = recv(socket, buffer, 1024, 0);

        // If the message was received successfully
        if (read_size > 0)
        {
            // Check if the message is not an empty line
            if (strcmp(buffer, "\n") != 0)
            {
                // Check if the message was sent by the server (contains the tag "[Me]")
                if (strstr(buffer, "[Me]:") != NULL)
                {
                    // Print the message in green
                    printf("\033[1;32m%s\033[0m", buffer);
                }
                else // The message was sent by another client
                {
                    // Print the message in blue
                    printf("\033[1;34m%s\033[0m", buffer);
                }

                // Flush the output buffer to ensure that the message is printed immediately
                fflush(stdout);
            }

            // Clear the buffer for the next message
            memset(buffer, 0, sizeof(buffer));
        }
    }

    // Exit the thread when the loop is exited
    return NULL;
}

// Create a client socket and connect to the server
int main()
{
    int client_socket;
    struct sockaddr_in server_address;

    // Create a new socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    // Set the server address information
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    // Convert the server IP address from text to binary format
    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0)
    {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Create a new thread to receive messages from the server
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, receive_messages, &client_socket);

    // Continuously read user input and send it to the server
    char message[1024] = {0};
    while (1)
    {
        fgets(message, 1024, stdin);
        send(client_socket, message, strlen(message), 0);
        memset(message, 0, sizeof(message));
    }

    // Close the socket and exit the program
    close(client_socket);
    return 0;
}
