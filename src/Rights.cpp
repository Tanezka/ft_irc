#include "../include/Rights.hpp"

    Rights::Rights():is_here(true), is_admin(false), has_voice(false), is_invited(false), is_banned(false), is_muted(false){}
    void    Rights::set_here(bool b){is_here = b;}
    void    Rights::set_admin(bool b)
    {
        is_admin = b;
        // MessageHandler::sendReply(fd, )
    }
    void    Rights::set_voice(bool b){has_voice = b;}
    void    Rights::set_invited(bool b){is_invited = b;}
    void    Rights::set_banned(bool b){is_banned = b;}
    void    Rights::set_muted(bool b){is_muted = b;}
    void    Rights::set_fd(int b){fd = b;}
    void    Rights::set_channel(std::string b){b = channelName;}

    bool    Rights::get_here(){return is_here;}
    bool    Rights::get_admin(){return is_admin;}
    bool    Rights::get_voice(){return has_voice;}
    bool    Rights::get_invited(){return is_invited;}
    bool    Rights::get_banned(){return is_banned;}
    bool    Rights::get_muted(){return is_muted;}
    int     Rights::get_fd(){return fd;}
    std::string     Rights::get_channel(){return channelName;}
    std::string     Rights::get_status()
    {
        std::string stat;

        if (is_here)
            stat.append("H");
        else
            stat.append("G");
        if (is_admin)
            stat.append("@");
        if (has_voice)
            stat.append("+");
        return (stat);
    }