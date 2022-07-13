#pragma once

#include "remote/remote_utils.h"

namespace srtcl::remote
{
    template <const std::size_t buf_size = 1024> class tcp_client_t
    {
        public:
            tcp_client_t(const ip_addr_t& ip_in, const port_t& port_in)
            {
                ip = ip_in;
                tcp_port = port_in;
                serv_addr.sin_family = AF_INET;
                serv_addr.sin_port = htons(tcp_port);
                connected = false;
            }
            port_t port   (void)      const {return tcp_port;}
            ip_addr_t ip_addr(void)      const {return ip;}
            bool      is_connected(void) const {return connected;}
            
            void connect_to_server(void)
            {
                if ((tcp_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
                    throw failed_socket_exception("Socket creation failure");
                if(inet_pton(AF_INET, ip.get_c_str(), &serv_addr.sin_addr)<=0)
                    throw invalid_address_exception("Invalid IP address");
                if (connect(tcp_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
                    throw failed_connection_exception("Failed remote connection");
                //Need some multithreading stuff here!
            }
            
        private:
            int tcp_socket = 0;
            port_t tcp_port;
            struct sockaddr_in serv_addr;
            char buffer[buf_size] = {0};
            ip_addr_t ip;
            std::atomic<bool> connected;
    };
}