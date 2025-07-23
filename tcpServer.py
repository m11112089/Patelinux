#!/usr/bin/env python
# -*- coding: utf-8 -*-
import socket
import datetime

HOST = '192.168.1.1'
PORT = 5001

tcpBunch = 7300
cTcpBunch = tcpBunch * 100

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind((HOST, PORT))
s.listen(5)

print('server start at: %s:%s' % (HOST, PORT))
print('wait for connection...')

while True:
    conn, addr = s.accept()
    print('connected by ' + str(addr))

    writeFileName = ''
    totalLength = 0
    
    writeFileName = datetime.datetime.now().strftime('%Y-%m-%d--%H-%M-%S-%f') + '.bin'
    
    f = open(writeFileName, "wb")

    while True:
        indata = conn.recv(tcpBunch)
        #print('recved: %d bytes' %(totalLength))

        if len(indata) == 0: # connection closed
            conn.close()
            print('client closed connection.')
            break
        totalLength += len(indata)
        f.write(indata)

    print('recved: %d bytes' %(totalLength))
    f.close()
    
s.close()