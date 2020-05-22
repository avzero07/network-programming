/*
    Variation of the simple Client-Server Package.
    A Simple Bi-Directional Chat

    Client Side

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

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

    if(argc < 3){
        fprintf(stderr,"ERROR: Usage is %s <server ip> <server port>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    int sockfd, portno, stat=1;
    char buffer[256];
    struct sockaddr_in serv_addr;

    /*
        server Definition

        struct  hostent {
            char    *h_name;        /* official name of host
            char    **h_aliases;    /* alias list
            int     h_addrtype;     /* host address type
            int     h_length;       /* length of address
            char    **h_addr_list;  /* list of addresses from name server
            #define h_addr  h_addr_list[0]  /* address, for backward compatiblity
            };
    */
    struct hostent *server;

    // 1. Create a Socket File Descriptor
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd < 0)
        error("ERROR: Could Not Open Socket");

    // 2. Resolve Server IP
    server = gethostbyname(argv[1]);
    if(server == NULL){
        fprintf(stderr,"ERROR: Server Cannot be Found\n");
        exit(EXIT_FAILURE);
    }

    // 2.5 Init serv_addr
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[2]);
    bzero(buffer,256);

    // Init serv_addr
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);

    // 3. Connect to Server
    if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR: Could not Connect to Remote Server");

    // 4. Start Loop
    while(stat) {
        printf("Client : ");
        fgets(buffer,255,stdin);
        printf("\n");
        int n = write(sockfd,buffer,strlen(buffer));
        if(n < 0)
            error("ERROR: Could Note Write to Remote Socket");

        if(!checkEnd(buffer))
            break;
        // Recycle Buffer
        bzero(buffer,256);

        n = read(sockfd,buffer,255);
        if(n < 0)
            error("ERROR: Problem Reading from Remote Socket");
        printf("Server: %s",buffer);
        printf("\n");

        if(!checkEnd(buffer))
            break;

        // Recycle Buffer
        bzero(buffer,256);
    }

    return 0;
}
