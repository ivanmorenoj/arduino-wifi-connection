#!/usr/bin/python
# Basic communication between arduino (esp8266 board) and raspberry pi
# Author: Ivan Moreno
#	May 2020

import socket

port = 35000
server_socket = socket.socket()
server_socket.bind(('0.0.0.0', port))

print("Intialize socket server, listen port: {}".format(port))
server_socket.listen(2)
conn, address = server_socket.accept()
print("Connection from: " + str(address))

while True:
    data = conn.recv(1024).decode()

    if data.find("quit!") >= 0 :
        print("Recive disconnect from client")
        print("Close connection...")
        conn.send(b'quit!')
        break
    print("From connected user: " + str(data))
    data = input('Write response -> ')
    conn.send(data.encode())

conn.close()
