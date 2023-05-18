# UDP based Math server

The goal is to write a UDP-based client and server. The server implements
a simple mathematical calculator. The client sends a message that contains a mathematical operation to be
done. The server performs the necessary computation and replies with the result.
Assume that both operands (op1 and op2) are integers; and that all operators are binary and the operation
is op1 operator op2. The return value is an integer (suitably truncated). The operators to be implemented
are: add, mul, mod, hyp (find hypotentuse give two numbers)

## How to Compile

We have provided the make file used for compilation of the code.

Run the following command to compile both the server and client.

```bash
make
```

This will produce the Mserver and Mclient executables in the same folder.

## How to run

Open one terminal and setup the server using the command below.

```bash
./Mserver $PORT
```

We can write any valid port number.

> If port number is below 1024, then we would require sudo permissions to run the server, as it is a admin port. Default port number is also admin port.

Now, after the server has been setup. We can start the client in another terminal.

Client requires IP Address of the server (else is default to localhost) and a port number on which server is setup.

```bash
./Mclient $IP_ADDR $PORT
```

## Conclusion

The program works correctly, provided that the IP address and port numbers are correct.

The input message should be in correct order for the server to compute. Any other format will end up being ignored by the server.

If invalid messages are indentified and are prompted to the client.

To exit the client we can just type "exit".
For the server we will have to terminate the program using Ctrl+C.

```bash
make clean 
```

This will remove the executable files.

## Author

Soham Tripathy (CS20B073)

## References

UDP server and client base code was provided by the professor.
