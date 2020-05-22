/*
    Variation of the simple Client-Server Package.
    A Simple Bi-Directional Chat

    Server Side

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

/*
    Handle Error and Exit
*/
void error(char *emsg){
    perror(emsg);
    exit(EXIT_FAILURE);
}

/*
    Check for Conversation End
*/

int checkEnd(char *msg){
    if(! strcmp("quit\n",msg)){
        printf("Ending Communication!\n\n");
        return 0;
    }
    return 1;
}

/*
    Main Function
*/
int main(int argc, char *argv[]){

    if(argc < 2)
        error("ERROR: Server Port Not Provided");

    int sockfd, clisockfd, portno, clilen, stat=1;

    char buffer[256]; // Each Message can 255 Bytes Long
    bzero(buffer,256);

    /*
        sockaddr Definition

            struct sockaddr_in {
                short   sin_family;
                u_short sin_port;
                struct  in_addr sin_addr;
                char    sin_zero[8];
            };
    */
    struct sockaddr_in serv_addr, cli_addr;

    // 1. Create a Socket File Descriptor
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd < 0)
        error("ERROR: Could not Open Socket");

    // 1.5 : Cleanup and Init
    portno = atoi(argv[1]); // Convert Parameter from String to Int
    bzero((char *) &serv_addr, sizeof(serv_addr)); // Cast struct to char point and clear
    // Init serv_addr
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno); // Host to Network Style Conversion

    // 2. Bind Socket
    if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR: Could not Bind Socket");

    // 3. Listen for Connections
    listen(sockfd, 5); // Apparently 5 is standard

    // 4. Accept Incoming Connection
    clisockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if(clisockfd < 0)
        error("ERROR: Could Not Accept Connection");

    // 5. Start Loop
    while(stat) {
        int n = read(clisockfd,buffer,255);
        if(n < 0)
            error("ERROR: Problem Reading from Remote Socket");
        printf("Client : %s\n",buffer);

        if(!checkEnd(buffer))
            break;

        // Re-use buffer
        bzero(buffer,256);

        printf("Server : ");
        fgets(buffer,255,stdin);
        printf("\n");

        if(!checkEnd(buffer))
            break;

        n = write(clisockfd,buffer,255);
        if(n < 0)
            error("ERROR: Could Note Write to Remote Socket");

        bzero(buffer,256);
    }

    return 0;
}
