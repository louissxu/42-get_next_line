#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# define FD_MAX (1024 + 1)
# define BUFF_SIZE (BUFFER_SIZE > 10 || BUFFER_SIZE < 1 ? 10 : BUFFER_SIZE)

#include <stdlib.h>
#include <unistd.h>

char    *get_next_line(int fd);

#endif