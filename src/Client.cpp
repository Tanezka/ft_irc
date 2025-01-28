#include "../include/Client.hpp"

Client::Client(){}

Client::Client(int socket, const std::string& ip): _clientSocketFd(socket), _clientIp(ip), _nickname("test_nick"), _username("test_user"), _passAccepted(false), _hasNick(false), _hasUser(false){}

Client::~Client(){}

int Client::getFd() const {return _clientSocketFd;}
const std::string& Client::getNickname () const {return _nickname;}
const std::string& Client::getRealname () const {return _realname;}
const std::string& Client::getUsername () const {return _username;}
const std::string& Client::getClientIp () const {return _clientIp;}
const std::string& Client::getChannel () const {return _channel;}
const std::string& Client::getPassword () const {return _pass;}

bool Client::getPassAccepted() const {return _passAccepted;}
bool Client::getHasNick() const {return _hasNick;}
bool Client::getHasUser() const {return _hasUser;}
bool Client::isAuthenticated() const { return _hasUser && _passAccepted && _hasNick; }

void Client::setNickname(const std::string& nickname){_nickname = nickname;}
void Client::setRealname(const std::string& realname){_realname = realname;}
void Client::setUsername(const std::string& username){_username = username;}
void Client::setClientIp(const std::string& ip){_clientIp = ip;}
void Client::setChannelname(const std::string& channel){_channel = channel;}
void Client::setPassword(const std::string& pass){_pass = pass;}
void Client::setPassAccepted(bool auth) { _passAccepted = auth; }
void Client::setHasNick(bool auth) { _hasNick = auth; }
void Client::setHasUser(bool auth) { _hasUser = auth; }