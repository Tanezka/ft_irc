#ifndef MESSAGEHANDLER_HPP
#define MESSAGEHANDLER_HPP

#include "Channel.hpp"
#include "Line.hpp"
#include "Defines.hpp"
#include <cstdlib> //atoi
#include <map>
#include <iostream>
#include <sstream>

struct Message;
class Client;
class Channel;
class Line;
class Server;

class MessageHandler
{
private:
    static Server* _server;
    static  std::map<unsigned int, Client> &clientlist;
    static  std::map<std::string, void(*)(int, Line)> m_funcs; // Komut fonksiyonları haritası
    static std::map<std::string, Channel> _channels; // Mevcut kanalları saklamak için

public:
    static void listClient(); // listele
    static void handleData(int ,std::string); // Gelen veriyi işleme
    static void handleMessage(int, Line); // Mesaj işleme
    static void handleJoin(int, Line); // JOIN komutunu işleme
    static void handleNick(int, Line); // NICK komutunu işleme
    static void handleUser(int, Line); // USER komutunu işleme
	static void handleSingle(int fd, Line patates); // Mesajı tekil hale getirip işleme
    static void handleWho (int, Line);
    static void handleMode(int, Line);
    static void handleQuit(int, Line);
    static void handleCap(int, Line);
    static void handlePass(int, Line);
    static void handlePart(int, Line);
    static void handleBan(int, Line);
    static void handleInvite(int, Line);
    static void handleKick(int, Line);
    static void init(Server& server); // Fonksiyonları başlatma
	static Line convertLine(std::string buf); // Raw satır Line objesi haline gelir
    static void sendReply(int fd, const std::string& reply);

    static bool isValidNickname(const std::string& nickname);
    static int  findFdByNickname(const std::string& nickname);
};

#endif
