/*
 *             Author: Michael Marven
 *       Date Created: 03/05/16
 * Last Date Modified: 03/05/16
 *          File Name: ftserve.cpp
 *           Overview: The program partially satisfies the requirements for 
 *                     Project 2. This is the server program for the project
 *
 *                     Usage: ./ftserve port#
 *
 *                     This program is adapted from my submission for Project 1
 *                     and examples provided at these pages:
 *                     Beej's Guide to Network Programming webpage
 *                     http://www.gnu.org/software/libc/manual/html_node/Simple-Directory-Lister.html
 *                     http://stackoverflow.com/questions/20911584/how-to-read-a-file-in-multiple-chunks-until-eof-c
 *
 *                     Extra credit - The server is 
 *                     multithreaded and can accept up to 5 client connections.
 *                     It uses fork() to spawn child processes that communicate
 *                     with clients while the parent process listens for more
 *                     connections. 
 *              Input: The program receives commands from the ftclient program
 *
 *             Output: The messages are output to stdout
 *
 *
 */

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <csignal>
#include <sstream>
#include <iterator>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>
#include <dirent.h>


const int ARGS_NUM          = 2; // Correct number of command line arguments
const int MAX_PORT_NUM      = 65535; // Maximum port number allowed
const int BACKLOG           = 5; // Maximum number of connections
const int MAX_OUT_MSG       = 512; // Maximum outgoing message size
const int MAX_TRANS_MSG     = 512; // Maximum transmitted message size
const int MAX_FILE_CHUNK    = 4092; // Maximum transmitted data size
const int MAX_PACK_SIZE     = 4096; // Maximum size of transmitted packet
const int ERR_MSG_SIZE      = 15; // Size of FILE NOT FOUND msg
const int OK_MSG_SIZE       = 6; // Size of ready msg

const char *host = "localhost";

struct CmdData
{
	std::string command;
	std::string file;
	int dataPort;	
};

// Declare a variable for the child process pid
pid_t spawnPid = -5;

/*
 * The validateCommArgsQuant function accepts an int as a parameter for the
 * quantity of command line arguments and validates that the value is correct
 */
void validateArgsNum(int argsEnt, int args, std::string prog);

/*
 * The printCommError function prints an error message explaining the correct
 * format for command line argument entry and exits the program
 */
void printCommError(std::string prog);

/*
 * The isValidPort() func checks whether the parameter is a valid port number
 */
bool isValidPort(std::string port, std::string prog);

/*
 * The error() function prepends a custom error to a strerror msg
 */
void error(std::string msg);

/*
 * The sigchld_handler() function works with sigaction() to redefine the action 
 * taken for SIGCHLD signal from the default action of ignoring the signal to
 * killing the process that generated the signal
 */
void sigchld_handler(int s);

/*
 * The sigint_handler() function works with sigaction() to redefine the action 
 * taken for SIGINT signal from the default action to exiting gracefully
 */
void sigint_handler(int s);

/*
 * The setUpConn() function sets up the parameters for the socket and connection 
 * then binds to the socket
 */
void setUpConn(const char *host, const char *port, int *sock_fd);

/*
 * The recvMsg() function receives the incoming message and stores it in the 
 * inMsg parameter; The function exits the program if the client closes the 
 * connection
 */
void recvMsg(void *inMsg, int *new_fd);

/*
 * The sendMsg() function sends the message stored in the outMsg parameter
 */
void sendMsg(void *outMsg, int *new_fd, int msgLen);

/*
 * The buildDir() function returns a vector witha  list of the files in the 
 * current working directory
 */
std::vector<std::string> buildDir();

/*
 * The completeRequest() function completes the client request
 */
void completeRequest(CmdData *dst, std::string port, std::string host, 
                     int *new_fd);

