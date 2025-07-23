import threading
import time
import socket
import datetime
import select
import Queue as Queue
    
HOST = '192.168.1.1'
DPORT = 5001
CPORT = 5002
TIMEOUT = 0.1   # 100ms

tcpBunch = 7300
cTcpBunch = tcpBunch * 100

class Storage(threading.Thread):
    def __init__(self, q):
        threading.Thread.__init__(self)
        self.que=q
        self.file_prefix="none"
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.s.bind((HOST, DPORT))
        self.s.listen(5)
        self.fileno = 0;
        self.fdRead = [self.s]

    def write2file(self, prefix):
        conn, addr = self.s.accept()
        print('connected by ' + str(addr))

        writeFileName = ''
        totalLength = 0
    
        writeFileName = prefix + '_' + str(self.fileno) + '.bin'
    
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

        self.fileno += 1
        print('recved: %d bytes' %(totalLength))
        f.close()

    def sendfiles(rhost, rport):
        files = os.listdir(folder)
        for file in files:
            s = socket.socket()
            s.connect((rhost, rport))
            path = os.path.join(folder, file)
            print (path)
            with open(path, 'rb') as f:
                s.sendall(f.read())
            s.close()
            f.close()

    def run(self):
        print("Thread Terminal running")
        while True:
            readable, _, _ = select.select(self.fdRead, [], [], TIMEOUT)

            if readable:                
                self.write2file(self.file_prefix)

            try:
                msg = self.que.get_nowait()
                if msg is not None:  # Hit end of queue
                    cmd = msg.split(':')
                    #queue_active = False
                    if cmd[0] == 'quit':
                        break
                    elif cmd[0] == 'start':
                        self.file_prefix = cmd[1].strip() + datetime.datetime.now().strftime('-%Y-%m-%d--%H-%M-%S')
                        print (self.file_prefix)
                        self.fileno = 0
                        self.write2file(prefix)             
            except:
                pass
                
que = Queue.Queue()
storage=Storage(que)
storage.start()

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind((HOST, CPORT))
s.listen(5)
fdRead = [s]

try:
    while True:
        readable, _, _ = select.select(fdRead, [], [], TIMEOUT)
        if readable:
            conn, addr = s.accept()
            print('connected by ' + str(addr))
            indata = conn.recv(1024)
            cmd = indata.split(':')
            que.put(indata)
            #self.write2file("vis")
except KeyboardInterrupt:
    que.put('quit')


storage.join()

print("Done.")
