# UDP based Echo Server and Client

We were given the code for server and client based on UDP. We had to make modifications to them, such that they work as a UDP echo server and client.

- **Echo Server**: Waits for the client message and sends the recieved message back.
- **Echo Client**: Sends a message and waits for the server's response.

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

The input message size from the client cannot exceed 1024 bytes, this will crash the program.

To exit we will have to terminate the program using Ctrl+C.

```bash
make clean 
```

This will remove the executable files.

## Author

Soham Tripathy (CS20B073)

## References

UDP server and client base code was provided by the professor.
