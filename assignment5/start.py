import subprocess
from time import sleep

if __name__ == '__main__':
    # Create a new process and pass arguments to the worker function
    p = list()
    f = open("input.txt", 'r')
    for i in range(int(f.readline().split()[0])):
       out = subprocess.Popen(["./ospf", "-i", str(i), "-f", "input", "-o", "output", "-h", "1", "-l", "2", "-s", "10"])
       p.append(out)
       
    sleep(102) 
    for l in p:
        l.kill()  
    