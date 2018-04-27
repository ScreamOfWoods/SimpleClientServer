#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "prj_types.h"
#include "client_misc.h"

int main(int argc, char *argv[])
{
    struct sockaddr_in serv_addr;
	package_t pack;
    int sock = 0, valread;
    memset(&serv_addr, 0, sizeof(serv_addr));
    
	argParse(argc, argv, &serv_addr, &sock, &pack);
	connectToServer(&sock, &serv_addr, &pack);

    return 0;
}
