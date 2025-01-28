#include "../include/MessageHandler.hpp"

std::map<std::string, void(*)(int, Line)> MessageHandler::m_funcs;
std::map<unsigned int, Client> &MessageHandler::clientlist = Server::getClientList();
Server* MessageHandler::_server = NULL;

void MessageHandler::handleSingle(int fd, Line line)
{
	std::string		command = line.getCommand();
	if (command == "LIST\r\n")
		MessageHandler::listClient();
    if (m_funcs.find(command) != m_funcs.end()) 
        m_funcs[command](fd, line);
    else
        std::cout << "Command not found: " << command << std::endl;
}

Line MessageHandler::convertLine(std::string buf)
{
	Line					line;
	size_t	                it = 0;
	size_t	                it2;

	if (buf[0] == ':')
	{
		it2 = buf.find(" ", it);
		line.setPrefix(buf.substr(it, it2 - it));
		it = it2 + 1;
	}
	
	it2 = buf.find(" ", it);
	line.setCommand(buf.substr(it, it2 - it));
	it = it2 + 1;

	if ((it2 = buf.find(":", it)) != std::string::npos)
	{
		line.setParameter(buf.substr(it, it2 - it - 1));
		it = it2 + 1;
		it2++;
		line.setTrailing(buf.substr(it2, buf.length() - (it2 - 1)));
	}
	else
		line.setParameter(buf.substr(it, buf.size() - it));  // Set parameter
	line.setRaw(buf);
	return line;
}

void MessageHandler::handleData(int fd, std::string buf)
{
    std::string				raw;
	size_t	                begin = 0;
	size_t                  end = begin;

	while ((end = buf.find("\r\n", begin) ) != std::string::npos)
	{
		raw = buf.substr(begin, end - begin);
        std::cout << "Raw: (" << raw << ")" << std::endl;
		MessageHandler::handleSingle(fd, MessageHandler::convertLine(raw));
		begin = end + 2;
	}
}
void MessageHandler::handleMessage(int fd, Line buf) 
{
    if(clientlist[fd].isAuthenticated() == false)
    {
        sendReply(fd, "You are not authenticated!!\r\n");
        return;
    }
    std::string channelName = buf.getParameter();
    std::string nickname = clientlist[fd].getNickname();

    if (channelName[0] == '#') 
    {
        if (Channel::findChannel(channelName))
        {
            
            Channel& channel = Channel::getChannel(channelName);
            if(channel.getIsModerated())
            {
                if(!channel.get_right(fd).get_voice() && !channel.get_right(fd).get_admin())
                {
                    sendReply(fd, ERR_CANNOTSENDTOCHAN(clientlist[fd].getNickname(), channelName, "m"));
                    return;   
                }
            }
            if ((channel.get_extern()) && (clientlist[fd].getChannel() != channelName))
            {
                sendReply(fd, ERR_CANNOTSENDTOCHAN(clientlist[fd].getNickname(), channelName, "n"));
                return;
            }
            if (buf.getTrailing().empty())
            {
                sendReply(fd, ERR_NEEDMOREPARAMS(clientlist[fd].getNickname(), "PRIVMSG"));
                return;
            }
            std::string msg = ":" + nickname + "!" + clientlist[fd].getUsername() + " PRIVMSG " + buf.getParameter() + " :" + buf.getTrailing() + "\r\n";
            channel.broadcast_butself(fd, msg);
        }
        else
        {
            sendReply(fd, ERR_NOSUCHCHANNEL(nickname, channelName));
            return;
        }
    } 
    else {
        std::map<unsigned int, Client>::iterator userIt = clientlist.begin();
        bool userFound = false;

        for (; userIt != clientlist.end(); ++userIt) 
        {
            if (userIt->second.getNickname() == channelName) 
            {
                userFound = true;
                break;
            }
        }

        if (!userFound) 
        {
            sendReply(fd, ERR_NOSUCHNICK(nickname, channelName));
            return;
        }
        std::string msg = ":" + nickname + "!" + clientlist[fd].getUsername() + " PRIVMSG " + channelName + " :" + buf.getTrailing() + "\r\n";
        send(userIt->first, msg.c_str(), msg.size(), 0);
    }
}

