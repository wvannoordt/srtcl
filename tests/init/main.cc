#include "srtcl.h"

int main(int argc, char** argv)
{
    const srtcl::remote::port_t port = 8080;
    srtcl::remote::ip_addr_t ip("127.0.0.1");
    
    srtcl::remote::tcp_server_t server(port);
    srtcl::remote::tcp_client_t client(ip, port);
    
    bool is_server = false;
    if (argc>1)
    {
        std::string ag(argv[1]);
        is_server = (ag=="-s");
    }
    if (is_server)
    {
        server.host();
    }
    else
    {
        client.connect_to_server();
    }
}