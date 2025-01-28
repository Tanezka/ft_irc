#ifndef LINE_HPP
#define LINE_HPP

#include <iostream>

struct Message;
class Client;
class Channel;

class Line
{
private:
	std::string	prefix;
	std::string	command;
	std::string	parameter;
	std::string	trailing;
	std::string rawLine;

public:

	std::string	getPrefix();
	std::string	getCommand();
	std::string	getParameter();
	std::string	getTrailing();
	std::string	getRaw();
	void		setPrefix(std::string);
	void		setCommand(std::string);
	void		setParameter(std::string);
	void		setTrailing(std::string);
	void		setRaw(std::string);
};

#endif
