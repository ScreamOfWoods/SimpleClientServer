#ifndef PRJ_TYPES_H_
	#define PRJ_TYPES_H_
	typedef struct Dum_mp3
	{
		char name[100];
		char artist[50];
		char genre[50];
		float duration;
		int year;
	} dum_mp3_t;

	typedef struct Dum_jpg
	{
		char name[100];
		char artist[50];
		float width;
		float height;
		int year;
	} dum_jpg_t;

	typedef struct Package
	{
		char fileName[50];	
		short isMp3;
	} package_t;
#endif
