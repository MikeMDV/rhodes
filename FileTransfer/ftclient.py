#!/usr/bin/env python

#
#             Author: Michael Marven
#       Date Created: 03/04/16
# Last Date Modified: 03/08/16
#          File Name: ftclient.py
#           Overview: The program partially satisfies the requirements for 
#                     Project 2. This is the client program for the project
#
#                     ftclient.py must be made an executable for all users 
#                     with chmod a+x ftclient.py
#
#                     Usage: ./ftclient serv_hostname serv_port# -g | -l [file] data_port#
#
#                     Commands: -g - Get file, must be used with file name
#                               -l - List directory contents
#
#                     This program is adapted from program my submission for 
#                     Project 1, from examples provided at
#                     Python v2.6.6 documentation and the overall structure of
#                     client server programs as presented at
#                     Beej's Guide to Network Programming webpage and these
#                     pages
#                     http://stackoverflow.com/questions/3207219/how-to-list-all-files-of-a-directory-in-python
#                     https://docs.python.org/2/howto/sockets.html
#                     http://stackoverflow.com/questions/17667903/python-socket-receive-large-amount-of-data
#
#              Input: The user inputs commands to retrieve a directory or file
#                     from the remote server
#
#             Output: The messages are output to stdout
#
#
#

import socket
import sys
import argparse
import time
import io
from os import walk


#   #   #   #   #   #   #   #
#
# Function: setUpConn()
#
#    Entry: None; Function uses global parameters
#
#     Exit: Socket connection is set up with host
#
#  Purpose: Set up a socket connection with the host and port designated in
#           the command line arguments
#
#
#   #   #   #   #   #   #   #

def setUpConn():
 
    # Begin connection process
    HOST, PORT = args.host, args.c_port

    # Create a socket
    global sock
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Connect to server and send msg
    # TODO: Add error checking for connection and print statement establish conn
    sock.connect((HOST, PORT))
    
#   #   #   #   #   #   #   #
#
# Function: makeRequest()
#
#    Entry: None; Function uses global parameters
#
#     Exit: Initial control message request is sent to server
#
#  Purpose: Send the initial control message to the server
#
#
#   #   #   #   #   #   #   #
    
def makeRequest():

    # Create control message to send
    # If -g flag was not present, args.g == None
    if args.g == None and args.l == 'l':
        msgTrans = args.l + " " + str(args.d_port)
    else:
        msgTrans = "g " + str(args.d_port) + " " + args.g

    # Send control message 
    sock.send(msgTrans + '\n')
    
#   #   #   #   #   #   #   #
#
# Function: receiveFile()
#
#    Entry: None; Function uses global parameters
#
#     Exit: Save the file or print the directory contents from the server
#
#  Purpose: Receive the file or directory contents from the server
#
#
#   #   #   #   #   #   #   #
    
def receiveFile():
    
    # Set of socket for data reception, an INET, STREAMing socket
    serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # Bind the socket to the localhost and port
    serversocket.bind(('localhost', args.d_port))
    # Become a server socket
    serversocket.listen(1)
    
    # Send ready control message to server
    sock.send(ok + '\n')
    
    # Enter loop to receive data
    if args.g == None and args.l == 'l': # Receive directory
        print "Receiving directory\nstructure from\n" + args.host + ":" + str(args.d_port) + "\n"
        while 1:
            # Accept connection from server
            (clientsocket, address) = serversocket.accept()
            
            while 1:
                # Receive the directory contents
                dir = clientsocket.recv(MAX_MSG_LENGTH)
                if dir == '':
                    break
                print dir
                
                # Send ready acknowledgement to server
                sock.send(ok + '\n')
            break

        # Close the socket
        clientsocket.close()
        serversocket.close()
        sock.close()
    elif args.l == None: # Receive requested file
    
        # Check for file name in current directory and handle
        f = []
        for (dirpath, dirnames, filenames) in walk('./'):
            f.extend(filenames)
            break
        if args.g in f:
            print ("File name exists in current directory.\n"
                   "Please choose another file name.\n"
                  )
            serversocket.close()
            sock.close()
            sys.exit(0)
            
        # Open file for writing in append and text mode
        file = io.open(args.g, 'ab')
        
        # Number of characters read is prepended to the beginning of the 
        # outgoing message and sent to client. Client reads the number, 
        # then receives the message. If the total number of characters
        # received does not match, client will continue to recv()
        
        print "Receiving \"" + args.g + "\"\nfrom " + args.host + ":" + str(args.d_port) + "\n"
        while 1:
            # Accept connection from server
            (clientsocket, address) = serversocket.accept()
            
            while 1:
            
                # Receive the packet size - 4 characters long
                raw_msgLen = clientsocket.recv(HEADER_LENGTH)
                
                # Declare a string for the data to be written
                data = ''
                if raw_msgLen != '':
                    msgLen = int(raw_msgLen)
                
                # Receive the file chunks and write to file
                while len(data) < msgLen:
                    chunk = clientsocket.recv(msgLen - len(data))
                    if chunk == '':
                        break
                    data += chunk
                
                # Write the chunk to the file
                file.write(data)
                
                if chunk == '':
                    break
                
                # Send ready acknowledgement to server
                sock.send(ok + '\n')
            
            break
            
        print ("File transfer\n"
               "complete"
              )

        # Close the socket
        clientsocket.close()
        serversocket.close()
        sock.close()
        
        # Close the file
        file.close()
    
MAX_HANDLE_CHAR = 10
MAX_OUT_MSG_LEN = 512
MAX_MSG_LENGTH  = 512
MAX_FILE_CHUNK = 4092
HEADER_LENGTH = 4

if __name__ == "__main__":

    # Parse command line arguments
    parser = argparse.ArgumentParser(description='ftclient')
    parser.add_argument('host', help='server_hostname')
    parser.add_argument('c_port', type=int, help='server_port#')
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument("-g", nargs='?', type=str, metavar="FILE",
                        help='get file command')
    group.add_argument("-l", action='store_const', const='l', 
                       help='list directory command')
    parser.add_argument('d_port', type=int, help='data_port#')
    args = parser.parse_args()
    
    # Confirm port args are valid
    if args.c_port < 1 or args.c_port > 65535:
        print "Invalid control port number entered.\n"
        sys.exit(0)
    
    if args.d_port < 1 or args.d_port > 65535:
        print "Invalid data port number entered.\n" 
        sys.exit(0)
    
    # Set up connection
    setUpConn()
    
    # Make initial request
    makeRequest()
    
    # Declare potential messages to receive from server
    error = 'FILE NOT FOUND'
    ok = 'ready'
    
    # Receive control message on the control port
    recMsg = sock.recv(MAX_MSG_LENGTH)
    
    # If error, print explanation, close socket, and quit
    if error in recMsg:
        print args.host + ":" + str(args.c_port) + " says\n" + recMsg
        sock.close()
    elif ok in recMsg:
        # Server is ready to transmit
        receiveFile()
    
    # Exit the program
    sys.exit(0)



