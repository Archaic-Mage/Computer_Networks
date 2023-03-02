#https://pythontic.com/modules/socket/send
import socket
serverName = 'localhost'
serverPort = 12000
clientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

clientSocket.connect((serverName,serverPort))

sentence = input("Input lowercase sentence: ")

clientSocket.send(sentence.encode())

modifiedSentence = clientSocket.recv(1024)

print("From Server: ", modifiedSentence.decode())

clientSocket.close()
