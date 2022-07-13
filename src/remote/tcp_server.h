#pragma once

#include "remote/remote_utils.h"

namespace srtcl::remote
{
    template <const std::size_t buf_size = 1024> class tcp_server_t
    {
        public:
            tcp_server_t(const port_t& port_in)
            {
                tcp_port = port_in;
                address.sin_family = AF_INET;
                address.sin_addr.s_addr = INADDR_ANY;
                address.sin_port = htons(port_in);
                timeout = 5000; //milliseconds
            }
            port_t port (void) const {return tcp_port;}
            void host(void)
            {
                int er;
                if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
                    throw socket_failure_excpetion("socket failed");
                er = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
                er = ioctl(server_fd, FIONBIO, (char *)&on);
                if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
                    throw bind_failure_exception("bind failed");
                if (listen(server_fd, 3) < 0)
                    throw listen_failure_exception("listen failure");
                memset(fds, 0, sizeof(fds));
                fds[0].fd = server_fd;
                fds[0].events = POLLIN;
                
                // everything below probably needs to be done in a separate thread
                er = poll(fds, nfds, timeout);
                if (er <  0) print("call to poll failed");
                if (er == 0) print("poll timed out");
                int current_size = nfds;
                int new_sd;
                for (int i = 0; i < current_size; i++)
                {
                    if(fds[i].revents == 0) continue;
                    if(fds[i].revents != POLLIN)
                    {
                        printf("  Error! revents = %d\n", fds[i].revents);
                        // end_server = TRUE;
                        break;
                    }
                    if (fds[i].fd == server_fd)
                    {
                        printf("  Listening socket is readable\n");
                        do
                        {
                            new_sd = accept(server_fd, NULL, NULL);
                            if (new_sd < 0)
                            {
                                if (errno != EWOULDBLOCK)
                                {
                                    perror("  accept() failed");
                                    // end_server = TRUE;
                                }
                                break;
                            }
                            printf("  New incoming connection - %d\n", new_sd);
                            fds[nfds].fd = new_sd;
                            fds[nfds].events = POLLIN;
                            nfds++;
                        } while (new_sd != -1);
                        //https://www.ibm.com/docs/en/i/7.4?topic=designs-using-poll-instead-select
                        // if ((tcp_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
                        // {
                        //     throw accept_exception("accept");
                        // }
                    }
                }
            }
            void set_timeout(const int& to) {timeout = to;}
        private:
            struct pollfd fds[200];
            int nfds = 1;
            port_t tcp_port;
            int server_fd, tcp_socket, valread, on, timeout;
            struct sockaddr_in address; 
            int opt = 1;
            int addrlen = sizeof(address);
            char buffer[buf_size] = {0};
    };
}