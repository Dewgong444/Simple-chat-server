To run this project please perform the following steps in order:
1. In the command line type "make" without quotes.
2. Run the chatserve file by typing "python chatserve.py <portnumber>" where <portnumber> is some integer between 0 and 60000
3. Run the chatclient by typing "chatclient <hostname> <portnumber>" where <hostname> is whatever host is running the chatserve and <portnumber> is the same one as step 2.
4. Talk between them.
5. To end their connection, type "\quit" at any time in any message between them.
6. To end the chatserve for good, you'll need to hit CTRL + C in its window otherwise it will indefinitely wait for a connection.
7. Enjoy!