int main(int argc, char *argv[])
{
    // Validate the command line arguments
    validateArgsNum(argc, ARGS_NUM, argv[0]);
    isValidPort(argv[1], argv[0]);
    
    // Declare variables and structs
    int sockfd, newfd;  // listen on sockfd, new connection on newfd
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa_chld, sa_int;
    
    // Set up signal handler for SIGINT
    sa_int.sa_handler = sigint_handler; 
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = SA_RESTART;
    if (sigaction(SIGINT, &sa_int, NULL) == -1) {
        error("Sigaction - SIGINT: ");
        exit(1);
    }
    
    // Set up the connection, bind to the port, and listen for connections
    setUpConn(host, argv[1], &sockfd);
    
    // Set up signal handler and clean up any zombie processes
    sa_chld.sa_handler = sigchld_handler; 
    sigemptyset(&sa_chld.sa_mask);
    sa_chld.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa_chld, NULL) == -1) {
        error("Sigaction - kill zombies: ");
        exit(1);
    }
    
    // Main accept() loop
    while(1) 
    {  
        sin_size = sizeof their_addr;
        newfd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (newfd == -1) {
            error("Accept: ");
            continue;
        }
        
        // Fork the process and assign the pid of the child to spawnPid
        spawnPid = fork();
        
        // Check which process is running
        if (spawnPid == 0) 
        { 
            // If spawnPid == 0, this is the child process
            
            // Declare variables for child process
            char inMsgBuf[MAX_TRANS_MSG];
            char host[1024];
            char service[20];
            memset(&inMsgBuf, '\0', MAX_TRANS_MSG);
            CmdData dst;
            
            
            close(sockfd); // Child doesn't need the listener
            
            // Get host name
            getnameinfo((struct sockaddr *)&their_addr, sin_size, host, 
                        sizeof host, service, sizeof service, NI_NOFQDN);
            
            std::cout << "Connection from " << host << "\n";
            
            // Receive message
            recvMsg(&inMsgBuf, &newfd);
            
            // Parse message for command, port, and file name if present
            // std::string inMsg(inMsgBuf);
            std::istringstream inMsg(inMsgBuf);
            inMsg >> dst.command >> dst.dataPort >> dst.file;
            
            // Complete the client request
            completeRequest(&dst, argv[1], host, &newfd);
            
            // Close control port connection and quit
            close(newfd);
            exit(0);
        }
        else
        {
            // This is the parent process
            
            close(newfd);  // Parent doesn't need this
        }
        
    }
    
    return 0;
}

/*   *   *   *   *   *   *
 * 
 * Function: validateArgsNum()
 * 
 *    Entry: Input parameter is an int for the command line arguments entered,
 *           an int for the correct number of arguments, and a string for the
 *           program name
 *
 *     Exit: Validates the command line argument quantity
 *
 *  Purpose: Validate the command line argument quantity
 *
 *
 *   *   *   *   *   *   */
void validateArgsNum(int argsEnt, int args, std::string prog)
{
    // Validate that the quantity of command line arguments is correct
    if (argsEnt != args)
    {
        printCommError(prog);
    }
}

/*   *   *   *   *   *   *
 * 
 * Function: printCommError()
 * 
 *    Entry: None
 *
 *     Exit: Prints an error message displaying the correct command line arg
 *           format and exits the program
 *
 *  Purpose: Print an error message and exit the program      
 *
 *
 *   *   *   *   *   *   */
void printCommError(std::string prog)
{
    // Print error message explaining correct command line format
    std::cerr << "Usage: " << prog << " port#\n\n"
              << "Description: port number between 1 and 65535 must be provided\n"
              << "Example: " << prog << " 29658\n\n";
    
    std::exit(1);
}

/*   *   *   *   *   *   *
 * 
 * Function: isValidPort()
 * 
 *    Entry: Input parameter is a string
 *
 *     Exit: Validates the string input is valid port number
 *
 *  Purpose: Validates the string input is valid port number
 *
 *
 *   *   *   *   *   *   */
bool isValidPort(std::string port, std::string prog)
{
    // Initialize boolean return variable
    bool isValid = true;
    int portNum = std::stoi(port);
        
    if (portNum < 1 || portNum > MAX_PORT_NUM)
    {
        printCommError(prog);
    }

    return (isValid);
}

/*   *   *   *   *   *   *
 * 
 * Function: error()
 * 
 *    Entry: Input parameter is a string
 *
 *     Exit: Prints a custom error msg prepended to strerror and exits the 
 *           program
 *
 *  Purpose: Append a custom error to a strerror
 *
 *
 *   *   *   *   *   *   */
void error(std::string msg)
{
    std::cerr << msg << std::strerror(errno) << "\n";
}

/*   *   *   *   *   *   *
 * 
 * Function: sigchld_handler()
 * 
 *    Entry: Input parameter is an int representing the signal number which 
 *           is expected to be SIGCHLD
 *
 *     Exit: Kills the process passed by the parameter
 *
 *  Purpose: Works with sigaction() to redefine the action taken for SIGCHLD 
 *           signal from the default action of ignoring the signal to killing 
 *           the process that generated the signal
 *
 *
 *   *   *   *   *   *   */
void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

/*   *   *   *   *   *   *
 * 
 * Function: sigint_handler()
 * 
 *    Entry: Input parameter is an int representing the signal number which 
 *           is expected to be SIGINT
 *
 *     Exit: Prints message and exits program
 *
 *  Purpose: Works with sigaction() to redefine the action taken for SIGINT 
 *           signal from the default action to exiting gracefully
 *
 *
 *   *   *   *   *   *   */
