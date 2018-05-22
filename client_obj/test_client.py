# client  

import socket  

address = ('127.0.0.1', 32000)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(address)

s2 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s2.connect(address)


s2.send("hihihihs2");

data2 = s2.recv(512)  
print 'the data2 received is',data2  
s2.close()  

s.send('hihi')  
data = s.recv(512)  
print 'the data received is',data  

s.close()  

