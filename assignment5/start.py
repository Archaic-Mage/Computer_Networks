import subprocess
from time import sleep

if __name__ == '__main__':
    # Create a new process and pass arguments to the worker function
    p = list()
    
    for i in range(10):
       out = subprocess.Popen(["./ospf", "-i", str(i), "-f", "input", "-o", "output", "-h", "1", "-l", "2", "-s", "10"])
       p.append(out)
       
    sleep(102)
    for l in p:
        l.kill()  
    