#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include <time.h>

#include "get_next_line.h"

#define FILE_NAME "test.txt"

int main(void)
{
	// clock_t begin = clock();
	// for (int j = 0; j < 1000; j++)
	// {
	// 	char buff[1000];
	// 	int i = 0;
	
	// 	int fd = open(FILE_NAME, O_RDONLY);
	// 	while (i < 1000)
	// 	{
	// 		read(fd, (buff + i), 1);
	// 		i++;
	// 	}
	// 	close(fd);
	// }
	// clock_t end = clock();
	// double time_singles = (double)(end - begin) / CLOCKS_PER_SEC;
	// printf("The total time taken using singles was %f\n", time_singles);
		
	// clock_t begin2 = clock();
	// for (int j = 0; j < 1000; j++)
	// {
	// 	char buff[1000];
	// 	int i = 0;

	// 	int fd = open(FILE_NAME, O_RDONLY);
	// 	while (i < 1)
	// 	{
	// 		read(fd, (buff + i), 1000);
	// 		i++;
	// 	}
	// 	close(fd);
	// }
	// clock_t end2 = clock();
	// double time_blocks = (double)(end2 - begin2) / CLOCKS_PER_SEC;
	// printf("The total time taken using chunks was %f\n", time_blocks);

	char buff[1000];
	int fd = open(FILE_NAME, O_RDONLY);
	int ret = 1;
	int i = 0;
	while (i < 10)
	{
		ret = read(fd, buff, BUFF_SIZE);
		buff[ret] = '\0';
		printf("The current buffer is: <%s> return value was: %d\n", buff, ret);
		i++;
	}
}