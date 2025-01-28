#pragma once

#include <string>
#include <exception>

class SocketException : public std::exception{

    private:
        std::string _message;

    public:
        SocketException(const std::string& msg) : _message(msg) {}

        virtual const char* what() const throw(){
            return _message.c_str();
        }
        
        virtual ~SocketException() throw() {}
};