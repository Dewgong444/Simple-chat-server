#include <cstdlib>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <iostream>

using namespace std;

string getName()
{
	//trying to get the user's name
	string name;
	int noError = 0;
	while(noError == 0)
	{
		cout << "Please enter a user name below: " << endl;
        	getline(cin, name);
		//http://www.cplusplus.com/reference/string/string/size/
        	if (name.size() > 10)
        	{
                	cout << "ERROR: Username is too long! Please restrict to 10 characters or less!" << endl;
		}
		else 
		{ //for breaking the error check loop
			noError = 1;
		}
	}
        return name;
}

int initiateContact (char* hostname, char* portnum)
{
	//All this is from Beej's Guide, sections 5.1 - 5.4
	struct addrinfo hints, *res;
	int sockfd;
        hints = {};
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        getaddrinfo(hostname, portnum, &hints, &res);
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if(connect(sockfd, res->ai_addr, res->ai_addrlen) == -1)
        {
                cout << "ERROR: Trouble connecting to server!" << endl;
                exit;
        }
	/*else
	{
		cout << "We've made progress and a connection!" << endl;
	}*/
	return sockfd;
}

string receiveMessage(int sockfd)
{
	char receivedMessage[500]; //500 is the max right?
	string receiveString;
	for(int j = 0; j < 500; j++)
        {
    		receivedMessage[j] = '\0'; //set it all to blank basically
        }
	//Beej's Guide section 5.7
        if(recv(sockfd, &receivedMessage, sizeof(receivedMessage), 0) < 0)
        {
        	cout << "ERROR: Problem with reading message from server" << endl;
        	close(sockfd);
        	exit;
        }
        cout << receivedMessage;
        cout << endl;
	//https://stackoverflow.com/questions/8960087/how-to-convert-a-char-array-to-a-string
	receiveString = receivedMessage;
	return receiveString;
}

string sendMessage(int sockfd, string handle)
{
	char sentMessage[500]; //set to max
	string newMessage;
	string newHandle = handle;
	cout << newHandle;
        getline(cin, newMessage);
        newHandle.append(newMessage);
	if(newHandle.size() > 500)
	{
		cout << "ERROR: Message is too large! I must abort connection!" << endl;
		close(sockfd);
		exit;
	}
        for(int i = 0; i < 500; i++)
        {
        	sentMessage[i] = '\0'; //set it all to 0
        }
	//from https://www.geeksforgeeks.org/convert-string-char-array-cpp/
        strcpy(sentMessage, newHandle.c_str());
	//Beej's Guide 5.7
        if(send(sockfd, &sentMessage, sizeof(sentMessage), 0) < 0)
        {
        	cout << "ERROR: Problem sending message!" << endl;
        	close(sockfd);
        	exit;
        }
	return newHandle;
}

int main(int argc, char* argv[])
{	
  	if (argc != 3) //check that it's the right format
  	{
    		cout << "ERROR: Please enter your program as (chatclient <hostname> <port>)" << endl;
    		return 1;
  	}
	string userHandle;
	int sockfd;
	userHandle = getName();
	//http://www.cplusplus.com/reference/string/string/append/
	userHandle.append(": ");
	char* host = argv[1]; //for some reason initiate connection wont work if these aren't both char*
	char* port = argv[2];
	sockfd = initiateContact(host, port);
	string sendCheck;
	string receiveCheck;
	string quitString = "\\quit";
	while(1)
	{
		sendCheck = sendMessage(sockfd, userHandle);
		//from http://www.cplusplus.com/reference/string/string/find/
		size_t found = sendCheck.find(quitString);
		if(found != string::npos)
		{
			cout << userHandle << " is ending the connection!" << endl;
			break;
		}
		//newHandle = userHandle;
		receiveCheck = receiveMessage(sockfd);
		//again http://www.cplusplus.com/reference/string/string/find/
		found = receiveCheck.find(quitString);
		if(found != string::npos)
                {
			cout << "SERVER is ending the connection!" << endl;
			break;
		}
	}
	close(sockfd);
  	//cout << "Hello " << userHandle << endl;
  	return 0;
}
