# DNS server

This folder contains source files for a program to replicate the actual functioning of the DNS servers on a small scale.

## Contents

- **Source Files**: The various server levels have their own .cpp and .h files. These cpp files provide functions to create a server, store info in a temp database and accessor functions.

- The **'inputfile.txt'** contains the information about the ip addresses of various servers.

The format of the 'inputfile.txt' is given below.

```text
BEGIN_DATA
NR IPAddress
RDS IPAddress
TDS_com IPAddress
TDS_edu IPAddress
<ADS1> IPAddress
...
<ADS6> IPAddress
List_of_ADS1
www.abank.com 202.15.5.1
smtp.abank.com 202.15.5.2
ftp.abank.com 202.15.5.3
sshd1.abank.com 202.15.5.4
vpn.abank.com 202.15.5.5
List_of_ADS2
..
List_of_ADS3
..
List_of_ADS4
www.green.edu 192.13.5.1
smtp.green.edu 192.13.5.2
ftp.green.edu 192.13.5.3
sshd1.green.edu 192.13.5.4
vpn.green.edu 192.13.5.5
List_of_ADS5
..
List_of_ADS6
..
END_DATA
```

\<Content\> these tags can be replaced by the actual ads server names. Like in the above example the \<ADS1\> can be replaced by 'abank.com'.

- **record-log**: This is generated using the below command.

```bash
script record-log
```

The terminal session is recoreded.

```bash
exit
```
This exits the recording session and generates the file.

```bash
cat record-log
```
The above command is used to view the contents of the record-log. It contains the information about my testing phase.

- **readme.md**: contains assignment description and ways to compile and run.
- **Makefile**
- **COMMENTS.txt**: contains information on my experience and learing outcomes.


## Steps to Compile

Before compiling remove temp files by the following command.

```bash
make clean
```

Now compile using the following command.

```bash
make 
```

'run' executable is generated.

## Run and Test

The run executable expects two arguments,
(1) Init Port number
(2) inputfile.txt -> contains ip_address mapping information.

Run the program using the following command.

```bash
./run 8000 inputfile.txt
```

The port number must be above 1023, or it will give non-priveledged port error.

It will ask for server name, and will give you the mapping if present.

## Note

While making changes to the inputfile.txt, the top level domain must remain same ie. .com and .edu. Any changes to that will not be caught.