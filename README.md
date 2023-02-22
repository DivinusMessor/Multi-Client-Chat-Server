# Multi-Client Chat Server

A simple implementation of a chat server that allows multiple clients to connect and chat with each other.

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
