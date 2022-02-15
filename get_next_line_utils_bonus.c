/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lxu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 16:09:19 by lxu               #+#    #+#             */
/*   Updated: 2022/02/15 16:09:23 by lxu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

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
	t_buff	*buff;

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
	t_buff	*buff;
	t_buff	*next_buff;

	if (*list_of_buffers)
	{
		buff = *list_of_buffers;
		while (buff->fd != fd && buff->next)
			buff = buff->next;
		if (buff->fd == fd)
			return (buff);
	}
	next_buff = new_buff();
	if (!next_buff)
		return (NULL);
	next_buff->fd = fd;
	if (!(*list_of_buffers))
		*list_of_buffers = next_buff;
	else
		buff->next = next_buff;
	return (next_buff);
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
	t_buff	*buff;
	t_buff	*tmp;

	if (!*list_of_buffers_head)
		return ;
	buff = *list_of_buffers_head;
	if (buff->fd == fd)
	{
		*list_of_buffers_head = buff->next;
		free(buff->str);
		free(buff);
		return ;
	}
	while (buff->next)
	{
		if (buff->next->fd == fd)
		{
			tmp = buff->next->next;
			free(buff->next->str);
			free(buff->next);
			buff->next = tmp;
			return ;
		}
		buff = buff->next;
	}
}

/**
 * @brief Calculates length of a string.
 * 
 * @param str String to measure.
 * @return size_t Number of chars in string. Not including the null terminator.
 */
size_t	ft_strlen(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		i++;
	}
	return (i);
}

/**
 * @brief Takes two strings and returns new heap string of the two joined.
 * 
 * @param s1 The first string to join.
 * @param s2 The second string to append.
 * @return char* A new heap string with the two strings joined.
 */
char	*ft_strjoin(char *s1, char *s2)
{
	size_t	i;
	size_t	j;
	char	*result;

	result = malloc(sizeof (*result) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		result[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		result[i + j] = s2[j];
		j++;
	}
	result[i + j] = '\0';
	return (result);
}
