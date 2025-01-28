#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include "Channel.hpp"

class Channel;
class Client
{
    private:
        int _clientSocketFd;
        std::string _clientIp;
        std::string _nickname;
        std::string _username;
        std::string _realname;
        std::string _ip;
        std::string _srvname;
        std::string _channel;
        std::string _pass;

        bool        _passAccepted;
        bool        _hasNick;
        bool        _hasUser;

    public:
        Client();
        ~Client();
        Client(int socket, const std::string& ip);

        int getFd() const;
        const std::string& getNickname() const;
        const std::string& getRealname() const;
        const std::string& getUsername() const;
        const std::string& getClientIp() const;
        const std::string& getChannel() const;
        const std::string& getPassword() const;

        bool    getPassAccepted() const;
        bool    getHasNick() const;
        bool    getHasUser() const;

        void    setPassAccepted(bool auth);
        void    setHasNick(bool auth);
        void    setHasUser(bool auth);
        void    setNickname(const std::string& nickname);
        void    setRealname(const std::string& realname);
        void    setUsername(const std::string& username);
        void    setClientIp(const std::string& clientIp);
        void    setChannelname(const std::string&);
        void    setPassword(const std::string&);
        bool    isAuthenticated() const;
};

#endif