void MessageHandler::handleJoin(int fd, Line buf)
{
    if(clientlist[fd].isAuthenticated() == false)
    {
        sendReply(fd, "You are not authenticated!!\r\n");
        return;
    }

	std::istringstream iss(buf.getParameter());
    std::string channelName, password;
    iss >> channelName;
    if (iss)
    {
        iss >> password;
    }

	if (channelName[0] != '#')
	{
    	MessageHandler::sendReply(fd, ERR_NOSUCHCHANNEL(clientlist[fd].getNickname(), buf.getParameter()));
		return ;
	}
    std::vector<int>            channelClientList;
    std::vector<std::string>    banList;
    std::vector<std::string>::iterator    it2;
    std::vector<int>::iterator  it;
    std::string                 names;
    std::string                 reply;

    if (Channel::findChannel(channelName))
    {
        Channel &channel = Channel::getChannel(channelName);
        if(channel.get_inChannel(fd))
        {
            reply = ":irc_serv NOTICE " + clientlist[fd].getNickname() + " :You are already in channel!!!!!!.\r\n";
			sendReply(fd, reply);
            return;
        }
        banList = channel.getBanList();
        it2 = banList.begin();
        while (it2 != banList.end())
        {
            if (*it2 == clientlist[fd].getNickname())
            {
				sendReply(fd, "465 " + clientlist[fd].getNickname() + " " + channel.getName() + " :Cannot join channel (+b)\r\n");
                return;
            }
            it2++;
        }
        if(channel.getInviteOnly())
        {
            if(!channel.get_right(fd).get_invited())
            {
                sendReply(fd, ERR_INVITEONLYCHAN(clientlist[fd].getNickname(), channelName));
                return;
            }
        }
		
		if (channel.get_keyprotected())
		{
			if (password != channel.get_password())
			{
				sendReply(fd, "464 " + clientlist[fd].getNickname() + " " + channel.getName() + " :Cannot join channel, no password or password mismatch (+k)\r\n");
                return;
			}
		}

		if (channel.get_has_limit())
		{
			if (channel.getClients().size() >= channel.get_limit())
			{
				sendReply(fd, "471 " + clientlist[fd].getNickname() + " " + channel.getName() + " :Cannot join channel, channel is full. (+l)\r\n");
				return;
			}
		}
        channel.addClient(fd);
        channelClientList = channel.getClients();
    }
    else
    {
        Channel &channel = Channel::createChannel(channelName);
        channel.addClient(fd);
        channelClientList = channel.getClients();
    }

    it = channelClientList.begin();
    while (it != channelClientList.end())
    {
        names.append(clientlist[*it].getNickname());
        it++;
        if (it != channelClientList.end())
            names.append(" ");
    }
    sendReply(fd, RPL_NAMREPLY(clientlist[fd].getNickname(), channelName, names));
    sendReply(fd, RPL_ENDOFNAMES(clientlist[fd].getNickname(), channelName));
    clientlist[fd].setChannelname(channelName);
}

void MessageHandler::handleNick(int fd, Line buf) {
    std::string oldNick = clientlist[fd].getNickname();
    std::string newNick = buf.getParameter();

    if (newNick.empty()) {
        sendReply(fd, ERR_NEEDMOREPARAMS(oldNick, "NICK"));
        return;
    }

    if (!isValidNickname(newNick)) {
        sendReply(fd, ERR_ERRONEUSNICKNAME(oldNick, newNick));
        return;
    }

    for (std::map<unsigned int, Client>::iterator it = clientlist.begin(); it != clientlist.end(); ++it) {
        if (it->second.getNickname() == newNick) {
            sendReply(fd, ERR_NICKNAMEINUSE(oldNick, newNick));
            return;
        }
    }

    if (newNick == "test-nick")
    {
        sendReply(fd, ":irc_serv NOTICE " + oldNick + " :That nickname is invalid\r\n");
        return;
    }
    clientlist[fd].setNickname(newNick);
    clientlist[fd].setHasNick(true);
    
    std::string msg = ":" + oldNick + " NICK " + newNick + "\r\n";

    for (std::map<unsigned int, Client>::iterator it = clientlist.begin(); it != clientlist.end(); ++it) {
        if (it->first != static_cast<unsigned int>(fd)) {
            send(it->first, msg.c_str(), msg.size(), 0);
        }
    }

    std::string successReply = ":" + oldNick + " NICK " + newNick + "\r\n";
    //send(fd, successReply.c_str(), successReply.size(), 0);
    std::cout << oldNick << " has changed their nickname to " << newNick << std::endl;
}

