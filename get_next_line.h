#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# define FD_MAX (1024 + 1)
# define BUFF_SIZE (BUFFER_SIZE > 1000 || BUFFER_SIZE < 1 ? 1000 : BUFFER_SIZE)

#include <stdlib.h>
#include <unistd.h>

typedef struct	s_buff
{
	int				fd;
	char			*str;
	struct s_buff	*next;
}	t_buff;

char    *get_next_line(int fd);

t_buff	*new_buff(void);
t_buff	*get_buff(t_buff **list_of_buffers_head, int fd);
void	destroy_buff(t_buff *buff);
void	remove_buff(t_buff **list_of_buffers_head, int fd);
char	*ft_strslice(char *str, size_t left, size_t right);

#endif