all: chatClient.o chatServer.o function.o
	gcc chatClient.o function.o -o chatClient
	gcc chatServer.o function.o -o chatServer

chatClient.o: chatClient.c function.h
	gcc -g -c chatClient.c -o chatClient.o

chatServer.o: chatServer.c function.h
	gcc -g -c chatServer.c -o chatServer.o

function.o: function.c
	gcc -g -c function.c -o function.o

clean:
	rm *.o chatClient chatServer client.txt server.txt
