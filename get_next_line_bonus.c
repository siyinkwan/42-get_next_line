/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguan <sguan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 19:29:55 by sguan             #+#    #+#             */
/*   Updated: 2024/12/17 21:35:39 by sguan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static int	check_newline(char *buffer)
{
	int	i;

	i = 0;
	while (buffer[i])
	{
		if (buffer[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

static char	*extract_line(char **leftover, size_t newline_index)
{
	char	*line;
	char	*temp;

	line = ft_substr(*leftover, 0, newline_index + 1);
	if (!line)
		return (NULL);
	temp = ft_strdup(*leftover + newline_index + 1);
	free(*leftover);
	*leftover = temp;
	return (line);
}

static char	*update_leftover(char **leftover, char *buffer)
{
	char	*temp;

	if (*leftover)
	{
		temp = ft_strjoin(*leftover, buffer);
		free(*leftover);
		*leftover = temp;
	}
	else
		*leftover = ft_strdup(buffer);
	return (*leftover);
}

static char	*read_and_process(int fd, char **leftover)
{
	char	*buffer;
	ssize_t	bytes_read;
	int		newline_index;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		if (!update_leftover(leftover, buffer))
			return (free(buffer), NULL);
		newline_index = check_newline(*leftover);
		if (newline_index >= 0)
			return (free(buffer), extract_line(leftover, newline_index));
		bytes_read = read(fd, buffer, BUFFER_SIZE);
	}
	free(buffer);
	if (bytes_read == -1)
	{
		free(*leftover);
		*leftover = NULL;
	}
	return (NULL);
}

char	*get_next_line(int fd)
{
	static char	*leftover[FOPEN_MAX];
	char		*line;
	int			newline_index;

	if (fd < 0 || fd > FOPEN_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	if (leftover[fd])
	{
		newline_index = check_newline(leftover[fd]);
		if (newline_index >= 0)
			return (extract_line(&leftover[fd], newline_index));
	}
	line = read_and_process(fd, &leftover[fd]);
	if (line)
		return (line);
	if (leftover[fd] && *leftover[fd])
	{
		line = ft_strdup(leftover[fd]);
		free(leftover[fd]);
		leftover[fd] = NULL;
		return (line);
	}
	free(leftover[fd]);
	leftover[fd] = NULL;
	return (NULL);
}

//#include <fcntl.h>
//#include <stdio.h>

//int	main(void)
//{
//	int	fd1 = open("test1.txt", O_RDONLY);
//	int fd2 = open("test2.txt", O_RDONLY);
//	char	*line;
//	int		i;

//	i = 0;
//	line = (char *)1;
//	while (line)
//	{
//		line = get_next_line(fd1);
//		if (line)
//		{
//			printf("%s", line);
//			free(line);
//		}
//		line = get_next_line(fd2);
//		if (line)
//		{
//			printf("%s", line);
//			free(line);
//		}
//		i++;
//	}
//	if (line)
//		free(line);
//	close(fd1);
//	close(fd2);
//}
