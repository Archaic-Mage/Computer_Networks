# CRC Implementation

## Introduction

This is a simple implementation of the CRC algorithm in cpp. The code is written in a way that it can be easily ported to other languages.

**Input:** The input to your program will be as follows:
```bash
./ec infile outfile
```
The input file (infile) contains a set of binary strings (each of length 128 bits). For each string, you
have to compute the CRC checksum, as explained below.

## Cyclic Redundancy Check

The CCITT CRC-8 polynomial: x8 + x2 + x + 1 (0x07) is used in the Bluetooth, IrDA, and many other.
This implementation only uses the bit-wise algorithm.

**Output:** The output for each input string will be as shown in the example:
```txt
Input: 0101......01
CRC: 0101......01110..11
<Other Output> - See below
```
Note that each input string is 128 bits and the checksums are 8 bits each. All output will be stored
in the ’outfile’.

## Error Detection Properties

For each entry in the input file, report the error detection properties of the mechanism as follows:

- Generate 10 odd number (3 and above) of random bit errors (for each string) and check if the error
is detected. The output format is as follows:

```txt 
Original String: 0101......01110
Original String with CRC: 0101......01110..11
Corrupted String: 1011......01110..11
Number of Errors Introduced: 3
CRC Check: Failed (or Passed)
```

- For each input string, generate 5 random bursty errors, each of length 6 (for each string) with
starting bit error position uniformly random between 100 and 110. Check if the error is detected
in each case. The output format is similar to what is shown above.

This output should also be appended to the output file mentioned earlier.
Random generators such as random and drand48 from ```stdlib.h``` may be useful.

## Contents 

- Source files 
- Sample files
- Corresponding Output file, showing both CRC computation and error detection tests

## How to Compile 

```bash
make
```

## Input Files

I have provided a program to generate the input files for you. You can compile it as follows:

```bash
make cInput
```

Run the generated program as follows:

```bash
./cIn 
```
