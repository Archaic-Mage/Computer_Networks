# GBN Protocol

This folder contains source files for a program to replicate the actual functioning of the GBN protocol on a small scale.

## Contents

- **Source Files**: The Sender and Receiver folders contain respective source files to compile. These programs after compilation can run on any other machine for testing

- **record-script**: The scripts are generated using the script linux command. In each folder there is a record-script folder which contains the information of the testing phase.

- **readme.md**: Ways to compile and run.
- **Makefile**
- **COMMENTS.md**: contains information on my experience and learing outcomes.

## Steps to Compile

In the respective folder for Receiver and Sender compile using the following command.

```bash
make 
```

Executables 'sender' and 'receiver' are generated.

## Run and Test

### Arguments 

For *Sender* side the arguments list is as follows:

- -d – Turn ON Debug Mode (OFF if -d flag not present)
- -s string – Receiver Name or IP address.
- -p integer – Receiver’s Port Number
- -l integer – PACKET LENGTH, in bytes
- -r integer – PACKET GEN RATE, in packets per second
- -n integer – MAX PACKETS
- -w integer – WINDOW SIZE
- -f integer – MAX BUFFER SIZE

For the *Receiver* Side the argument list is as follows:

- -d – Turn ON Debug Mode (OFF if -d flag not present)
- -p integer – Receiver’s Port Number
- -n integer – MAX PACKETS
- -e float – Packet Error Rate (RANDOM DROP PROB)

To run the executable expects the above arguments,

Run the program using the following command (Example command)

> Receiver has to run first (server)

On machine 1: (Receiver)

```bash
./receiver -e 0.0001 -n 400 -p <PORT> -d 
```

On machine 2: (Sender)

```bash
./sender -s <machine-2-IP> -p <PORT> -l 128 -r 10 -n 400 -w 3 -f 10 -d
```

>The port number must be above 1023, or it will give non-priveledged port error.


## Note

The '-n' argument, 'PORT' should be the same, otherwise it may produce some undefined behaviours.
