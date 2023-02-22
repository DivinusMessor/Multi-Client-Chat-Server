// This code is a simple implementation of a chat server that accepts multiple clients
// and enables them to chat with each other.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080      // The port number on which the server will listen for connections
#define MAX_CLIENTS 10 // The maximum number of clients that can connect to the server

int clients[MAX_CLIENTS]; // An array to store the client sockets
int num_clients = 0;      // The number of clients currently connected

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // A mutex to protect the shared resource (clients)

// Function to send messages to all clients except the sender
void send_messages(char *message, int sender_socket, char *name)
{
    pthread_mutex_lock(&lock); // Acquire the lock before accessing the shared resource (clients)
    for (int i = 0; i < num_clients; i++)
    {
        if (clients[i] != sender_socket)
        {
            char formatted_message[1024];
            if (strcmp(name, "me") == 0)
            { // Add a tag "[Me]" for messages sent by the server
                snprintf(formatted_message, sizeof(formatted_message), "[Me]: %s", message);
            }
            else
            {
                snprintf(formatted_message, sizeof(formatted_message), "[%s]: %s", name, message);
            }
            send(clients[i], formatted_message, strlen(formatted_message), 0); // Send the message to the client
        }
    }
    pthread_mutex_unlock(&lock); // Release the lock after accessing the shared resource (clients)
}

// Function to handle a client connection
void *handle_client(void *arg)
{
    int client_socket = *(int *)arg;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Get the client IP address and port number
    if (getpeername(client_socket, (struct sockaddr *)&client_addr, &client_len) == -1)
    {
        perror("Getpeername error");
        close(client_socket);
        pthread_exit(NULL);
    }

    char client_info[INET_ADDRSTRLEN + 10];
    sprintf(client_info, "%s:%d", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    printf("New client connected: %s\n", client_info);

    clients[num_clients++] = client_socket; // Add the client socket to the array of clients

    char buffer[1024] = {0};
    char name[20] = {0};
    sprintf(name, "client%d", num_clients); // Assign a name to the client

    // Notify all clients that a new client has entered the chat
    char message[1024];
    snprintf(message, sizeof(message), "%s has entered the chat.\n", client_info);
    send_messages(message, client_socket, "me");

    // Start receiving messages from the client and sending them to other clients
    while (1)
    {
        int read_size = recv(client_socket, buffer, 1024, 0);
        if (read_size <= 0)
        { // If the client has disconnected, break out of the loop
            break;
        }

        send_messages(buffer, client_socket, name); // Send the message to all other clients

        memset(buffer, 0, sizeof(buffer)); // Clear the buffer for the next message
    }

    printf("Client disconnected: %s\n", client_info);

    send_messages(" has left the chat.\n", client_socket, client_info); // Notify other clients that this client has left

    while (1)
    {
        // Read the message from the client
        int read_size = recv(client_socket, buffer, 1024, 0);

        // If the message size is 0 or negative, the client disconnected
        if (read_size <= 0)
        {
            break;
        }

        // Send the received message to all other connected clients
        send_messages(buffer, client_socket, name);

        // Clear the buffer for next use
        memset(buffer, 0, sizeof(buffer));
    }

    // When the loop is exited, the client has disconnected
    printf("Client disconnected: %s\n", client_info);

    // Notify all other clients that the client has left the chat
    send_messages(" has left the chat.\n", client_socket, client_info);

    // Close the client socket and exit the thread
    close(client_socket);
    pthread_exit(NULL);
}

int main()
{
    // create a socket for the server
    int server_socket;
    struct sockaddr_in address;

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    // set socket options to reuse address
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("Setsockopt error");
        exit(EXIT_FAILURE);
    }

    // specify the server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // bind the socket to the specified address and port
    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // listen for incoming connections
    if (listen(server_socket, MAX_CLIENTS) < 0)
    {
        perror("Listen error");
        exit(EXIT_FAILURE);
    }

    // start accepting clients and create a thread to handle each client
    printf("Server is up and listening on port %d...\n", PORT);
    pthread_t threads[MAX_CLIENTS];
    int i = 0;
    while (1)
    {
        int client_socket;
        if ((client_socket = accept(server_socket, (struct sockaddr *)&address, (socklen_t *)&address)) < 0)
        {
            perror("Accept error");
            continue;
        }

        // create a thread to handle the new client
        pthread_create(&threads[i], NULL, handle_client, &client_socket);
        i++;

        if (i == MAX_CLIENTS)
        {
            break;
        }
    }

    // wait for all threads to finish before exiting
    for (int j = 0; j < i; j++)
    {
        pthread_join(threads[j], NULL);
    }

    // close the server socket
    close(server_socket);

    return 0;
}
