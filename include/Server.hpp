#ifndef SERVER_HPP
#define SERVER_HPP

#include "SocketException.hpp"
#include "PollHandler.hpp"
#include "SignalHandler.hpp"
#include "MessageHandler.hpp"
#include "Message.hpp"
#include "Client.hpp"
#include "Rights.hpp"

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdlib>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <poll.h>
#include <fcntl.h>

class Channel;
class Client;

class Server
{
    private:
        sockaddr_in sa;
        int _mainsock_fd;
        int _portNum;
        const std::string& _password;
        const std::string& _host;
        std::vector<pollfd> pfd;
        static  std::map<unsigned int, Client> _clients;
        std::map<std::string, Channel*> _channels;

        PollHandler _pollHandler;
        void setSocket();
        void socketHandle();
        void setNonBlock(int socket);
        void setPollfd();
        void acceptClient(int fd);

    public:
        Server(int port, const std::string& pass, const std::string& host);
        ~Server();
        PollHandler& getPollHandler();
        const std::string& getPass();
        void initServ();
        void run();
        static  std::map<unsigned int, Client> &getClientList();
        const std::string& getHost() const;
};
#endif