void MessageHandler::handleUser(int fd, Line buf) 
{
    std::string username = buf.getParameter();
    std::string nickname = clientlist[fd].getNickname();
    int         it;

    if (username.empty()) {
        sendReply(fd, ERR_NEEDMOREPARAMS(nickname, "USER"));
        return;
    }
    for (std::map<unsigned int, Client>::iterator it = clientlist.begin(); it != clientlist.end(); ++it) {
        if (it->second.getUsername() == username) {
            sendReply(fd, ERR_NICKNAMEINUSE(nickname, username));
            return;
        }
    }
    it = username.find(' ');
    clientlist[fd].setUsername(username.substr(0, it));
    clientlist[fd].setHasUser(true);
    MessageHandler::sendReply(fd, RPL_WELCOME(_server->getHost(), clientlist[fd].getNickname(), buf.getPrefix()));
    std::cout << "User changed username to " << buf.getParameter() << std::endl;
}

void MessageHandler::handleWho(int fd, Line buf)
{
    std::vector<std::string>    array;
    std::string         parameter = buf.getParameter();
    size_t              begin = 0;
    size_t              end = 0;
    if(clientlist[fd].isAuthenticated() == false)
    {
        sendReply(fd, "You are not authenticated!!\r\n");
        return;
    }
    std::string                 reply;
	while ((end = parameter.find(' ', begin) ) != std::string::npos)
	{
		array.push_back(parameter.substr(begin, end - begin));
        std::cout << "Word: (" << array.back() << ")" << std::endl;
		begin = end + 1;
	}
    if (array.empty())
        array.push_back(parameter);
    if (buf.getParameter()[0] == '#')
    {
        if (Channel::findChannel(array.front()) == false)
        {
    	    MessageHandler::sendReply(fd, ERR_NOSUCHCHANNEL(clientlist[fd].getNickname(), buf.getParameter()));
            return;
        }
        Channel& channel = Channel::getChannel(array.front());
        std::vector<int> _clients = channel.getClients();
        std::vector<int>::iterator  it;
        it = _clients.begin();
        while (it != _clients.end())
        {
            reply = RPL_WHOREPLY(clientlist[fd].getNickname(), channel.getName(), clientlist[*it].getUsername(), clientlist[*it].getClientIp(), "irc_serv", clientlist[*it].getNickname(), channel.getClientStatus(fd), "0", clientlist[*it].getRealname()); //status hop count ve host alınmalı şuan geçici dolduruluyor
            sendReply(fd, reply);
            it++;
        }
        sendReply(fd, RPL_ENDOFWHO(clientlist[fd].getNickname(), channel.getName()));
    }
}

