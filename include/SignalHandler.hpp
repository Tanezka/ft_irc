#ifndef SIGNALHANDLER_HPP
#define SIGNALHANDLER_HPP


#include <csignal>
#include <iostream>
#include <map>
#include "Server.hpp"

class Client;

class SignalHandler {
    
    private:
        static void handler(int signum);
        static sig_atomic_t running;
        static  std::map<unsigned int, Client> &clientlist;

    public:
        static void signals();
        static bool isRunning();
};

#endif