#include "../include/Line.hpp"

	std::string Line::getPrefix(){return prefix;}
	std::string Line::getCommand(){return command;}
	std::string Line::getParameter(){return parameter;}
	std::string Line::getTrailing(){return trailing;}
	std::string Line::getRaw(){return rawLine;}
	void		Line::setPrefix(std::string pre){prefix = pre;}
	void		Line::setCommand(std::string com){command = com;}
	void		Line::setParameter(std::string par){parameter = par;}
	void		Line::setTrailing(std::string traO){trailing = traO;}
	void		Line::setRaw(std::string r){rawLine = r;}