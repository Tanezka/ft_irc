#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Server.hpp"
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream> //convert

class Client;
class Rights;
struct Message;
class Channel 
{
private:
    std::string                             _name;  // Kanal adı
    std::vector<int>                        _clients;  // Kanalda bulunan istemcilerin dosya tanıtıcıları
    std::map<int, Rights>                   rights;
    std::vector<std::string>                _banList;
    static std::map<unsigned int, Client>   &c_list;
    static std::map<std::string, Channel>   _channels; // Mevcut kanalları saklamak için
    bool                                    is_inviteonly;
    bool                                    is_moderated;
    bool                                    is_keyprotected;
    bool                                    has_limit;
    bool                                    no_extern;
    int                                     user_limit;
    std::string                             password;


public:
    Channel();
    Channel(const std::string& name);
    void                                addClient(int clientFd);
    void                                removeClient(int clientFd);
    void                                broadcast(std::string Message);
    void                                broadcast_butself(int fd, std::string msg); 
    void                                receive_message(struct Message);
    void                                banUser(std::string& nickname);
    void                                listClients() const;
    const std::string&                  getName() const;
    const std::vector<int>&             getClients() const;
    const std::string                   getClientStatus(int);
    const std::string                   getChannelStatus(int);
    void                                reply_ops(int);
    const std::vector<std::string>&     getBanList() const;
    Rights&                             get_right(int);
    bool                                get_inChannel(int);
    bool&                               getInviteOnly();
    bool&                               getIsModerated();
    bool&                               get_invite_only();
    bool&                               get_moderated();
    bool&                               get_keyprotected();
    bool&                               get_has_limit();
    bool&                               get_extern();
    std::string                         get_password();
    unsigned    int                     get_limit();                                
    void                                set_invite_only(bool);
    void                                set_moderated(bool);
    void                                set_keyprotected(bool);
    void                                set_has_limit(bool);
    void                                set_extern(bool);
    void                                set_password(std::string);
    void                                set_limit(int);
    static  Channel&                    getChannel(const std::string& name); // Kanalı almak için
    static  bool                        findChannel(const std::string &name); // kanalı bulmak için;
    static  Channel&                    createChannel(const std::string& name); // Yeni kanal oluşturmak için
};

#endif
