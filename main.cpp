#include "ConfigFileParser.hpp"
#include "VirtualHostServer.hpp"
#include "Reactor.hpp"
#include "ServerConfig.hpp"
#include "HTTPHeader.hpp"
#include "LocationParse.hpp"
#include "HTTPMethod.hpp"

int startServer (void) {
    // Create an Reactor to dispatch events
    Reactor *reactor = Reactor::getInstance();

    // Create an VirtualHostServer to listen for incoming connections on a specific port
    // We will have one socket listening for new connections for this server
    // Pass the VirtualHostConfig class here
	ServerConfig serverConfig1;
	serverConfig1.setPort(8080);

	// Example location with an allowed http method
	LocationConfig location1;
	// TODO: Once ready ... location1.addAllowedMethod(HTTP_METHOD_GET);
	serverConfig1.setLocation(location1);
    VirtualHostServer virtualHostServer1(*reactor, serverConfig1);
    // As well as start litening the VirtualHostServer registers its socket with the Reactor.
    virtualHostServer1.listen();

    // Create a second VirtualHostServer on a different port 8081
    // We will have another socket listening for new connections for this other server
    // Pass the VirtualHostConfig class here
    // VirtualHostServer virtualHostServer2(reactor, 8081);
    // Also starts litening and registers its socket with the Reactor.
    // virtualHostServer2.listen();

    // This VirtualHostServer creation should be really a loop, driven from the 
    // number of Server blocks in the config file

    // Run the event loop
    // Reactor event loop uses select to detect sockets with data, and dispatches 
    // the request to the right VirtualHostServer
    reactor->runEventLoop();

	// Stops listening for new connections. Closes sockets
	virtualHostServer1.stop();
	delete reactor; // Careful if program exit and don't get here!!!!!!
	return (0);
}

int main(int argc, char **argv){
	if (argc <= 2){
		try{
			std::string configFile;
			if (argc == 2){
				configFile=argv[1];
			}
			else{
				configFile=("./httpd.conf");
			}
			// ## PARSE INTEGRATION IN PROGRESS ##
			LocationParse parse;
			parse.checkFile(configFile);
			parse.getNextLocation();
			parse.getParsedLocations();
		}
		catch (std::exception &e){
			std::cerr << e.what() << std::endl;
			return (1);
		}	
	}
	else {
		std::cout << "ERROR: Wrong parameters." << std::endl;
		return (1);
	}
	return (startServer());
}
