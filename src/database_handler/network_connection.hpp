// Wrapper class for the C websocket API.
// Will be used completely for local communication for updates to the database

#ifndef NETWORK_CONNECTION_HPP
#define NETWORK_CONNECTION_HPP

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

class NetworkConnection {
public:
  NetworkConnection(char *);
  ~NetworkConnection();
  int Listen();

private:
  // struct addrinfo *servinfo;
  char *port;
  int sock_fd;
};

#endif // !NETOWKR_CONNECTION_HPP
