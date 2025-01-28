#include "../include/Server.hpp"

std::map<unsigned int, Client> Server::_clients;


Server::Server(int port, const std::string& pass, const std::string& host)
    :_portNum(port), _password(pass), _host(host) {}

Server::~Server()
{
        
}

PollHandler& Server::getPollHandler() {return _pollHandler;}
const std::string& Server::getPass() {return _password;}

void Server::initServ()
{
    setSocket();     
}

void Server::setSocket()
{
    try
    {
        socketHandle();
        setNonBlock(_mainsock_fd);
    }
    catch(const SocketException& e)
    {
        std::cerr << "Socket initialization failed: " << e.what() << '\n';
        close(_mainsock_fd);
        exit(1);
    }
        
}

const std::string& Server::getHost() const {return _host;}

void Server::socketHandle()
{
    _mainsock_fd = socket(AF_INET, SOCK_STREAM, 0);
        
	(void)_host;
	(void)_password;
    if(_mainsock_fd == -1)
        throw SocketException("Failed to create socket");
        
    int opt = 1;    
    if(setsockopt(_mainsock_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        throw SocketException("Failed to configure socket");

    sa.sin_family = AF_INET;
    sa.sin_port = htons(_portNum);
    sa.sin_addr.s_addr = INADDR_ANY;

    if(bind(_mainsock_fd, (const sockaddr*)&sa, sizeof(sa)) < 0)
        throw SocketException("Failed bind mode maybe port already used");

    if(listen(_mainsock_fd,10) < 0)
        throw SocketException("Failed to listen on socket");
}

void Server::setNonBlock(int socket)
{
    std::cout << socket << ": Set NonBlocking Mode" << std::endl;

    if(fcntl(socket,F_SETFL, O_NONBLOCK) < 0)
        throw SocketException("Failed to set NonBlocking mode server socket");
}

void Server::run()
{
    SignalHandler::signals();

    std::cout << "Server <" << _mainsock_fd - 2 << "> Connected" << std::endl;
    std::cout << "Waiting to accept a connection..." << std::endl;

    _pollHandler.addFd(_mainsock_fd, POLLIN);

    while (SignalHandler::isRunning()) 
    {
        int ret = _pollHandler.poll(1000);
        if (ret > 0) 
        {
            std::vector<pollfd> fds = _pollHandler.getFds();
            for (size_t i = 0; i < fds.size(); ++i) 
            {
                if (fds[i].fd == _mainsock_fd && (fds[i].revents & POLLIN)) 
                {
                    sockaddr_in sa;
                    socklen_t socklen = sizeof(sa);
                    int tmpfd = accept(_mainsock_fd, (sockaddr *)&sa, &socklen);

                    if (tmpfd > 0) 
                    {
                        acceptClient(tmpfd);
                        std::cout << "Accepted connection from " << inet_ntoa(sa.sin_addr) << ":" << ntohs(sa.sin_port) << "\n";
                        _pollHandler.addFd(tmpfd, POLLIN);
                    } 
                    else if (tmpfd == -1) 
                        std::cerr << "Error on accept\n";
                }
                else if (fds[i].revents & POLLIN) 
                {
                    char buf[1024];
                    memset(buf, 0, sizeof(buf));
                    ssize_t bytes_received = recv(fds[i].fd, buf, sizeof(buf), 0);

                    if (bytes_received == 0) 
                    {
                        std::cout << "Client disconnected, fd: " << fds[i].fd << "\n";
                        MessageHandler::handleData(fds[i].fd, "QUIT :ctrl+c disconnect\r\n");
                        close(fds[i].fd);
                        _pollHandler.removeFd(fds[i].fd);
                        _clients.erase(fds[i].fd);
                    } 
                    else if (bytes_received < 0) 
                    {
                        std::cerr << "Receive error on fd: " << fds[i].fd << "\n";
                        MessageHandler::handleData(fds[i].fd, "QUIT :error disconnect\r\n");
                        close(fds[i].fd);
                        _pollHandler.removeFd(fds[i].fd);
                        _clients.erase(fds[i].fd);
                    }
                    else 
                    {
                        MessageHandler::handleData(fds[i].fd, buf);
                    }
                }
            }
        }
    }
std::cout << "Server Shutting Down Successfully" << std::endl;
}


void Server::acceptClient(int fd)
{
    Client  new_client(fd, inet_ntoa(sa.sin_addr));

    _clients[fd] = new_client;
}

std::map<unsigned int, Client> &Server::getClientList()
{
    return (_clients);
}

    
