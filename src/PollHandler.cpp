#include "../include/PollHandler.hpp"


struct FdCompare
{
    int fd;

    FdCompare(int fd) : fd(fd) {}

    bool operator()(const pollfd& pfd) const 
    {
        return pfd.fd == fd;
    }
};

void PollHandler::addFd(int fd, short events) 
{
    pollfd pfd = {fd, events, 0};
    _fds.push_back(pfd);
}

void PollHandler::removeFd(int fd) 
{
    _fds.erase(std::remove_if(_fds.begin(), _fds.end(), FdCompare(fd)),
    _fds.end());
}

int PollHandler::poll(int timeout) { return ::poll(_fds.data(), _fds.size(),timeout) ;}

const std::vector<pollfd>& PollHandler::getFds() const {return _fds;}