void MessageHandler::handleMode(int fd, Line buf)
{
    if (clientlist[fd].isAuthenticated() == false)
    {
        sendReply(fd, "You are not authenticated!!\r\n");
        return;
    }

    std::string reply;
    std::string parameter = buf.getParameter();

    if (parameter.empty() || parameter[0] != '#')
    {
        sendReply(fd, ERR_NOSUCHCHANNEL(clientlist[fd].getNickname(), buf.getParameter()));
        return;
    }

    std::istringstream iss(parameter);
    std::string channelName,modes;
    iss >> channelName >> modes;
    std::vector<std::string> users;
    std::string user;
    if (!Channel::findChannel(channelName))
    {
        sendReply(fd, ERR_NOSUCHCHANNEL(clientlist[fd].getNickname(), buf.getParameter()));
        return;
    }
    
    while(iss >> user)
        users.push_back(user);
    Channel &channel = Channel::getChannel(channelName);
    std::string modeReply;

    if(channel.get_inChannel(fd))
    {
        for (size_t i = 0; i < modes.size(); ++i)
        {
            char mode = modes[i];
            bool isAdding = (mode == '+');
            bool isRemoving = (mode == '-');

	    	if (channel.get_right(fd).get_admin() == false)
	    	{
	    		reply = ":irc_serv NOTICE " + clientlist[fd].getNickname() + " :You do not have the necessary privileges to perform that action.\r\n";
	    		sendReply(fd, reply);
	    		return;
	    	}

            // Modu belirle ve uygula
            if (isAdding || isRemoving)
            {
                i++; // Modu al
                if (i >= modes.size())
                    break; // Mod bitti
                char modeChar = modes[i];

                switch (modeChar)
                {
                    case 'i':
                        channel.set_invite_only(isAdding);
                        modeReply += (isAdding ? "+i " : "-i ");
                        break;
                    case 'm':
                        channel.set_moderated(isAdding);
                        modeReply += (isAdding ? "+m " : "-m ");
                        break;
                    case 'n':
                        channel.set_extern(isAdding);
                        modeReply += (isAdding ? "+n " : "-n ");
                        break;
                    case 'l':
                        channel.set_has_limit(isAdding);
                        if (isAdding && i < modes.size()) // Limit varsa
                        {
                            i++;
                            std::stringstream ss;
                            if (mode == '+')
                            {
                                channel.set_has_limit(true);
                                int limit = atoi(modes.substr(i, modes.find(' ', i) - i).c_str());
                                channel.set_limit(limit);
                                ss << limit;
                            }
                            else
                                channel.set_has_limit(false);
                            modeReply +=  std::string(&mode) + "l " + ss.str() + " ";
                        }
                        break;
                    case 'k':
                        channel.set_keyprotected(isAdding);
                        if (isAdding && i + 1 <= modes.size()) // Anahtar varsa
                        {
                            if (mode == '+')
                            {
                                channel.set_keyprotected(true);
                                channel.set_password(user);
                            }
                            else
                                channel.set_keyprotected(false);
                        }
                        break;
                    case 'o':
                        if (channel.get_right(fd).get_admin())
                        {
                            std::string                 sign;
                            std::vector<std::string>::iterator  it;
                            std::vector<int>::iterator  ite;
                            std::vector<int>            vec = channel.getClients();

                            it = users.begin();
                            ite = vec.begin();
                            while(ite != vec.end() && it != users.end())
                            {
                                if (clientlist[*ite].getNickname() == *it)
                                {
                                    reply = ":irc_serv 324 " + clientlist[fd].getNickname() + " " + channel.getName() + " " + mode + "o " + clientlist[*ite].getNickname() + "\r\n";
                                    if (mode == '+')
                                        channel.get_right(*ite).set_admin(true);
                                    else
                                        channel.get_right(*ite).set_admin(false);
                                    channel.broadcast(reply);
                                    it++;
                                    ite = vec.begin();
                                }
                                ite++;
                            }
                        }
                        break;
	    			case 'v':
                        if (channel.get_right(fd).get_admin())
                        {
                            std::string                 sign;
                            std::vector<std::string>::iterator  it;
                            std::vector<int>::iterator  ite;
                            std::vector<int>            vec = channel.getClients();

                            it = users.begin();
                            ite = vec.begin();
                            while(ite != vec.end() && it != users.end())
                            {
                                if (clientlist[*ite].getNickname() == *it)
                                {
                                    reply = ":irc_serv 324 " + clientlist[fd].getNickname() + " " + channel.getName() + " " + mode + "v " + clientlist[*ite].getNickname() + "\r\n";
                                    if (mode == '+')
                                        channel.get_right(*ite).set_voice(true);
                                    else
                                        channel.get_right(*ite).set_voice(false);                                
                                    channel.broadcast(reply);
                                    it++;
                                    ite = vec.begin();
                                }
                                ite++;
                            }
                        }
                        break;				
                    default:
                        sendReply(fd, ERR_UNKNOWNMODE(clientlist[fd].getNickname(), std::string(1, modeChar)));
                        return;
                }
            }
        }
    }
	if (modes.empty())
	{
    	reply = ":irc_serv 324 " + clientlist[fd].getNickname() + " " + channel.getName() + " " + channel.getChannelStatus(fd) + "\r\n";// +imn +o tanez -o eray
    	sendReply(fd, reply);
	}
}


int MessageHandler::findFdByNickname(const std::string &nickname)
{
    for (std::map<unsigned int, Client>::iterator it = clientlist.begin(); it != clientlist.end(); ++it)
    {
        if (it->second.getNickname() == nickname)
        {
            return it->first;
        }
    }
    return -1;
}

