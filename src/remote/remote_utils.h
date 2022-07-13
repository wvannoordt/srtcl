#pragma once

#include <exception>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string>
#include <thread>
#include <cstring>

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
    
    class socket_failure_excpetion : public remote_exception
    {
        using remote_exception::remote_exception;
    };
    
    class bind_failure_exception : public remote_exception
    {
        using remote_exception::remote_exception;
    };
    
    class listen_failure_exception : public remote_exception
    {
        using remote_exception::remote_exception;
    };
    
    class accept_exception : public remote_exception
    {
        using remote_exception::remote_exception;
    };
    
    class failed_socket_exception : public remote_exception
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
}