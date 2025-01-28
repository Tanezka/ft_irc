#include "../include/Message.hpp"
    
Message::Message(const std::string& s, const std::string& c, const std::string &r): sender(s), content(c), timestamp(std::time(NULL)), raw(r)
{
	raw.append("\r\n");
}