#ifndef SERVER_RW_H
#define SERVER_RW_H	
#include "prj_types.h"

#define BASE_PATH "/home/screamofwoods/Desktop/spr_proj/"

	int readMp3(char* fileName, dum_mp3_t* mp3Buff)
	{
		int read_fd, bytes_read;
		char full_path[1024] = {0};
		strcat(full_path, BASE_PATH);
		strcat(full_path, "music/");	
		strcat(full_path, fileName);
		//readStructure that is mp3
		read_fd = open(full_path, O_RDONLY | O_NONBLOCK);
		bytes_read = read(read_fd, (void*) mp3Buff, sizeof(dum_mp3_t));
		if(bytes_read > 0)
		{
			//printf("\n%s\n", pack_data->mp3Data.name);
			return 1;		
		}
		else
		{
			perror("Error while reading");
			return 0;
		}
	}

	int readJpg(char* fileName, dum_jpg_t* jpgBuff)
	{
		int read_fd, bytes_read;
		char full_path[1024] = {0};
		strcat(full_path, BASE_PATH);
		strcat(full_path, "pics/");	
		strcat(full_path, fileName);
		//readStructure that is mp3
		read_fd = open(full_path, O_RDONLY | O_NONBLOCK);
		bytes_read = read(read_fd, (void*) jpgBuff, sizeof(dum_jpg_t));
		if(bytes_read > 0)
		{
			//printf("\n%s\n", pack_data->mp3Data.name);
			return 1;		
		}
		else
		{
			perror("Error while reading");
			return 0;
		}
	}

	int writeMp3(char* fileName, dum_mp3_t mp3Buff)
	{
		int write_fd, bytes_write;
		char full_path[1024] = {0};
		memset(full_path, 0, sizeof(full_path));
		strcat(full_path, BASE_PATH);	
		strcat(full_path, "music/");
		strcat(full_path, fileName);

		//writeStructure that is mp3
		write_fd = open(full_path, O_CREAT|O_WRONLY|O_NONBLOCK, 0777);
		bytes_write = write(write_fd, (void*) &mp3Buff, sizeof(dum_mp3_t));
		if(bytes_write > 0)
		{
			//printf("Writen successfully!\n");
			return 1;	
		}
		else
		{
			perror("Write mp3 error");
			return 0;
		}
	}	

	int writeJpg(char* fileName, dum_jpg_t jpgBuff)
	{
		int write_fd, bytes_write;
		char full_path[1024] = {0};
		memset(full_path, 0, sizeof(full_path));
		strcat(full_path, BASE_PATH);	
		strcat(full_path, "pics/");
		strcat(full_path, fileName);

		//writeStructure that is jpg
		write_fd = open(full_path, O_CREAT | O_WRONLY | O_NONBLOCK, 0777);
		bytes_write = write(write_fd, (void*) &jpgBuff, sizeof(dum_jpg_t));
		if(bytes_write > 0)
		{
			//printf("Writen successfully!\n");
			return 1;	
		}
		else
		{
			perror("Write jpg error");
			return 0;
		}
	}

#endif
