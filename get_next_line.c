#include "get_next_line.h"

#include <stdio.h>

size_t	find_end_char(char *str, int *new_line_found)
{
	// Takes string
	// Returns position of new line pointer and sets new_line_found to 1 if new line found
	// Else returns position of null char (end of string) and sets new_line_found to 0

	size_t i = 0;
	*new_line_found = 0;
	if (!str)
	{
		return (0);
	}
	while (str[i])
	{
		if (str[i] == '\n')
		{
			*new_line_found = 1;
			return (i);
		}
		i++;
	}
	return (i);
}

int	grow_buffer(char *str, int fd)
{
	char new_buff[BUFF_SIZE + 1];

	ssize_t chars_read = read(fd, new_buff, BUFF_SIZE);
	if (chars_read == -1)
	{
		return (-1);
	}
	new_buff[chars_read] = '\0';


	//join strs
	size_t	str_len = 0;
	while (str[str_len])
	{
		str_len++;
	}
	size_t i = 0;
	while (new_buff[i])
	{
		str[str_len + i] = new_buff[i];
		i++;
	}
	str[str_len + i] = '\0';

	// printf("The strings grow buff is working with are buff: %s    new_buff: %s    tmp: %s\n", *str, new_buff, tmp);
	if (chars_read == 0)
		return (1);
	return (0);
}

// char	*ft_strslice(char *str, size_t left, size_t right)
// {
// 	size_t str_len;

// 	str_len = 0;
// 	while (str[str_len])
// 	{
// 		str_len++;
// 	}
// 	if (right > str_len)
// 	{
// 		right = str_len;
// 	}
// 	if (left > right)
// 	{
// 		left = right;
// 	}
// 	size_t str_size = (right - left + 1);
// 	char	*result = malloc(sizeof (*result) * (str_size));
// 	if (!result)
// 	{
// 		return (NULL);
// 	}
// 	size_t i = 0;
// 	while (left + i < right)
// 	{
// 		result[i] = str[left + i];
// 		i++;
// 	}
// 	result[i] = '\0';
// 	return (result);
// }

char	*extract_next_line(char *buff, size_t end_position)
{
	char	*next_line;

	if (buff[end_position] == '\n')
	{
		end_position++;
	}
	next_line = malloc(sizeof (*next_line) * (end_position + 1));
	if (!next_line)
	{
		return (NULL);
	}
	size_t i = 0;
	while (i < end_position)
	{
		next_line[i] = buff[i];
		i++;
	}
	next_line[i] = '\0';

	size_t j = 0;
	while (buff[i + j])
	{
		buff[j] = buff[i + j];
		// printf("copying across char %c\n", buff[j]);
		j++;
	}
	buff[j] = '\0';
	return (next_line);
}

char    *get_next_line(int fd)
{
    static char buff_remaining[FD_MAX * (2 * BUFF_SIZE + 1)];

	int		new_line_found;
	ssize_t	end_position = find_end_char(&(buff_remaining[fd * (2 * BUFF_SIZE + 1)]), &new_line_found);
	int		reached_end_of_fd = 0;
	while (!new_line_found && reached_end_of_fd == 0)
	{
		reached_end_of_fd = grow_buffer(&(buff_remaining[fd * (2 * BUFF_SIZE + 1)]), fd);
		if (reached_end_of_fd == -1)
		{
			return (NULL);
		}
		end_position = find_end_char(&(buff_remaining[fd * (2 *BUFF_SIZE + 1)]), &new_line_found);
		// printf("the value of end pos: %zu, end_of_fd: %d, nl_found: %d\n", end_position, reached_end_of_fd, new_line_found);
		// printf("the current state of the buffer is %s\n", buff_remaining[fd]);
	}
	// printf("The current state of the buffer is:\n----------\n%s\n----------\n", &(buff_remaining[fd * (2 * BUFF_SIZE + 1)]));
	char	*next_line = extract_next_line(&(buff_remaining[fd * (2 * BUFF_SIZE + 1)]), end_position);
	// free(buff_remaining[fd]);
	// buff_remaining[fd] = NULL;
	if (reached_end_of_fd && next_line[0] == '\0')
	{
		free(next_line);
		return (NULL);
	}
	return (next_line);

    // grow_buffer(pointer, fd)
    // find end char
    // gnl start to end
    // extract out the new line and return from buff
    // grow buff - keeps filling chunks to buff until it finds a \n or \0 char
    // return buff
    // if new line not found

}