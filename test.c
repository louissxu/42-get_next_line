#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include <time.h>

#define FILE_NAME "bacon.txt"

int main(void)
{
	clock_t begin = clock();
	for (int j = 0; j < 1000; j++)
	{
		char buff[1000];
		int i = 0;
	
		int fd = open(FILE_NAME, O_RDONLY);
		while (i < 1000)
		{
			read(fd, (buff + i), 1);
			i++;
		}
		close(fd);
	}
	clock_t end = clock();
	double time_singles = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("The total time taken using singles was %f\n", time_singles);
		
	clock_t begin2 = clock();
	for (int j = 0; j < 1000; j++)
	{
		char buff[1000];
		int i = 0;

		int fd = open(FILE_NAME, O_RDONLY);
		while (i < 1)
		{
			read(fd, (buff + i), 1000);
			i++;
		}
		close(fd);
	}
	clock_t end2 = clock();
	double time_blocks = (double)(end2 - begin2) / CLOCKS_PER_SEC;
	printf("The total time taken using chunks was %f\n", time_blocks);
	
}