void MessageHandler::handleQuit(int fd, Line line) {
    std::string quitMessage = line.getTrailing();
    std::string nickname = clientlist[fd].getNickname();
    std::string username = clientlist[fd].getUsername();
    std::string hostname = _server->getHost();

    std::string msg = ":" + nickname + "!" + username + "@" + hostname + " QUIT :Quit: " + quitMessage + "\r\n";

    std::string channelName = clientlist[fd].getChannel();
    if (!channelName.empty()) {
        Channel& channel = Channel::getChannel(channelName);
        std::string msg2 = ":" + nickname + "!" + username + " PART " + channelName; // bug olabilir
        msg2 += " :" + quitMessage;
        msg2 += "\r\n";
        channel.removeClient(fd);
        channel.broadcast_butself(fd, msg2);
    }
    
    for (std::map<unsigned int, Client>::iterator it = clientlist.begin(); it != clientlist.end(); ++it) {
        if (it->first != static_cast<unsigned int>(fd)) {
            send(it->first, msg.c_str(), msg.size(), 0);
        }
    }
    
    clientlist.erase(fd);
    _server->getPollHandler().removeFd(fd);
    close(fd);
    std::cout << nickname << " has disconnected." << std::endl;
}

void MessageHandler::handleCap(int fd, Line line)
{
    std::string command = line.getParameter();

    if (command == "LS") 
    {
        std::string capabilities = "multi-prefix";

        std::string response = ":irc_serv CAP * LS :" + capabilities + "\r\n";
        sendReply(fd, response);
    } else if (command == "REQ") {
        std::string requestedCapability = line.getTrailing();
        
        if (requestedCapability == "multi-prefix") 
        {
            std::string response = ":irc_serv CAP " + clientlist[fd].getNickname() + " ACK " + requestedCapability + "\r\n";
            sendReply(fd, response);
        } else 
        {
            std::string response = ":irc_serv CAP " + clientlist[fd].getNickname() + " NAK " + requestedCapability + "\r\n";
            sendReply(fd, response);
        }
    } else if (command == "END") 
    {
        Client& client = clientlist[fd];

        std::cout << "Client " << client.getNickname() << " has ended CAP negotiation." << std::endl;
        
        //client.setIsCapNegotiated(true);
    } else 
    {
        std::cout << "Unknown CAP command: " << command << std::endl;
    }
}

void MessageHandler::handlePass(int fd, Line line) 
{
    if(clientlist[fd].getPassAccepted() == true)
        return;
    std::string password = line.getParameter();
    std::string serverPassword = _server->getPass();

    if (password == serverPassword) {
        clientlist[fd].setPassAccepted(true);
        std::cout << clientlist[fd].getNickname() << " has successfully authenticated." << std::endl;
    } else {
        // Password is incorrect
        std::string errorMessage = ERR_PASSWDMISMATCH(clientlist[fd].getNickname());
        sendReply(fd, errorMessage);
        close(fd);
        _server->getPollHandler().removeFd(fd);
        clientlist.erase(fd);
        std::cout << "Incorrect password for " << clientlist[fd].getNickname() << ". Connection closed." << std::endl;
    }
}

void MessageHandler::handlePart(int fd, Line line) 
{    
    if (!clientlist[fd].isAuthenticated()) {
        sendReply(fd, "You are not authenticated!!\r\n");
        return;
    }
    std::string channelName = line.getParameter();
    std::string nickname = clientlist[fd].getNickname();
    std::string username = clientlist[fd].getUsername();
    std::string quitMessage = line.getTrailing();

    if (channelName.empty() || channelName[0] != '#') {
        sendReply(fd, ERR_NOSUCHCHANNEL(nickname, channelName));
        return;
    }

    if (!Channel::findChannel(channelName)) {
        sendReply(fd, ERR_NOSUCHCHANNEL(nickname, channelName));
        return;
    }
    Channel& channel = Channel::getChannel(channelName);
    std::string msg = ":" + nickname + "!" + username + " PART " + channelName;
    if (!quitMessage.empty()) {
        msg += " :" + quitMessage; 
    }
    msg += "\r\n";

    channel.removeClient(fd);
    channel.broadcast_butself(fd, msg);
    clientlist[fd].setChannelname("");
    sendReply(fd, msg); 
}

