/*
 * Usage: ./executable <ip> <port>
 * Source: http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
 */


#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n, filecount;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[1024];
    if (argc < 3) {// usage error handling
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)// socket error handling
        error("ERROR opening socket");
    server = gethostbyname(argv[1]); //hostname
    if (server == NULL) {            // host error handling
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    // Start listening
    printf("Listening on %s %s...\n", argv[1], argv[2]);
    filecount = 0;
    while(true){
        bzero(buffer,1024);           // fill buffer with zeros
        n = read(sockfd,buffer,1024); // read packet contents from socket
        if (n < 0)                     // error checking
           error("ERROR reading from socket");
        // write to file

        if (( n > 180 )&&( n < 1023)){
            char filename[15];
            sprintf(filename, "%04d.pcap", filecount);        
            printf("Filename: %s\n", filename);
            printf("Number of bytes: %d\n", n);
            FILE *fp = fopen(filename,"w");// actual file
            fwrite(buffer, n, 1, fp);//correct parameters?
            fclose(fp);
            filecount++;                   //increment file name counter
        }
    }
    return 0;
}
