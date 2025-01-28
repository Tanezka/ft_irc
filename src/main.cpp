#include "../include/Server.hpp"

int main(int argc, char **argv)
{
    if(argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }

    int port = std::atoi(argv[1]);
	
    if (port <= 0 || port > 65535) 
    {
		std::cout << "Invalid port" << std::endl;
		return 1;
	}

    Server server(port,argv[2],"E.T");
    server.initServ();
    MessageHandler::init(server);
    server.run();
}