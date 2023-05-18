from pwn import *
import matplotlib.pyplot as plt
import math


reciever = ELF("./ReceiverGBN/receiver")
sender = ELF("./SenderGBN/sender")

log_error = [i for i in range(-12,0,1)]
re_trans_rate = list()

k = 0.2
while (k < 0.99):
    log_error.append(math.log10(k))
    k+=0.1
    
    


for i in log_error:
    p_rec = reciever.process(["-e", str(10**i), "-n", "40", "-p", "8080"]);
    p_sen = sender.process(["-s", "127.0.0.1", "-p", "8080", "-l", "128", "-r", "10", "-n", "40", "-w", "3", "-f", "10" ])
    a = p_rec.recvall()
    b = p_sen.recvall().decode().split("; ")

    re_trans_rate.append(float(b[2].split()[3]))
    print("done " + str(i))
    
plt.title("Log(Pck Err Rate) V/s Re-Trans Rate")
plt.xlabel("Log(Packet Error Rate)")
plt.ylabel("Retransmission Rate")
plt.plot(log_error, re_trans_rate)
plt.show()