#ifndef RIGHTS_HPP
#define RIGHTS_HPP

#include "Server.hpp"
#include <string>
#include <vector>
#include <map>
#include <algorithm>

class Client;
struct Message;
class Rights 
{
private:
    int     fd;
    bool    is_here;
    bool    is_admin;
    bool    has_voice;
    bool    is_invited;
    bool    is_banned;
    bool    is_muted;
    std::string channelName;

public:
    Rights();

    void    set_here(bool);
    void    set_admin(bool);
    void    set_voice(bool);
    void    set_invited(bool);
    void    set_banned(bool);
    void    set_muted(bool);
    void    set_fd(int);
    void    set_channel(std::string);

    bool           get_here();
    bool           get_admin();
    bool           get_voice();
    bool           get_invited();
    bool           get_banned();
    bool           get_muted();
    std::string    get_status(); 
    std::string    get_channel();
    int            get_fd();    
};

#endif
