#include "ConfigFileParser.hpp"
#include "VirtualHostServer.hpp"
#include "Reactor.hpp"
#include "ServerConfig.hpp"
#include "HTTPHeader.hpp"
#include "Parse.hpp"
#include "HTTPMethod.hpp"

int startServer (const Parse & cfg) {
    // Create an Reactor to dispatch events
    Reactor *reactor = Reactor::getInstance();

	std::vector<VirtualHostServer> virtualHostServers;

	// Starts all servers in the config file
	const std::vector<ServerConfig> & serverConfigs = cfg.getParsedCfgs();
	for(std::vector<ServerConfig>::const_iterator it = serverConfigs.begin(); it != serverConfigs.end(); ++it) {
		const ServerConfig & config = *it;
		VirtualHostServer virtualHostServer(*reactor, config);
		virtualHostServers.push_back(virtualHostServer);
		// As well as start litening the VirtualHostServer registers its socket with the Reactor.
		virtualHostServers.back().start();
	}

    // Run the event loop
    reactor->runEventLoop();

	// Stops listening for new connections. Closes sockets
	for(std::vector<VirtualHostServer>::iterator it = virtualHostServers.begin(); it != virtualHostServers.end(); ++it) {
		VirtualHostServer & virtualHostServer = *it;
		virtualHostServer.stop();
	}

	delete reactor;
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
			Parse parse;
			parse.setFullCfg(configFile);
			startServer(parse);
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
	return (0);
}
