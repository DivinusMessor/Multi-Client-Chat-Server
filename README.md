# Multi-Client Chat Server

A simple implementation of a chat server that allows multiple clients to connect and chat with each other.
The Multi-Client-Chat-Server project consists of two C programs, server.c and client.c, that enable multiple clients to communicate with each other through a central server.

The server.c program creates a server that listens for incoming connections from clients. Once a client has connected, the server assigns a unique ID to the client and adds it to the list of connected clients. The server then forwards any messages it receives from a client to all other connected clients. Clients can disconnect at any time, and the server will remove them from the list of connected clients.

The client.c program allows users to connect to the server and send messages to other connected clients. When a user starts the client.c program, they are prompted to enter their name and the IP address of the server they wish to connect to. Once connected, the client can send messages to all other connected clients. The client.c program also includes a thread that listens for incoming messages from the server and prints them to the console.

Overall, the Multi-Client-Chat-Server project provides a simple example of a client-server architecture and how multiple clients can communicate with each other through a central server.

## Features

- Accepts multiple clients
- Broadcasts messages to all connected clients
- Prints messages in different colors based on sender (server or client)
- Handles disconnection of clients
- Allows multiple clients to connect and communicate in real-time
- Displays the IP address and port number of each client that connects to the server
- Notifies all clients when a new client enters or leaves the chat
- Assigns a unique name to each client based on the order in which they connect
- Prints messages from other clients in blue and messages from the server (broadcasts) in green
- Supports the sending of messages from clients to other connected clients
- Implements a mutex to protect the shared resource (clients array) and prevent data corruption
- Handles the disconnection of clients and notifies all other clients of the disconnected client's departure

## Usage

1. Clone the repository: `git clone https://github.com/DivinusMessor/Multi-Client-Chat-Server.git`
2. Compile the server program: `gcc server.c -o server -lpthread`
3. Start the server: `./server`
4. Compile the client program: `gcc client.c -o client -lpthread`
5. Start the client: `./client`
