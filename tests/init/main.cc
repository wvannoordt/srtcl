#include "srtcl.h"

int main(int argc, char** argv)
{
    const std::size_t port = 8080;
    srtcl::remote::ip_addr_t ip("127.0.0.1");
    
    srtcl::remote::tcp_server_t server(port);
    srtcl::remote::tcp_client_t client(ip, port);
    
    server.host();
    // client.connect_to_server();
}