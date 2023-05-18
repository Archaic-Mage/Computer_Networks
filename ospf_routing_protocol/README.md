# OSPF

## Compiling Information

- Clean the directory for the fresh compilation by using the following command
  
```bash
make clean
```

- Compile the program using the following command.

```bash
make
```

- The above generates an 'ospf' executable which can be used to run routers as different processes.

## Run Information

### Requirements

There should be an input file in the current directory, which should contain information about the network topology.

It should be of the following format.

```txt
<no_of_nodes> <no_of_edges>
n1 n2
n2 n3
n3 n5
...
```

The following usage format should be maintained while running the 'ospf' executable.

```Usage: ./ospf -i <router_id> -f <input_file> -o <out-file> -h <hello_interval> -l <lsa_interval> -s <spf_interval>```


### How to Run

I have provided the a 'start.py' python file which runs the 'ospf' executable on different processes inacting different routers.

Run the following command to execute them.

```bash
python3 start.py
```

This runs the routers for 100 seconds and then kills them.
The HELLO, LSA and SPF intervals are set to 1, 2 and 10 seconds respectively.

>NOTE: The 'main.cpp' must be compiled before running the above command.