# TCP based Simple File Server

The goal is to write a TCP based client and server. The client sends a message
that contains a filename and a number N that denotes the number of bytes requested. Assume that file size
is greater than or equal to N. The server will send the LAST N bytes of the specified file (assume that it is
in its current directory). If the file does not exist, the server sends a special message “SORRY!”.
The client will save the received bytes in a file with name filename1 in the current directory. If the file
does not exist in the server, the client will print the message: “Server says that the file does not exist.”
Assume that the files only contain regular printable text characters.

## How to Compile

We have provided the make file used for compilation of the code.

Run the following command to compile both the server and client.

```bash
make
```

This will produce the server and client executables in the same folder.

## How to run

Open one terminal and setup the server using the command below.

```bash
./server $PORT
```

We can write any valid port number.

> If port number is below 1024, then we would require sudo permissions to run the server, as it is a admin port. Default port number is also admin port.

Now, after the server has been setup. We can start the client in another terminal.

Client requires IP Address of the server (else is default to localhost) and a port number on which server is setup.

```bash
./client $IP_ADDR $PORT
```

## Conclusion

The program works correctly, provided that the IP address and port numbers are correct.

The input message should be in correct order for the server to compute. Any other format will be checked and prompted to the client.

If format is correct and the filename does not exist that will be informed by the server as error 404, and appropriate message will be prompted to the client.

If we give the command "exit", we can exit from the client side and also close the child fork of the server.

The server itself should be closed by teminating using Ctrl+C.

```bash
make clean 
```

This will remove the executable files.

## Author

Soham Tripathy (CS20B073)

## References

UDP server and client base code was provided by the professor.