void sigint_handler(int sig)
{
    // Gracefully exit
    // Kill the foreground pid in spawnPid with the signal passed as a param
    int k = kill(spawnPid, sig);
    if (k == 0 || k == -1)
    {
        // Print a message with the signal number
        std::cout << "Terminated by signal " << sig << "\n";
    }

    exit(0);
}
 
/*   *   *   *   *   *   *
 * 
 * Function: setUpConn()
 * 
 *    Entry: Input parameters are an char arrays for the host and port number, 
 *           and an int pointer for the socket file descriptor
 *
 *     Exit: Value of parameter sock_fd will be changed
 *
 *  Purpose: Sets up the TCP connection; Uses getaddrinfo() to return a linked 
 *           list of addrinfo structs which are used to set options; More info
 *           at Beej's Guide to Network Programming webpage
 *
 *
 *   *   *   *   *   *   */
void setUpConn(const char *host, const char *port, int *sock_fd)
{
    // Declare variables and structs
    int yes = 1;
    int status;
    struct addrinfo hints, *servinfo, *p;
    
    // Set up a linked list of structs of address info
    // Set up addrinfo struct hints to populate some struct fields 
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // Specify IPv4
    hints.ai_socktype = SOCK_STREAM; // Specify TCP stream sockets
    
    // Specify localhost as IP and port from command line args
    if ((status = getaddrinfo(host, port, &hints, &servinfo)) != 0) 
    {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }
    
    // Loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) 
    {
        if ((*sock_fd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) 
        {
            error("Server: socket :");
            continue;
        }

        if (setsockopt(*sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) 
        {
            error("setsockopt :");
            exit(1);
        }

        if (bind(*sock_fd, p->ai_addr, p->ai_addrlen) == -1) 
        {
            close(*sock_fd);
            error("server: bind :");
            continue;
        }

        break;
    }
    
    // Free the linked-list
    freeaddrinfo(servinfo);
    
    if (p == NULL)  
    {
        error("Server: failed to bind: ");
        exit(1);
    }
    
    // Listen for connections on port; Max of BACKLOG connections allowed
    if (listen(*sock_fd, BACKLOG) == -1) {
        error("Listen: ");
        exit(1);
    }
    
    std::cout << "Server open on " << port << "\n";
}

/*   *   *   *   *   *   *
 * 
 * Function: recvMsg()
 * 
 *    Entry: Input parameters are a char array for the message, 
 *           and an int pointer for the new socket file descriptor
 *
 *     Exit: Value of parameter inMsg
 *
 *  Purpose: Receives a message from the client and ends the child process if 
 *           the client closes the connection
 *
 *
 *   *   *   *   *   *   */
void recvMsg(void *inMsg, int *new_fd)
{
    // Declare variables
    int inMsgLen;
    
    // Receive the message; Will block until msg arrives;
    inMsgLen = recv(*new_fd, inMsg, MAX_TRANS_MSG, 0);
    if (inMsgLen == -1)
    {
        error("Receive: ");
    }
    else if (inMsgLen == 0)
    {
        // If message length == 0, the connection has been terminated
        exit(0);
    }
}

/*   *   *   *   *   *   *
 * 
 * Function: sendMsg()
 * 
 *    Entry: Input parameters are a char array for the message, an int pointer 
 *           for the new socket file descriptor, and an int for the msg length
 *
 *     Exit: Sends a message to the client
 *
 *  Purpose: Sends a message to the client 
 *
 *
 *   *   *   *   *   *   */
void sendMsg(void *outMsg, int *new_fd, int msgLen)
{
    // Declare variables
    int bytesSent;
    
    // Receive message
    bytesSent = send(*new_fd, outMsg, msgLen, 0);
    
    if (bytesSent == -1)
    {
        error("send");
    }
    else if (bytesSent < msgLen)
    {
        error("Error: Incomplete transmission");
        exit(1);
    }
}

/*   *   *   *   *   *   *
 * 
 * Function: buildDir()
 * 
 *    Entry: None
 *
 *     Exit: Returns a vector of strings with the contents of the current
 *           working directory 
 *
 *  Purpose: Return the contents of the current directory 
 *
 *
 *   *   *   *   *   *   */
std::vector<std::string> buildDir()
{
    // Declare directory stream, struct, and vector
    DIR *dp;
    struct dirent *ep;
    std::vector<std::string> dir;
    
    // Populate the struct with the directory contents
    dp = opendir("./");
    if (dp != NULL)
    {
        while ((ep = readdir(dp)))
        {
            dir.push_back(ep->d_name);
        }
        (void) closedir(dp);
    }
    else
    {
        error("Couldn't open the directory");
        exit(0);
    }
    
    return dir;
}

/*   *   *   *   *   *   *
 * 
 * Function: completeRequest()
 * 
 *    Entry: CmdData struct with the command, data port, and file name if 
 *           needed, and a string for the control port
 *
 *     Exit: Sends the directory information or the file 
 *
 *  Purpose: Sends the directory or the file 
 *
 *
 *   *   *   *   *   *   */
void completeRequest(CmdData *dst, std::string con_port, std::string host, 
                     int *new_fd)
{
    // List contents of the directory
    std::vector<std::string> dirListing = buildDir();
    
    // Check command 
    if (dst->command == "g") // Send file over data port
    {
        // Print status to console window
        std::cout << "File \"" << dst->file << "\"\nrequested on port "
                  << con_port << ".\n";
                  
        // Check if file is present in directory
        bool isPresent = false;
        for (unsigned int i = 0; i < dirListing.size(); i++)
        {
            if (dirListing[i] == dst->file)
            {
                isPresent = true;
            }
        }
        if (!isPresent)
        {
            // Print status to console window
            std::cout << "File not found. Sending\n" << "error message to\n"
                      << host << ":" << dst->dataPort << "\n";
                      
            // Send error to client on control connection
            std::string outStr = "FILE NOT FOUND";
            char *outMsg = new char[MAX_OUT_MSG];
            strcpy(outMsg, outStr.c_str());
            sendMsg(outMsg, new_fd, ERR_MSG_SIZE);
            delete [] outMsg;
            
            exit(0);
        }
        
        // Inform client that the server is ready to transmit
        std::string outStr = "ready";
        char *outMsg = new char[MAX_OUT_MSG];
        strcpy(outMsg, outStr.c_str());
        sendMsg(outMsg, new_fd, OK_MSG_SIZE);
        
        // Receive message that client is ready to receive directory
        char inMsgBuf[MAX_TRANS_MSG];
        recvMsg(&inMsgBuf, new_fd);
        std::string cliStatus;  
        
        // Parse message for client status
        std::istringstream inMsg(inMsgBuf);
        inMsg >> cliStatus;
        if (cliStatus == "ready") // Open connection to client to send data
        {
            // Declare variables and structs
            int d_sockfd;  
            struct addrinfo hints, *servinfo, *p;
            int dStatus;
            
            // Convert integer port number to const char
            std::string data_port;
            std::stringstream  temp;
            temp << dst->dataPort;
            data_port = temp.str();
            
            memset(&hints, 0, sizeof hints);
            hints.ai_family = AF_INET; // Specify IPv4
            hints.ai_socktype = SOCK_STREAM; // Specify TCP stream sockets
            
            // Specify localhost as IP and port from CmdData struct
            if ((dStatus = getaddrinfo(host.c_str(), data_port.c_str(), &hints, 
                 &servinfo)) != 0) 
            {
                fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(dStatus));
                exit(1);
            }
            
            // Loop through all the results and connect to the first we can
            for(p = servinfo; p != NULL; p = p->ai_next) 
            {
                if ((d_sockfd = socket(p->ai_family, p->ai_socktype,
                        p->ai_protocol)) == -1) 
                {
                    error("client: socket");
                    continue;
                }

                if (connect(d_sockfd, p->ai_addr, p->ai_addrlen) == -1) 
                {
                    close(d_sockfd);
                    error("client: connect");
                    continue;
                }

                break;
            }
            
            if (p == NULL) 
            {
                fprintf(stderr, "client: failed to connect\n");
                exit(1);
            }
            
            // Free the linked list
            freeaddrinfo(servinfo);
            
            // Send the requested file
            std::cout << "Sending \"" << dst->file << "\"\n" << "to " 
                      << host << ":" << dst->dataPort << "\n";
            
            // Declare buffer and open file stream
            char outChunkBuf[MAX_FILE_CHUNK];
            std::ifstream file(dst->file, std::ios_base::in);
            if (!file) // File was unable to be opened
            {
                error("File open: ");
                exit(1);
            }
            int bytesRead;
            std::string outPackBuf;
            char outPack[MAX_PACK_SIZE];
            
            /*
             * Number of characters read is prepended to the beginning of the 
             * outgoing message and sent to client. Client reads the number, 
             * then receives the message. If the total number of characters
             * received does not match, client will continue to recv()
             */
            
            // Enter read, send, receive acknowledgement loop until EOF
            while (1)
            {
                // TODO: Clean this up and remove break statement
                // do while loop?
                file.read(outChunkBuf, MAX_FILE_CHUNK);
                // Get bytes read and prepend the number to the outgoing data
                // Pad with spaces for numbers < 4 digits
                bytesRead = file.gcount();
                if (bytesRead == 0)
                {
                    break;
                }

                std::string bytes = std::to_string(static_cast<long long>(bytesRead));
                // Static cast is required - see page below for details
                // http://stackoverflow.com/questions/10664699/stdto-string-more-than-instance-of-overloaded-function-matches-the-argument
                
                if (bytesRead < 10)
                {
                    outPackBuf = bytes + "   " + std::string(outChunkBuf);
                }
                else if (bytesRead < 100)
                {
                    outPackBuf = bytes + "  " + std::string(outChunkBuf);
                }
                else if (bytesRead < 1000)
                {
                    outPackBuf = bytes + " " + std::string(outChunkBuf);
                }
                else 
                {
                    outPackBuf = bytes + std::string(outChunkBuf);
                    
                }

                
                // Copy string to char array without terminating w/null char
                memcpy(outPack, outPackBuf.c_str(), MAX_PACK_SIZE);
                
                
                // Send message
                sendMsg(outPack, &d_sockfd, (bytesRead + 4));
                
                // Wait for acknowledgement from client on control port
                recvMsg(&inMsgBuf, new_fd);
                
                // If acknowledgement was received, send the next file listing
            }
            
            // Close the connection
            close(d_sockfd);
            delete [] outMsg;
        }
    }
    else if (dst->command == "l") // Send directory contents over data port
    {
        std::cout << "List directory requested\non port " << dst->dataPort
                  << ".\n";
        
        // Inform client that the server is ready to transmit
        std::string outStr = "ready";
        char *outMsg = new char[MAX_OUT_MSG];
        strcpy(outMsg, outStr.c_str());
        sendMsg(outMsg, new_fd, OK_MSG_SIZE);
        // memset(&outMsg, '\0', MAX_OUT_MSG);
        
        
        // Receive message that client is ready to receive directory
        char inMsgBuf[MAX_TRANS_MSG];
        recvMsg(&inMsgBuf, new_fd);
        std::string cliStatus;  
        
        // Parse message for client status
        std::istringstream inMsg(inMsgBuf);
        inMsg >> cliStatus;
        if (cliStatus == "ready") // Open connection to client to send data
        {
            // Declare variables and structs
            int d_sockfd;  
            struct addrinfo hints, *servinfo, *p;
            int dStatus;
            
            // Convert integer port number to const char
            std::string data_port;
            std::stringstream  temp;
            temp << dst->dataPort;
            data_port = temp.str();
            
            memset(&hints, 0, sizeof hints);
            hints.ai_family = AF_INET; // Specify IPv4
            hints.ai_socktype = SOCK_STREAM; // Specify TCP stream sockets
            
            // Specify localhost as IP and port from CmdData struct
            if ((dStatus = getaddrinfo(host.c_str(), data_port.c_str(), &hints, 
                 &servinfo)) != 0) 
            {
                fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(dStatus));
                exit(1);
            }
            
            // Loop through all the results and connect to the first we can
            for(p = servinfo; p != NULL; p = p->ai_next) 
            {
                if ((d_sockfd = socket(p->ai_family, p->ai_socktype,
                        p->ai_protocol)) == -1) 
                {
                    error("client: socket");
                    continue;
                }

                if (connect(d_sockfd, p->ai_addr, p->ai_addrlen) == -1) 
                {
                    close(d_sockfd);
                    error("client: connect");
                    continue;
                }

                break;
            }
            
            if (p == NULL) 
            {
                fprintf(stderr, "client: failed to connect\n");
                exit(1);
            }
            
            // Free the linked list
            freeaddrinfo(servinfo);
            
            // Send the directory contents
            std::cout << "Sending directory\ncontents to " << host << ":"
                      << dst->dataPort << "\n";
            for (unsigned int i = 2; i < dirListing.size(); i++)
            {
                // Copy element of dirListing array and send
                strcpy(outMsg, dirListing[i].c_str());
                sendMsg(outMsg, &d_sockfd, dirListing[i].size());
                
                // Wait for acknowledgement from client
                recvMsg(&inMsgBuf, new_fd);
                
                // If acknowledgement was received, send the next file listing
            }
            
            // Close the connection
            close(d_sockfd);
            delete [] outMsg;
        }
    }
}

