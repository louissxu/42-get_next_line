#include "get_next_line.c"
#include <fcntl.h>
#include <stdio.h>

#define FILE_NAME "test.txt"

int main(void)
{
	int fd = open(FILE_NAME, O_RDONLY);
	// char *full_buff;
	// full_buff = NULL;
	
	// int reached_end_of_fd = 0;
	// int i = 0;
	// while (i < 10)
	// {
	// 	reached_end_of_fd = grow_buffer(&full_buff, fd);
	// 	printf("after %03d iterations the buffer contents is\n-----\n%s\n-----\n", i, full_buff);
	// 	i++;
	// }

	int i = 0;
	while (i < 10)
	{
		char *nl = get_next_line(fd);
		printf("testing: line number %2d: <%s>\n", i, nl);
		free(nl);
		i++;
	}
}