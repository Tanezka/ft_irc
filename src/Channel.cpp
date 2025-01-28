#include "../include/Channel.hpp"
#include "Channel.hpp"

// Statik değişkeni tanımlayın
std::map<std::string, Channel> Channel::_channels;
std::map<unsigned int, Client> &Channel::c_list = Server::getClientList();

Channel::Channel() {}

Channel::Channel(const std::string& name) : _name(name), is_inviteonly(false), is_moderated(false), is_keyprotected(false), has_limit(false), no_extern(false) {}

void Channel::addClient(int clientFd) 
{
    std::string         msg;

    rights[clientFd].set_fd(clientFd);
    if (_clients.empty())
        rights[clientFd].set_admin(true);
    rights[clientFd].set_here(true);
    _clients.push_back(clientFd);
    std::string nickname = c_list[clientFd].getNickname();
    std::cout << "Client " << nickname << " has been added to the channel: " << _name << std::endl;
    msg = ":" + nickname + "!" + c_list[clientFd].getUsername() + " JOIN " + _name + "\r\n";
    broadcast(msg);
}

void Channel::removeClient(int clientFd) 
{
    std::map<int, Rights>::iterator     it;
    _clients.erase(std::remove(_clients.begin(), _clients.end(), clientFd), _clients.end());
    it = rights.find(clientFd);
    rights.erase(it);
    rights[_clients.front()].set_admin(true);
    broadcast(":irc_serv MODE " + _name + " +o " + c_list[_clients.front()].getNickname() + "\r\n");
    std::cout << "Client " << clientFd << " removed from channel " << _name << std::endl;
}

void Channel::broadcast(std::string msg) 
{
    std::vector<int>::iterator  it;

    std::cout << "Sending to client: " << msg.c_str() <<std::endl;
    it = _clients.begin();
    while (it != _clients.end())
    {
        send(*it, msg.c_str(), msg.size(), 0);
        it++;
    }
}

void Channel::broadcast_butself(int fd, std::string msg)
{
    std::vector<int>::iterator  it;

    std::cout << "Sending to client: " << msg.c_str() <<std::endl;
    it = _clients.begin();
    while (it != _clients.end())
    {
        if (fd == *it)
        {
            it++;
            continue;
        }
        send(*it, msg.c_str(), msg.size(), 0);
        it++;
    }
}

const std::string& Channel::getName() const {
    return _name;
}

const std::vector<int>& Channel::getClients() const {return _clients;}

void Channel::listClients() const 
{
    std::vector<int>::const_iterator it;

    std::cout << _name  << "'s client list:\n" << std::flush;
    it = _clients.begin();
    while (it != _clients.end())
    {
        std::cout << c_list[*it].getNickname() << "\n" << std::flush;
        it++; 
    }
}

Channel& Channel::getChannel(const std::string& name) 
{
    return _channels[name]; // Mevcut kanalı döndür
}

bool   Channel::findChannel(const std::string &name)
{
    std::map<std::string, Channel>::iterator it;

    it = _channels.find(name); 
    if (it != _channels.end())
        return true;
    return false;
}

const std::string   Channel::getClientStatus(int fd)
{
    return (rights[fd].get_status());
}

Rights&        Channel::get_right(int fd)
{
    return (rights[fd]);
}

const std::string   Channel::getChannelStatus(int fd)
{
    std::string     stat = "+";
    std::string     part2;
    std::stringstream         ss;

    if (is_inviteonly)
        stat.append("i ");
    if (is_moderated)
        stat.append("m ");
    if (no_extern)
        stat.append("n ");
    if (has_limit)
    {
        ss << user_limit;
        stat.append("l ");
        part2.append(" ");
        part2.append(ss.str());
    }
    
    if (is_keyprotected)
    {
        stat.append("k ");
        part2.append(" ");
        part2.append(password);
    }   

    if (stat.size() == 1)
        stat.clear();
    stat.append(part2);
    reply_ops(fd);
    std::cout << "Channel stats: (" << stat << ")" << std::endl;
    return (stat);
}

void    Channel::reply_ops(int fd)
{
    std::string     rply = ":irc_serv 324 " + c_list[fd].getNickname() + " " + _name + " +o ";
    std::string     fstr;
    std::map<int, Rights>::iterator it;

    it = rights.begin();
    while(it != rights.end())
    {
        fstr.clear();
        if (it->second.get_admin())
        {
            fstr.append(rply);
            fstr.append(c_list[it->second.get_fd()].getNickname());
            fstr.append("\r\n");
            std::cout << "fstr: (" << fstr << ")" << std::endl;
            MessageHandler::sendReply(fd, fstr);
        }
        it++;
    }
}


Channel &Channel::createChannel(const std::string& name) {
    Channel newChannel(name);
    _channels[name] = newChannel;
    std::cout << "Channel created: " << name << std::endl;
    return _channels[name];
}

const std::vector<std::string>& Channel::getBanList() const {return _banList;}

void Channel::banUser(std::string& nickname)
{
    _banList.push_back(nickname);
}

bool Channel::get_inChannel(int fd)
{
    std::vector<int>::iterator it;
    it = _clients.begin();
    while(it != _clients.end())
    {
        if(fd == *it)
            return true;
        it++;
    }
    return false;
}

bool&    Channel::getInviteOnly() {return is_inviteonly;}
bool&    Channel::getIsModerated() {return is_moderated;}
void    Channel::set_invite_only(bool b){is_inviteonly = b;}
void    Channel::set_moderated(bool b){is_moderated = b;}
void    Channel::set_keyprotected(bool b){is_keyprotected = b;}
void    Channel::set_has_limit(bool b){has_limit = b ;}
void    Channel::set_extern(bool b){no_extern = b;}
void    Channel::set_limit(int b){user_limit = b;}
void    Channel::set_password(std::string b){password = b;}

bool&    Channel::get_invite_only(){return (is_inviteonly);}
bool&    Channel::get_moderated(){return (is_moderated);}
bool&    Channel::get_keyprotected(){return (is_keyprotected);}
bool&    Channel::get_has_limit(){return (has_limit);}
bool&    Channel::get_extern(){return (no_extern);}
unsigned    int     Channel::get_limit(){return (user_limit);}
std::string    Channel::get_password(){return (password);}