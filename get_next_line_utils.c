#include "get_next_line.h"

/**
 * @brief Creates a new buff object
 * 
 * Creates an empty buff object. And fills it with zero values. Note
 * specifically that it creates a free-able string with one '\0' char (an
 * "empty" string) so that downstream behaviour is consistent. Downstream users
 * don't have to check for existance of a string. There is always a string (even
 * if 0 length) in buff->str and never a NULL. The empty value is an empty
 * string ("\0").
 * 
 * @return t_buff* The newly created buff object.
 */
t_buff	*new_buff(void)
{
	t_buff *buff;

	buff = malloc(sizeof (*buff) * 1);
	if (!buff)
	{
		return (NULL);
	}
	buff->fd = 0;
	buff->str = malloc(sizeof (*buff->str) * 1);
	if (!buff->str)
	{
		free(buff);
		return (NULL);
	}
	buff->str[0] = '\0';
	buff->next = NULL;
	return (buff);
}

/**
 * @brief Gets the pointer to a buffer with corresponding fd identifier.
 * 
 * Passed the head to a list of buff objects. Finds the object with the
 * corresponding fd identifier (sort of like a key, but it's not truly a
 * key:value pair dictionary) and returns that object.
 * 
 * If that buff object doesn't exist. Creates it and tacks it onto the end of
 * the list.
 * 
 * If the list was empty, Creates it and makes it the head node.
 * 
 * @param list_of_buffers Head node of the linked list of buff objects.
 * @param fd The file descriptor key, and also the fd to read from when reading.
 * @return t_buff* The corresponding buff object (regardless of which route it
 * was found or created)
 */
t_buff	*get_buff(t_buff **list_of_buffers, int fd)
{
	t_buff *buff;
	t_buff *next_buff;

	if (*list_of_buffers)
	{
		buff = *list_of_buffers;
		while (buff->fd != fd && buff->next)
		{
			buff = buff->next;
		}
		if (buff->fd == fd)
		{
			return (buff);
		}
	}
	next_buff = new_buff();
	if (!next_buff)
	{
		return (NULL);
	}
	next_buff->fd = fd;
	if (!(*list_of_buffers))
	{
		*list_of_buffers = next_buff;
	}
	else
	{
		buff->next = next_buff;
	}
	return (next_buff);
}

/**
 * @brief Destroys a buff object
 * 
 * Destroys a buff object by freeing correctly.
 * 
 * @param buff The buff object to be free'd
 */
void	destroy_buff(t_buff *buff)
{
	free(buff->str);
	free(buff);
	return ;
}

/**
 * @brief Removes a buff object with corresponding key
 * 
 * Given the head of a buff object linked list. Removes the buff object and
 * fixes the linked list. Either removes that link and joins the ends. Or if it
 * is the head it removes it and resets the head pointer.
 * 
 * If a node with that key doesn't exist it does nothing.
 * 
 * @param list_of_buffers_head Head object of the list of buffers
 * @param fd The fd key to look for
 */
void	remove_buff(t_buff **list_of_buffers_head, int fd)
{
	t_buff *buff;
	t_buff *tmp;

	if (!*list_of_buffers_head)
	{
		return ;
	}
	buff = *list_of_buffers_head;
	if (buff->fd == fd)
	{
		*list_of_buffers_head = buff->next;
		destroy_buff(buff);
		return ;
	}
	while (buff->next)
	{
		if (buff->next->fd == fd)
		{
			tmp = buff->next->next;
			destroy_buff(buff->next);
			buff->next = tmp;
			return ;
		}
		buff = buff->next;
	}
	return ;
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