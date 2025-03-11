#include <iostream>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

constexpr int DEFAULT_PORT{8080};
constexpr int MAX_BUFFER_SIZE{4096};
constexpr char EXIT_SYMBOL{'#'};

bool is_client_connection_closed(const char *msg);

int main(int argc, char const *argv[]) {
  int client, server;

  struct sockaddr_in server_addres;

  client = socket(AF_INET, SOCK_STREAM, 0);
  if (client < 0) {
    std::cout << "SERVER ERROR: establishing socket error." << std::endl;
    exit(0);
  }

  std::cout << "SERVER: Socket for server was created successfull.\n";
  server_addres.sin_port = htons(DEFAULT_PORT);
  server_addres.sin_family = AF_INET;
  server_addres.sin_addr.s_addr = htons(INADDR_ANY);

  int ret = bind(client, reinterpret_cast<struct sockaddr *>(&server_addres),
                 sizeof(server_addres));

  if (ret < 0) {
    std::cout << "SERVER ERROR: binding connection. Socket has already been "
                 "establishing."
              << std::endl;
    return -1;
  }

  socklen_t size = sizeof(server_addres);
  std::cout << "SERVER: listening clients...\n";

  listen(client, 1);

  server = accept(client, reinterpret_cast<struct sockaddr *>(&server_addres),
                  &size);
  if (server < 0) {
    std::cout << "SERVER ERROR: can't accepting client." << std::endl;
  }

  char buffer[MAX_BUFFER_SIZE];
  bool isExit = false;
  while (server > 0) {
    strcpy(buffer, "=> Server connected!\n");
    send(server, buffer, MAX_BUFFER_SIZE, 0);
    std::cout << "=> Connected to the client #1" << std::endl
              << "Enter " << EXIT_SYMBOL << " to end the connection\n\n";

    std::cout << "Client: ";
    recv(server, buffer, MAX_BUFFER_SIZE, 0);
    std::cout << buffer << std::endl;
    if (is_client_connection_closed(buffer)) {
      isExit = true;
    }

    while (isExit) {
      std::cout << "Server: ";
      std::cin.getline(buffer, MAX_BUFFER_SIZE);
      send(server, buffer, MAX_BUFFER_SIZE, 0);
      if (is_client_connection_closed(buffer)) {
        break;
      }

      std::cout << "Client: ";
      recv(server, buffer, MAX_BUFFER_SIZE, 0);
      std::cout << buffer << std::endl;
      if (is_client_connection_closed(buffer)) {
        break;
      }
    }
    std::cout << "\nGoodbue...\n" << std::endl;
  }
}

bool is_client_connection_closed(const char *msg) {
  for (size_t i = 0; i < strlen(msg); i++) {
    if (msg[i] == EXIT_SYMBOL) {
      return true;
    }
  }
  return false;
}
