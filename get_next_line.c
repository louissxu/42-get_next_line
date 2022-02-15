#include "get_next_line.h"

/**
 * @brief Finds end char of a string. Either a \n or \0 char.
 * 
 * Searches a string for first instance of the end of a line (which will either
 * be a \n or a \0 if it is the last line). To keep track of this there is a var
 * passed by reference to keep a flag of whether it was a \n or \0 that was
 * found.
 * 
 * @param str The string to be searched.
 * @param new_line_found Integer passed by reference. 1 if \n found. 0 if \0.
 * @return size_t The position of the found char in the string. In practice this
 * is either the strlen without including the end char.
 */
size_t	find_end_char(char *str, int *new_line_found)
{
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

/**
 * @brief Takes a string (passed by reference) and adds the next read buffer.
 * 
 * Takes a string (by reference). Runs read and concats the new read buffer onto
 * the end of the current string.
 * 
 * @param str The reference to the string we want to extend.
 * @param fd The file descriptor to read from.
 * @return int Return flag. 0 if successfully reads and adds on. 1 if error.
 * -1 if end of file reached.
 */
int	grow_buffer(char **str, int fd)
{
	char new_buff[BUFF_SIZE + 1];

	ssize_t chars_read = read(fd, new_buff, BUFF_SIZE);
	if (chars_read == -1)
	{
		return (-1);
	}
	new_buff[chars_read] = '\0';
	size_t	str_len = 0;
	if (*str)
	{
		while ((*str)[str_len])
		{
			str_len++;
		}
	}
	str_len += chars_read;
	char *tmp = malloc(sizeof (*tmp) * (str_len + 1));
	if (!tmp)
	{
		return (1);
	}
	size_t i = 0;
	if (*str)
	{
		while ((*str)[i])
		{
			tmp[i] = (*str)[i];
			i++;
		}
	}
	size_t j = 0;
	while (new_buff[j])
	{
		tmp[i + j] = new_buff[j];
		j++;
	}
	tmp[i + j] = '\0';
	if (*str)
	{
		free((*str));
	}
	(*str) = tmp;
	if (chars_read == 0)
		return (1);
	return (0);
}

/**
 * @brief String slicing.
 * 
 * An implementation of string slicing. As close as I can get since C doesn't
 * allow for default/empty args. And couldn't use 0 as the "blank" flag as 0 is
 * a valid slice position.
 * 
 * @param str String to slice.
 * @param left Left position to slice from (inclusive).
 * @param right Right position to slice to (exclusive).
 * @return char* Heap string of sliced result.
 */

char	*ft_strslice(char *str, size_t left, size_t right)
{
	size_t str_len;

	str_len = 0;
	while (str[str_len])
	{
		str_len++;
	}
	if (right > str_len)
	{
		right = str_len;
	}
	if (left > right)
	{
		left = right;
	}
	size_t str_size = (right - left + 1);
	char	*result = malloc(sizeof (*result) * (str_size));
	if (!result)
	{
		return (NULL);
	}
	size_t i = 0;
	while (left + i < right)
	{
		result[i] = str[left + i];
		i++;
	}
	result[i] = '\0';
	return (result);
}

/**
 * @brief Pulls out the first line and replaces the buffer with what's left.
 * 
 * Takes a string (passed by reference). Splits a string at end position (end
 * position is included in the left string). Adds null chars if needed. Replaces
 * the string being read with the remainder.
 * 
 * @param buff The buffer to work with.
 * @param line_end_position The end position to split at.
 * @return char* The first part of the string that is being sliced off.
 */
char	*extract_next_line(char **buff, size_t line_end_position)
{
	char	*next_line;

	if ((*buff)[line_end_position] == '\n')
	{
		line_end_position++;
	}
	next_line = malloc(sizeof (*next_line) * (line_end_position + 1));
	if (!next_line)
	{
		return (NULL);
	}
	size_t i = 0;
	while (i < line_end_position)
	{
		next_line[i] = (*buff)[i];
		i++;
	}
	next_line[i] = '\0';
	char	*remaining_buffer = ft_strslice(*buff, line_end_position, SIZE_MAX);
	if (!remaining_buffer)
	{
		free(next_line);
		return (NULL);
	}
	free(*buff);
	*buff = remaining_buffer;
	return (next_line);
}

/**
 * @brief Function to get the next line from a file descriptor.
 * 
 * Get the next line from a file descriptor and return it as a heap string.
 * Line is terminated with either \n\0 if appropriate. or \0 if it is the "last"
 * line of the file
 * 
 * The remainder gets stored in a buffer_remaining heap string that keeps track
 * of the letters that have been read but not yet "used." To manage multiple FDs
 * these are kept in a static char pointer array with one slot for each possible
 * FD.
 * 
 * @param fd The file descriptor to read from
 * @return char* The heap string
 */
char    *get_next_line(int fd)
{
    static char *buff_remaining[FD_MAX];

	int		new_line_found;
	ssize_t	line_len = find_end_char(buff_remaining[fd], &new_line_found);
	int		reached_end_of_fd = 0;
	while (!new_line_found && reached_end_of_fd == 0)
	{
		reached_end_of_fd = grow_buffer(&(buff_remaining[fd]), fd);
		if (reached_end_of_fd == -1)
		{
			free(buff_remaining[fd]);
			return (NULL);
		}
		line_len = find_end_char(buff_remaining[fd], &new_line_found);
	}
	char	*next_line = extract_next_line(&(buff_remaining[fd]), line_len);
	if (reached_end_of_fd && next_line[0] == '\0')
	{
		free(next_line);
		if (buff_remaining[fd])
		{
			free(buff_remaining[fd]);
			buff_remaining[fd] = NULL;
		}
		return (NULL);
	}
	return (next_line);
}