#include <signal.h>
#include <iostream>
#include <vector>
#include <unistd.h>

#include "HTTPRequestCGI.hpp"

HTTPRequestCGI::HTTPRequestCGI() {}

HTTPRequestCGI::~HTTPRequestCGI() {}

std::string HTTPRequestCGI::execCGI(std::string cgiScriptRelativePath, std::string queryString)
{
	// Build absolute path to the script we want to execute
	std::string cgiScriptAbsolutePath = getAbsolutePath(cgiScriptRelativePath);
	if (access(cgiScriptAbsolutePath.c_str(), X_OK) != 0) {
        throw std::runtime_error("CGI script does not exist or is not executable");
    }

	int pipefds[2];
	if (pipe(pipefds) == -1) {
		throw std::runtime_error("Error calling pipe");
	}

	pid_t pid = fork();
	if (pid == 0) {
		// We are at the forked process to run the cgi
		std::string cgiInterpreter = "python";
		std::string cgiInterpreterAbsolutePath = "/usr/bin/" + cgiInterpreter;
		
		char** args = createArgs(cgiInterpreter, cgiScriptAbsolutePath, queryString);
	    char** env = createEnvironment(cgiScriptAbsolutePath, queryString);

		// NOTE: If parameters are sent to the script via an HTTP POST request, they are passed to the script's standard input.

		close(pipefds[0]);
		dup2(pipefds[1], 1);
		dup2(pipefds[1], 2);
		close(pipefds[1]);
		execve(cgiInterpreterAbsolutePath.c_str(), args, env);
		throw std::runtime_error("Error calling execve");
		exit (1);
	} else {
		// We are at the main process waiting for child to finish. Controll timeouts here
		pid_t result;
		int status;
		result = waitpid(pid, &status, 0);
	}
    
    close(pipefds[1]);
	std::string output = readFromPipe(pipefds[0]);
	close(pipefds[0]);

	return (output);
}

std::string HTTPRequestCGI::getAbsolutePath(const std::string& relativePath) {
    char cwd[FILENAME_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        char absolutePath[FILENAME_MAX];
        if (snprintf(absolutePath, sizeof(absolutePath), "%s/%s", cwd, relativePath.c_str()) < sizeof(absolutePath)) {
            return std::string(absolutePath);
        } else {
	        throw std::runtime_error("Absolute path too long");
        }
    } else {
		throw std::runtime_error("Error calling getcwd");
    }
}

char** HTTPRequestCGI::createEnvironment(const std::string& cgiScriptAbsolutePath, const std::string& queryString) {
    std::vector<std::string> env;

	// In the RFC this is called meta-variables (Parameters passed to the CGI through the environment)
	// https://datatracker.ietf.org/doc/html/rfc3875.html#section-4.1

	// TODO: Double check how much of the RFC we really need to implement. Would make sense to start small and
	// add as we need?

	// Path to the cgi script to be executed
    env.push_back("PATH_INFO=" + cgiScriptAbsolutePath);
    env.push_back("SCRIPT_NAME=" + cgiScriptAbsolutePath);

	// The QUERY_STRING variable contains a URL-encoded search or parameter string
    env.push_back("QUERY_STRING=" + queryString);

	// The CONTENT_LENGTH variable contains the size of the message-body attached to the request
	// Guess if we have a POST (document upload, form, etc), that would be the lenght of the body POST?
    env.push_back("CONTENT_LENGTH=");

	// The SERVER_NAME variable MUST be set to the name of the server host to which the client request is directed.
    env.push_back("SERVER_NAME=localhost");

    env.push_back("GATEWAY_INTERFACE=CGI/1.1");
    env.push_back("SERVER_PROTOCOL=HTTP/1.0");

	// We have to set up the port in which we received this HTTP request from the client
	// 8080 harcoded for now
    env.push_back("SERVER_PORT=8080");

	return createCharPtrArray(env);
}

char** HTTPRequestCGI::createArgs(const std::string& cgiInterpreter, const std::string& cgiScriptAbsolutePath, const std::string& queryString) {
	std::vector<std::string> args;
    args.push_back(cgiInterpreter);
    args.push_back(cgiScriptAbsolutePath);
    args.push_back(queryString);

	return createCharPtrArray(args);
}

char** HTTPRequestCGI::createCharPtrArray(const std::vector<std::string>& strings) {
    char** result = new char*[strings.size() + 1];
    for (size_t i = 0; i < strings.size(); ++i) {
        result[i] = new char[strings[i].length() + 1];
        std::strcpy(result[i], strings[i].c_str());
    }
    result[strings.size()] = NULL;
    return result;
}

std::string HTTPRequestCGI::readFromPipe(int pipefd) {
    char buffer[1024];
    ssize_t bytesRead;
    std::string output = "";
    while ((bytesRead = read(pipefd, buffer, sizeof(buffer))) != 0) {
        buffer[bytesRead] = '\0';
        output.append(buffer);
    }
    return output;
}