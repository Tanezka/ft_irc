#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <ctime>

struct Message 
{
    std::string sender;  // The nickname of the sender
    std::string content; // The message content
    std::time_t timestamp; // The timestamp when the message was sent
    std::string raw;  // The nickname of the sender
    Message(const std::string& s, const std::string& c, const std::string &r);
};
#endif