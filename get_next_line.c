#include "get_next_line.h"

/**
 * @brief Finds end char of a string. Either a \n or \0 char.
 * 
 * Searches a string for first instance of the end of a line (which will either
 * be a \n or a \0 if it is the last line). To keep track of this there is a var
 * passed by reference to keep a flag of whether it was a \n or \0 that was
 * found.
 * 
 * @param buff The buffer object to search
 * @param new_line_found Integer passed as reference for flag if new line is
 * found. 1 if \n found. 0 if \0 found.
 * @return size_t The position of the found char in the string. In practice this
 * is the length of the string up to the line terminating char.
 */
static size_t	find_end_char(t_buff *buff, int *new_line_found)
{
	size_t i = 0;
	*new_line_found = 0;
	while (buff->str[i])
	{
		if (buff->str[i] == '\n')
		{
			*new_line_found = 1;
			return (i);
		}
		i++;
	}
	return (i);
}

/**
 * @brief Expands a buff object by adding the next string from read.
 * 
 * Takes a buffer object. Runs read and concats the new read buffer onto
 * the end of the buffer object's current string.
 * 
 * @param buff The buff object we want to extend.
 * @return int Return flag. 0 if successfully reads and adds on. -1 if error.
 * 1 if end of file reached.
 */
static int	grow_buffer(t_buff *buff)
{
	char new_buff[BUFF_SIZE + 1];

	ssize_t chars_read = read(buff->fd, new_buff, BUFF_SIZE);
	if (chars_read == -1)
	{
		return (-1);
	}
	if (chars_read == 0)
	{
		return (1);
	}
	new_buff[chars_read] = '\0';
	size_t	str_len = 0;
	while (buff->str[str_len])
	{
		str_len++;
	}
	str_len += chars_read;
	char *tmp = malloc(sizeof (*tmp) * (str_len + 1));
	if (!tmp)
	{
		return (1);
	}
	size_t i = 0;
	while (buff->str[i])
	{
		tmp[i] = buff->str[i];
		i++;
	}
	size_t j = 0;
	while (new_buff[j])
	{
		tmp[i + j] = new_buff[j];
		j++;
	}
	tmp[i + j] = '\0';
	free(buff->str);
	buff->str = tmp;
	return (0);
}

/**
 * @brief Returns the first line from a buff str and replaces the leftover str.
 * 
 * Takes a buff object. Splits the buff string at end position (inclusive). Adds
 * null chars if needed. Replaces buff's str with the remaining string.
 * 
 * @param buff The buffer to work with.
 * @param line_end_position The end position to split at.
 * @return char* The first part of the string that is being sliced off.
 */
static char	*extract_next_line(t_buff *buff, size_t line_end_position)
{
	char	*next_line;

	if (buff->str[line_end_position] == '\n')
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
		next_line[i] = buff->str[i];
		i++;
	}
	next_line[i] = '\0';
	char	*remaining_str = ft_strslice(buff->str, line_end_position, SIZE_MAX);
	if (!remaining_str)
	{
		free(next_line);
		return (NULL);
	}
	free(buff->str);
	buff->str = remaining_str;
	return (next_line);
}

/**
 * @brief Function to get the next line from a file descriptor.
 * 
 * Get the next line from a file descriptor and return it as a heap string. \0
 * char is included in the line if the line ends with it.
 * 
 * How much gets read on each call of read is determined by BUFFER_SIZE. This is
 * reduced to a maximum reasonable if BUFFER_SIZE is too large by a define
 * macro.
 * 
 * Any remaining characters get stored in a buff object for use later. The buff
 * objects also form a linked list so that they can all be referenced by one
 * head variable.
 * 
 * @param fd The file descriptor to read from.
 * @return char* The heap string representation of the next line.
 */
char    *get_next_line(int fd)
{
	static t_buff *list_of_buffers;
	t_buff	*this_buff;

	this_buff = get_buff(&list_of_buffers, fd);

	int		new_line_found;
	ssize_t	line_len = find_end_char(this_buff, &new_line_found);
	int		reached_end_of_fd = 0;
	while (!new_line_found && reached_end_of_fd == 0)
	{
		reached_end_of_fd = grow_buffer(this_buff);
		if (reached_end_of_fd == -1)
		{
			remove_buff(&list_of_buffers, fd);
			return (NULL);
		}
		line_len = find_end_char(this_buff, &new_line_found);
	}
	char	*next_line = extract_next_line(this_buff, line_len);
	if (reached_end_of_fd && next_line[0] == '\0')
	{
		free(next_line);
		remove_buff(&list_of_buffers, fd);
		return (NULL);
	}
	return (next_line);
}