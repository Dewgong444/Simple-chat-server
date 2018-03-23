import socket
import sys

def startUp():
	#ensure correct format, argv[0] = chatserve.py, argv[1] = port
	if len(sys.argv) is not 2:
    		print "ERROR: Not correct format. Please enter as (python chatserve.py <port number>)."
    		sys.exit
	#get porn number from the compile line if entered correctly
	portNum = int(sys.argv[1])
	#using stack overflow to get current IP: https://stackoverflow.com/questions/166506/finding-local-ip-addresses-using-pythons-stdlib
	ipNum = socket.gethostbyname(socket.gethostname())
	#used this link to help here: https://www.geeksforgeeks.org/simple-chat-room-using-python/
	firstSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	firstSocket.bind((ipNum, portNum))
	#listen for ONLY 1 connection. only the chatclient should connect.
	firstSocket.listen(1)
	return firstSocket

def sendMessage(conn):
	rawMessage = raw_input('SERVER: ')[:492]
	#see textbook for "raw input" and .send()
	messageToSend = "SERVER: " + rawMessage
	conn.send(messageToSend)
	return messageToSend

def receiveMessage(conn):
	recvMessage = conn.recv(500) 
	#see textbook for python recv
	return recvMessage

def main():	
	quitMessage = '\quit'
	serve = startUp()
	#now we need to establish a loop that runs until force-closed.
	while True:
    		print "Waiting for connection from client ...."
		#text book below. section on TCPserver.py
    		conn, addr = serve.accept()
    		closeWhenFalse = True
    		while closeWhenFalse is True:
        		message = receiveMessage(conn)
        		#check for quit message first
			#https://www.tutorialspoint.com/python/string_find.htm
        		if message.find(quitMessage) is not -1:
            			print "CLIENT has closed the connection."
            			closeWhenFalse = False
            			break
            			#this break should go straight to the conn.close() line
        		print message
        		#now send a message back
        		sentMessage = sendMessage(conn)
        		#check for '\quit'
        		if sentMessage.find(quitMessage) is not -1:
            			print "SERVER has closed the connection."
            			closeWhenFalse = False
            			break
    		conn.close()

#added this just in case, from: https://stackoverflow.com/questions/4041238/why-use-def-main
if __name__ == "__main__":
	main()
