#include "get_next_line.h"

char	*find_end_char(char *str)
{
	//takes char pointer and returns the location of the end char pointer. if not found, return null
	size_t i = 0;
	while (str[i])
	{
		if (str[i] == )
	}
}

char    *get_next_line(int fd)
{
    static char *buff_remaining[FD_MAX];

	size_t	end_position = find_end_char(buff_remaining[fd]);
	int		reached_end_of_fd = 0;

	while (!end_position && !reached_end_of_fd)
	{
		reached_end_of_fd = grow_buffer(buff_remaining[fd], fd);
		end_position = find_end_char(buff_remaining[fd]);
	}
	char	*next_line = extract_next_line(buff_remaining[fd]);
	return (next_line);

    // grow_buffer(pointer, fd)
    // find end char
    // gnl start to end
    // extract out the new line and return from buff
    // grow buff - keeps filling chunks to buff until it finds a \n or \0 char
    // return buff
    // if new line not found

}