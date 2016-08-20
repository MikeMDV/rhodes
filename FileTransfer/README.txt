Project 2 README

Michael Marven
CS372 400 Winter 2016

File list:
    
    ftserve.cpp
    Makefile
    ftclient
    README.txt

All programs were compiled and tested on OSU flip.


Ftserve compilation

- Chatserve is coded in C++.

- Ensure Makefile and ftserve.cpp are in the same directory.

- Makefile must not have any file suffix

- Enter "make all" at the command line and the ftserve binary will be created.

- "make clean" removes the ftserve binary.


Ftclient preparation

- Ftclient is coded in Python with the shebang enabling it to be run as a 
  binary.
  
- It also must not have any file suffix.

- Make sure ftclient has executable permissions by entering 
  "chmod +x ftclient" on the command line


Ftserve execution

- Enter ./ftserve port# on the command line

- Example: ./ftserve 29658


Ftclient execution

- Enter ./ftclient hostname serv_port# -g [FILE] | -l  data_port# on the command line

- Example: ./ftclient localhost 29658 -l 29659

- Example: ./ftclient localhost 29658 -g long.txt 29659


Ftserve control

Ftserve must be started before ftclient.

Once started, ftserve will wait for a connection and a message from 
ftclient. When a connection is established, the command is parsed and the 
request is completed.

Ftserve is multi-threaded. It can accept up to 5 connections. A child process
will end once the client request is completed.


Ftclient control

Ftserve must be started before ftclient.

The command -l will retrieve the directory listing from the server and the 
combination of -g with a valid file name will retrieve a file from the server.

Once the file or directory listing is retrieved, the program ends.


Extra credit

The server is multithreaded and can accept up to 5 
client connections. It uses fork() to spawn child processes that communicate 
with clients while the parent process listens for more connections.

