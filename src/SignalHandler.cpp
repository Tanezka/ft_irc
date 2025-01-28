#include "../include/SignalHandler.hpp"

std::map<unsigned int, Client> &SignalHandler::clientlist = Server::getClientList();
sig_atomic_t SignalHandler::running = 1;

void SignalHandler::signals()
{
    std::signal(SIGINT, SignalHandler::handler);
    std::signal(SIGTERM, SignalHandler::handler);
    std::signal(SIGQUIT,SignalHandler::handler);
}

void SignalHandler::handler(int signum)
{
    std::map<unsigned int, Client>::iterator    it = clientlist.begin();
    while (it != clientlist.end())
    {
        close(it->first);
        it++;
    }
    running = 0;
    std::cout << "Received Signal " << signum << ".Shutting Down..." << std::endl;
}

bool SignalHandler::isRunning()
{ 
    return running;
}