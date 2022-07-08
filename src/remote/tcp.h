#pragma once

#include <exception>
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include <thread>

namespace srtcl::remote
{
    typedef uint32_t port_t;
    class remote_exception: virtual public std::exception
    {
        protected:
            int error_number;               ///< Error number
            int error_offset;               ///< Error offset
            std::string error_message;      ///< Error message

        public:

        /** Constructor (C++ STL string, int, int).
        *  @param msg The error message
        *  @param err_num Error number
        *  @param err_off Error offset
        */
        explicit remote_exception(const std::string& msg, int err_num = -1, int err_off = -1):
            error_number(err_num),
            error_offset(err_off),
            error_message(msg)
        {
            
        }
        virtual ~remote_exception() throw () {}
        virtual const char* what() const throw ()
        {
            return error_message.c_str();
        }
        virtual int getErrorNumber() const throw()
        {
            return error_number;
        }
        virtual int getErrorOffset() const throw()
        {
            return error_offset;
        }
    };
    
    class invalid_address_exception : public remote_exception
    {
        using remote_exception::remote_exception;
    };
    
    class failed_connection_exception : public remote_exception
    {
        using remote_exception::remote_exception;
    };
    
    struct ip_addr_t
    {
        ip_addr_t(){}
        ip_addr_t(const std::string& ip_name)
        {
            data = ip_name;
        }
        const char* get_c_str(void) {return data.c_str();}
        std::string data;
    };
    
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
            
            void connect(void)
            {
                if(inet_pton(AF_INET, ip.get_c_str(), &serv_addr.sin_addr)<=0)  
                {
                    throw invalid_address_exception("Invalid IP address");
                }
                if (connect(socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) 
                {
                    throw failed_connection_exception("Failed remote connection");
                }
                //Need some multithreading stuff here!
            }
            
        private:
            int socket = 0;
            port_t tcp_port;
            struct sockaddr_in serv_addr;
            char buffer[buf_size] = {0};
            ip_addr_t ip;
            std::atomic<bool> connected;
    };
    
    template <const std::size_t buf_size = 1024> class tcp_server_t
    {
        public:
            tcp_server_t(const port_t& port_in)
            {
                tcp_port = port_in;
                address.sin_family = AF_INET;
                address.sin_addr.s_addr = INADDR_ANY;
                address.sin_port = htons(port_in);
            }
            port_t port (void) const {return tcp_port;}
            void host(void)
            {
                if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
                {
                    perror("socket failed");
                    exit(EXIT_FAILURE);
                }
                if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
                {
                    perror("bind failed");
                    exit(EXIT_FAILURE);
                }
                if (listen(server_fd, 3) < 0)
                {
                    perror("listen");
                    exit(EXIT_FAILURE);
                }
                if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
                {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }
            }

        private:
            port_t tcp_port;
            int server_fd, new_socket, valread;
            struct sockaddr_in address; 
            int opt = 1;
            int addrlen = sizeof(address);
            char buffer[buf_size] = {0};
    };
}