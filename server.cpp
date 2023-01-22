#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include"parralel_quickSort.hpp"
const int PORT = 2023;

std::vector<int> sortVector(std::vector<int> vec, int number_threads) {
ParallelQuickSort(vec.begin(), vec.end(), std::less<int>(), number_threads);
  return vec;
}

int main() {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    std::cerr << "Error creating socket" << std::endl;
    return 1;
  }

  struct sockaddr_in server_addr;
  std::memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  // Bind the socket to the address
  if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    std::cerr << "Error binding socket" << std::endl;
    return 1;
  }

  // Listen for incoming connections
  listen(sockfd, 5);

  // Accept incoming connections
  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);
  int clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_len);
  if (clientfd < 0) {
    std::cerr << "Error accepting connection" << std::endl;
    return 1;
  }

  // Read the vector from the client
  std::vector<int> v;
  int n;
  int number_of_threads_ser;
  read(clientfd, & number_of_threads_ser, sizeof(int));
  read(clientfd, &n, sizeof(int));
  for (int i = 0; i < n; i++) {
    int x;
    read(clientfd, &x, sizeof(int));
    v.push_back(x);
  }

  // Sort the vector
  v = sortVector(v,number_of_threads_ser);

  // Send the sorted vector back to the client
  write(clientfd, &n, sizeof(int));
  for (int i = 0; i < n; i++) {
    write(clientfd, &v[i], sizeof(int));
  }

  // Close the sockets
  close(clientfd);
  close(sockfd);

  return 0;
}
