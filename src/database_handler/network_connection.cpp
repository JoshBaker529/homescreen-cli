
#include "network_connection.hpp"
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

NetworkConnection::NetworkConnection(char *p) {
  port = p;
  int status, yes;
  struct addrinfo hints, *servinfo;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;

  status = getaddrinfo("127.0.0.1", port, &hints, &servinfo);
  if (status != 0) {
    std::cerr << "Error: " << gai_strerror(status) << std::endl;
    exit(1);
  }

  struct addrinfo *stepper;
  for (stepper = servinfo; stepper != 0; stepper = stepper->ai_next) {
    sock_fd =
        socket(stepper->ai_family, stepper->ai_socktype, stepper->ai_protocol);
    if (sock_fd == -1) {
      continue;
    }
    status = setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    if (status == -1) {
      std::cerr << "Error: setsockopt" << std::endl;
      exit(1);
    }
    status = bind(sock_fd, stepper->ai_addr, stepper->ai_addrlen);
    if (status == -1) {
      close(sock_fd);
      continue;
    }
  }

  freeaddrinfo(servinfo);
}

NetworkConnection::~NetworkConnection() { close(sock_fd); }

int NetworkConnection::Listen() { return 0; }
