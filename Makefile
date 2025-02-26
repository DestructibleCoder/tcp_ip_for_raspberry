all: server client

server: 
	c++ server.cpp -o server -w

client:
	c++ client.cpp -o client -w
