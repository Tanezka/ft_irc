#ifndef DEFINES_HPP
#define DEFINES_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <exception>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <ctime>

#define RPL_JOIN(source, channel) ":" + source + " JOIN :" + channel + "\r\n"
#define RPL_ENDOFNAMES(source, channel) ":irc_serv 366 " + source + " " + channel + " :End of /NAMES list.\r\n"
#define RPL_NAMREPLY(source, channel, nickList) ": 353 " + source + " = " + channel + " :" + nickList + "\r\n"
#define RPL_WELCOME(server, nick, prefix) ":" + server  + " 001 " + nick +" :Welcome to the Internet Relay Network " + prefix + "\r\n"
#define ERR_NOSUCHCHANNEL(source, channel) "403 " + source + " " + channel + " :No such channel" + "\r\n"
#define ERR_NOTONCHANNEL(source, channel) "442 " + source + " " + channel + " :You're not on that channel" + "\r\n"
#define RPL_NOTOPIC(source, channel) "331 " + source + " " + channel + " :No topic is set" + "\r\n"
#define RPL_TOPIC(source, channel, topic) "332 " + source + " " + channel + " " + topic + "\r\n"
#define ERR_NOSUCHNICK(source, nick) "401 " + source + " " + nick + " :No such nick" + "\r\n"
#define ERR_NOTEXTTOSEND(source) "412 " + source + " :No text to send" + "\r\n"
#define ERR_NORECIPIENT(source) "411     " + source + " :No" + "\r\n"
#define ERR_USERNOTINCHANNEL(source, nick, channel) "441 " + source + " " + nick + " " + channel + " :They aren't on that channel" + "\r\n"
#define RPL_KICK(source, channel, target, reason)   ":" + source + " KICK " + channel + " " + target + " :" + reason + "\r\n"
#define ERR_CHANOPRIVSNEEDED(source, channel) ": 482 " + source + " " + channel + " :You're not the channel operator" + "\r\n"
#define RPL_QUIT(source, message) " :" + source + " QUIT :Quit: " + message + "\r\n"
#define ERR_NOSUCHSERVER(source, server) "402 " + source + " " + server + " :No such server" + "\r\n"
#define FAILED_SOCKET_SEND "Failed to send socket" + "\r\n"
#define ERR_NEEDMOREPARAMS(source, command) "461 " + source + " " + command + " :Not enough parameters" + "\r\n"
#define ERR_INVALIDCAPCMD(source) "410 " + source + " :Invalid CAP subcommand" + "\r\n"
#define ERR_ALREADYREGISTERED(source) "462 " + source + " :You may not register" + "\r\n"
#define ERR_UNKNOWNMODE(nick, mode) "472 " + nick + " " + mode + " :is unknown mode char to me" + "\r\n"
#define ERR_INVITEONLYCHAN(nick, channel) "473 " + nick + " " + channel + " :Cannot join channel (+i)\r\n"
#define ERR_PASSWDMISMATCH(source) "464 " + source + " :Password mismatch" + "\r\n"
#define ERR_CANNOTSENDTOCHAN(nick, channel, key) ":irc_serv 404 " + nick + " " + channel + " :Cannot send to channel (+" + key + ")\r\n"
#define RPL_CHANNELMODEIS(source, channel, modes) ":irc_serv 324 " + source + " " + channel + " " + modes + "\r\n"




//NICK
#define ERR_NONICKNAMEGIVEN(source) "431 " + source + " :No nickname given" + "\r\n"
#define ERR_ERRONEUSNICKNAME(source, nick) "432 " + source + " " + nick + " :Erroneous nickname" + "\r\n"
#define ERR_NICKNAMEINUSE(source, nick) "433 " + source + " " + nick + " :Nickname is already in use" + "\r\n"

//CHANNEL
#define ERR_NOSUCHCHANNEL(source, channel) "403 " + source + " " + channel + " :No such channel" + "\r\n"
#define ERR_USERONCHANNEL(source, channel) "443 " + source + " " + channel + " :is already on channel" + "\r\n"
#define ERR_BADCHANNELKEY(source, channel) "475 " + source + " " + channel + " :Cannot join channel (+k)" + "\r\n"
#define ERR_CHANNELISFULL(source, channel) "471 " + source + " " + channel + " :Cannot join channel (+l)" + "\r\n"
#define ERR_NOTONCHANNEL(source, channel) "442 " + source + " " + channel + " :You're not on that channel" + "\r\n"
#define RPL_PART(prefix, channel, message) ":" + prefix + " PART " + channel + " :" + message + "\r\n"
#define RPL_LISTSTART(nick, numusers) ": 321 " + nick + " Channel : "+ numusers + "\r\n"
#define RPL_LIST(source, channel, userCount, topic) ": 322 " + source + " " + channel + " " + userCount + " " + topic + "\r\n"
#define RPL_LISTEND(source) "323 " + source + " :End of /LIST" "\r\n"
#define RPL_ENDOFWHO(reqnick, channel) ":irc_serv 315 " + reqnick + " " + channel + " :End of WHO list\r\n"
#define RPL_WHOREPLY(reqnick, channel, user, host, server, nick, status, hopcount, realname) ":irc_serv 352 " + reqnick + " " + channel + " " + user + " " + host + " " + server + " " + nick + " " + status + " :" + hopcount + " " + realname + "\r\n"
//:server_name 352 <requesting_nick> <channel> <user> <host> <server> <nick> <H|G>[*][@|+] :<hopcount> <realname>
#endif
