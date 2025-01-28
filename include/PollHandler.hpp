#ifndef POLLHANDLER_HPP
#define POLLHANDLER_HPP

#include <vector>
#include <poll.h>
#include <algorithm>

class PollHandler {

    private:
        std::vector<pollfd> _fds;
    
    public:
        void addFd(int fd, short events);
        void removeFd(int fd);
        int poll(int timeout);
        const std::vector<pollfd>& getFds() const;

};

#endif