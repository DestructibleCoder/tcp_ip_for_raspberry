#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVER_IP "37.72.81.145"

constexpr int DEFAILT_PORT{8080};
constexpr char EXIT_CHAR{'#'};
constexpr int MAX_BUFFER_SIZE{4096};

bool is_server_connection_closed(const char *msg);

int main(int argc, const char *argv[]) {
  int client;

  struct sockaddr_in server_addres;
  client = socket(AF_INET, SOCK_STREAM, 0);
  if (client < 0) {
    std::cout << "CLIENT ERROR: establishing socket error.";
    exit(0);
  }

  server_addres.sin_port = htons(DEFAILT_PORT);
  server_addres.sin_family = AF_INET;
  inet_pton(AF_INET, SERVER_IP, &server_addres.sin_addr);

  std::cout << "\n=> <S-Del>Client socket created.";

  if (connect(client, reinterpret_cast<const struct sockaddr *>(&server_addres),
              sizeof(server_addres)) == 0) {
    std::cout << "=> Connection server " << inet_ntoa(server_addres.sin_addr)
              << "with port number " << DEFAILT_PORT << "\n";
  }

  char buffer[MAX_BUFFER_SIZE];
  std::cout << "Waiting for server confirmation...\n";
  recv(client, buffer, MAX_BUFFER_SIZE, 0);
  std::cout << "=> Connection established.\n"
            << "Enter " << EXIT_CHAR << " for close the connection.\n";

  while (true) {
    std::cout << "Server: ";
    std::cin.getline(buffer, MAX_BUFFER_SIZE);
    send(client, buffer, MAX_BUFFER_SIZE, 0);
    if (is_server_connection_closed(buffer)) {
      break;
    }

    std::cout << "Server: ";
    recv(client, buffer, MAX_BUFFER_SIZE, 0);
    std::cout << buffer;
    if (is_server_connection_closed(buffer)) {
      break;
    }
    std::cout << std::endl;
  }
  close(client);
  std::cout << "\nGoodbue...\n" << std::endl;

  return 0;
}

bool is_server_connection_closed(const char *msg) {
  for (int i = 0; i < strlen(msg); i++) {
    if (msg[i] == EXIT_CHAR) {
      return true;
    }
  }
  return false;
}
