#include <iostream>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

const int PORT = 2023;
const char* SERVER_IP = "127.0.0.1";

int main() {
  // Create a socket
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    std::cerr << "Error creating socket" << std::endl;
    return 1;
  }

  // Set up the server address
  struct sockaddr_in server_addr;
  std::memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
    std::cerr << "Error parsing server IP address" << std::endl;
    return 1;
  }

  // Connect to the server
  if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    std::cerr << "Error connecting to server" << std::endl;
    return 1;
  }

  // Send the vector to the server
  std::vector<int> v ;
  int n;
  int number_of_threads;
  std::cout<< "Eneter number of threads for the quicksort : ";
  std::cin>>number_of_threads;
  std::cout << "Enter size of the array : ";
  std::cin >> n;
  std::cout << "Enter elements of the array : ";
  int temp;
  for (int i =0 ; i < n ; ++i)
  {
    std::cin >> temp;
    v.push_back(temp); 
  }
  write(sockfd, &number_of_threads, sizeof(int));
  write(sockfd, &n, sizeof(int));
  for (int i = 0; i < n; i++) {
    // std:: cout << v[i]<<"\n";
    write(sockfd, &v[i], sizeof(int));
  }

  // Receive the sorted vector from the server
  std::vector<int> sorted_v;
  read(sockfd, &n, sizeof(int));
  for (int i = 0; i < n; i++) {
    int x;
    read(sockfd, &x, sizeof(int));
    sorted_v.push_back(x);
  }

  // Display the sorted vector
  for (int i = 0; i < n; i++) {
    std::cout << sorted_v[i] << " ";
  }
  std::cout << std::endl;

  // Close the socket
  close(sockfd);

  return 0;
}
