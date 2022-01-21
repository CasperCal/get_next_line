#include "get_next_line.h"

/*
 We free mem allocated for *str of **str and set the string itself to NULL.
 */

void    ft_free(char **str)
{
    if (*str != NULL)
    {
        free(*str);
        str = NULL;
    }
}
 /*
  We get the index and buffer, if the index is in the fd we check if it's the end of the str, if it is we free the *buffer.
  We take the substring of length buffersize, out of our buffer starting at the index
  We store our buffer into the result, reaset the index to 0, and now store the previously found substring in the buffer. (thus saving the position)
  */
char    *ft_join_line(int newl_position, char **buf)
{
    char *tmp;
    char *res;
    
    tmp = NULL;
    if (newl_position <= 0)
    {
        if (**buf == '\0')
        {
            free(*buf);
            *buf = NULL;
            return(NULL);
        }
        res = *buf;
        *buf = NULL;
        return(res);
    }
    tmp = ft_substr(*buf, newl_position, BUFFER_SIZE);
    res = *buf;
    res[newl_position] = 0;
    *buf = tmp;
    return (res);
}
/*
 >
 We use ft_strchr to find the first occurence of \n and we store it's pointer in newl
 If there is no \n we use the read funtion to store the amount of bytes of fd in bytes_read, if this <(error) or = 0 we call ft_join_line
    we add read_ret to our buffer via ft_strjoin
 */
char    *ft_read(int fd, char **buf, char *read_ret)
{
    size_t  byte_count;
    char    *newl;
    char    *tmp;
    
    newl = ft_strchr(*buf, '\n');
    tmp = NULL;
    byte_count = 0;
    while (newl == NULL)
    {
        byte_count = read(fd, read_ret, BUFFER_SIZE);
        if (byte_count <= 0)
            return(ft_join_line(byte_count, buf));
        read_ret[byte_count] = 0;
        tmp = ft_strjoin(*buf, read_ret);
        ft_free(buf);
        *buf = tmp;
        newl = ft_strchr(*buf, '\n');
    }
    return (ft_join_line(newl - *buf + 1, buf));
}

/*
Checks first if given paramters are ok -> if not return NULL for error
Assigns memory for read_ret + checks if assignment worked
Creates an empty buffer
Calls ft_read funtion to find result (the line)
We free the memort allocated for read_ret and return the result
*/

char    *get_next_line(int fd)
{
    static char *buf[4096];
    char        *read_ret;
    char        *res;
    
    if ( fd < 0 || fd > 4095 || BUFFER_SIZE <= 0 )
        return NULL;
    read_ret = (char *)malloc(sizeof(char) * BUFFER_SIZE + 1);
    if (read_ret == NULL)
        return NULL;
    if (!buf[fd])
        buf[fd] = ft_strdup("");
    res = ft_read(fd, &buf[fd], read_ret);
    ft_free(&read_ret);
    return (res);
}
