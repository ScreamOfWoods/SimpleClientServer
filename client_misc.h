#ifndef CLIENT_MISC_H
#define CLIENT_MISC_H

void print_usage()
{
	printf("Usage: client [OPTIONS]...\n");
	printf("Connect to the server written for the System programming course work.\n\n");
	printf("Mandatory arguments to long options are mandatory for short options too.\n");
	printf("-s, --server      specify the server's address.\n");
	printf("-p, --port        specify the server's port\n");
	printf("-f, --file-name   specify a file-name that the server should find and send\n");
}

void printMp3(dum_mp3_t mp3)
{
	printf("______________DUMMY MP3 FILE______________\n");
	printf("| Name     |   %s\n", mp3.name);
	printf("| Artist   |   %s\n", mp3.artist);
	printf("| Genre    |   %s\n", mp3.genre);
	printf("| Duration |   %f\n", mp3.duration);
	printf("| Year     |   %d\n", mp3.year);
	printf("__________________________________________\n");
}

void printJpg(dum_jpg_t jpg)
{
	printf("______________DUMMY JPG FILE______________\n");
	printf("| Name        |   %s\n", jpg.name);
	printf("| Artist      |   %s\n", jpg.artist);
	printf("| Width  (cm) |   %f\n", jpg.width);
	printf("| Height (cm) |   %f\n", jpg.height);
	printf("| Year        |   %d\n", jpg.year);
	printf("__________________________________________\n");
}

void checkFileName(package_t* package, char* fileName)
{
	char *ext = strrchr(package->fileName, '.');
	if(!strcmp(ext, ".mp3"))
	{
		package->isMp3 = 1;
	}
	else if(!strcmp(ext, ".jpg"))
	{
		package->isMp3 = 0;
	}
	else
	{
		printf("Unsupported format!");
		exit(EXIT_FAILURE);
	}
}

void connectToServer(int* sockfd, struct sockaddr_in* serv_addr, package_t* pack)
{
	int valread;	
	
	if ((connect(*sockfd, (struct sockaddr*) serv_addr, sizeof(*serv_addr))) < 0)
    {
        perror("Connection Failed");
        printf("Is the server up and running?\n");
        exit(EXIT_FAILURE);
    }
    send(*sockfd, (void*) pack, sizeof(package_t), 0);
	
	if(pack->isMp3)
	{
		dum_mp3_t res;
		if((valread = read(*sockfd, (dum_mp3_t*) &res, sizeof(dum_mp3_t))) == -1)
		{
			perror("Error while reading from socket");
			exit(EXIT_FAILURE);
		}
		if(strlen(res.name) == 0)
		{
			printf("Cannot find %s\n", pack->fileName);
			exit(EXIT_FAILURE);
		}
		else
		{
			printMp3(res);
		}
	}
	else
	{
		dum_jpg_t res;
		if((valread = read(*sockfd, (dum_jpg_t*) &res, sizeof(dum_jpg_t))) == -1)
		{
			perror("Error while reading from socket");
			exit(EXIT_FAILURE);
		}
		if(strlen(res.name) == 0)
		{
			printf("Cannot find %s\n", pack->fileName);
			exit(EXIT_FAILURE);
		}
		else
		{
			printJpg(res);
		}
	}
	close(*sockfd);
}

void argParse(int argc, char** argv, struct sockaddr_in* serv_addr, int* sockfd, package_t* package)
{
	static struct option long_options[] = 
	{
		{"server",    required_argument, 0, 's'},
		{"port",      required_argument, 0, 'p'},
		{"file-name", required_argument, 0, 'f'},
		{ 0,          0,                 0,  0}
	};
	
	int option_index = 0;
	int choice, sflag = 0, pflag = 0, fflag = 0;
	int port;
	int err = 0;
	memset(serv_addr, '0', sizeof(serv_addr));
	while((choice = getopt_long(argc, argv, "s:p:f:", long_options, &option_index)) != -1)
	{
		switch(choice)
		{
			case 's':
				sflag = 1;
				// Convert IPv4 address from text to binary form
				if((inet_pton(AF_INET, optarg, (void*) &serv_addr->sin_addr))<=0) 
				{
					perror("\nInvalid address\n");
					exit(EXIT_FAILURE);
				}
				if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)	
				{
					perror("\nSocket creation error \n");
					exit(EXIT_FAILURE);
				}
				break;
			case 'p':
				pflag = 1;
				port = strtol(optarg, NULL, 10);
				if(port == ERANGE)
				{
					perror("Conversion error");
					exit(EXIT_FAILURE);
				}
				serv_addr->sin_family = AF_INET;
			    serv_addr->sin_port = htons(port);
				break;
			case 'f':
				fflag = 1;
				strcpy(package->fileName, optarg);
				checkFileName(package, optarg);
				break;				
			case '?':
				err = 1;
				break;
		}
	}
	if((!sflag) || (!pflag) || (!fflag))
	{
		print_usage();
		exit(EXIT_FAILURE);
	}
	else if(optind != argc)
	{
		print_usage();
		exit(EXIT_FAILURE);
	}
	else if(err)
	{
		print_usage();
		exit(EXIT_FAILURE);
		
	}
	
}

#endif