void MessageHandler::handleBan(int fd, Line line)
{
    if (!clientlist[fd].isAuthenticated()) {
        sendReply(fd, "You are not authenticated!!\r\n");
        return;
    }

    std::string channelName = line.getParameter();
    std::string targetNick = line.getTrailing();
    std::string nickname = clientlist[fd].getNickname();

    if (channelName.empty() || channelName[0] != '#') {
        sendReply(fd, ERR_NOSUCHCHANNEL(nickname, channelName));
        return;
    }

    if (!Channel::findChannel(channelName)) {
        sendReply(fd, ERR_NOSUCHCHANNEL(nickname, channelName));
        return;
    }

    Channel& channel = Channel::getChannel(channelName);

    if (channel.get_right(fd).get_admin()) 
    {
        sendReply(fd, ERR_CHANOPRIVSNEEDED(nickname, channelName));
        return;
    }

    
    channel.banUser(targetNick);

    std::string msg = ":" + nickname + "!" + clientlist[fd].getUsername() + " BAN " + channelName + " :" + targetNick + "\r\n";
    channel.broadcast(msg);

    sendReply(fd, "Banned " + targetNick + " from " + channelName + "\r\n");
}

void MessageHandler::handleInvite(int fd, Line buf)
{
    int                     fd_target;
    std::istringstream iss(buf.getParameter());
    std::string invited_nick, channelName, dump;
    iss >> invited_nick >> channelName >> dump;
    if (dump.size() > 0)
    {
        std::cout << "too many args" << std::endl;
        return ;
    }
    if (Channel::findChannel(channelName))
    {
        Channel &channel = Channel::getChannel(channelName);
        fd_target = findFdByNickname(invited_nick);
        if (fd_target == -1)
        {
            std::cout << fd  << " Target user does not exist" << std::endl;
            return ;
        }
        Rights  &right = channel.get_right(fd_target);
        right.set_invited(true);
        std::cout << clientlist[fd_target].getNickname() << " is invited" << std::endl;
    }
}

void MessageHandler::handleKick(int fd, Line buf)
{
    std::istringstream iss(buf.getParameter());
    int         target_fd;
    std::string channelName, user, reason, dump;
    iss >> channelName >> user >> reason >> dump;

    if (Channel::findChannel(channelName))
    {
        Channel &channel = Channel::getChannel(channelName);
        if (channel.get_right(fd).get_admin() == false)
        {
			sendReply(fd, ":irc_serv NOTICE " + clientlist[fd].getNickname() + " :You do not have the necessary privileges to perform that action.\r\n");
			return;                        
        }
        target_fd = findFdByNickname(user);
        if (target_fd != -1)
        {
            const std::vector<int>& vec = channel.getClients();
            std::vector<int>::const_iterator  it = vec.begin();
            while (it != vec.end())
            {
                if (*it == target_fd)
                {
                    channel.removeClient(target_fd);
                    channel.broadcast(":" + clientlist[fd].getNickname() + " KICK " + channel.getName() + " " + clientlist[target_fd].getNickname() + " :" + reason + dump + "\r\n");
                    sendReply(target_fd, ":" + clientlist[fd].getNickname() + " KICK " + channel.getName() + " " + clientlist[target_fd].getNickname() + " :" + reason + dump + "\r\n");
                    clientlist[target_fd].setChannelname("");
                    return;
                }
                it++;
            }
        }
    }

}

void MessageHandler::listClient()
{
    std::map<unsigned int, Client>::iterator it = clientlist.begin();

    std::cout << "Client List:\n";
    while (it != clientlist.end())
    {
        std::cout << it->second.getNickname() << "\n";
        it++;
    }
}

void MessageHandler::sendReply(int fd, const std::string& reply) {

    send(fd, reply.c_str(), reply.size(), 0);  
}


void MessageHandler::init(Server& server)
{
	_server = &server;
    m_funcs["JOIN"] = MessageHandler::handleJoin;
    m_funcs["USER"] = MessageHandler::handleUser;
    m_funcs["NICK"] = MessageHandler::handleNick;
    m_funcs["PRIVMSG"] = MessageHandler::handleMessage;
    m_funcs["WHO"] = MessageHandler::handleWho;
    m_funcs["MODE"] = MessageHandler::handleMode;
    m_funcs["QUIT"] = MessageHandler::handleQuit;
    m_funcs["CAP"] = MessageHandler::handleCap;
    m_funcs["PASS"] = MessageHandler::handlePass;
    m_funcs["PART"] = MessageHandler::handlePart;
    m_funcs["INVITE"] = MessageHandler::handleInvite;
    m_funcs["KICK"] = MessageHandler::handleKick;
}

bool MessageHandler::isValidNickname(const std::string& nickname) {
    if (nickname.empty() || nickname.length() > 20) return false;
    for (size_t i = 0; i < nickname.length(); ++i) {
        char c = nickname[i];
        if (!(isalnum(c) || c == '-')) return false;
    }
    return true;
}