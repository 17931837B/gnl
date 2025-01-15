/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tobaba <tobaba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 12:41:54 by tobaba            #+#    #+#             */
/*   Updated: 2024/06/08 09:36:26 by tobaba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*check_end(char *str)
{
	int	len;

	len = ft_strlen(str);
	while (*str != '\0')
	{
		if (*str == (char)('\n'))
			return ((char *)str);
		str++;
	}
	return (0);
}

static char	*set_line(char *line)
{
	char	*stock;
	ssize_t	i;

	i = 0;
	while (line[i] != '\n' && line[i] != '\0')
		i++;
	if (line[i] == '\0')
		return (NULL);
	stock = ft_substr(line, i + 1, ft_strlen(line) - i);
	if (stock == NULL)
		return (NULL);
	if (*stock == 0)
	{
		free(stock);
		stock = NULL;
	}
	line[i + 1] = '\0';
	return (stock);
}

void	free_and_null(void **ptr)
{
	if (ptr != NULL && *ptr != NULL)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

static char	*get_line(int fd, char *stock, char *buff)
{
	ssize_t	check_read;
	char	*tmp;

	check_read = 1;
	while (check_read > 0)
	{
		check_read = read(fd, buff, BUFFER_SIZE);
		if (check_read == -1)
			return (free(stock), stock = NULL, NULL);
		else if (check_read == 0)
			break ;
		buff[check_read] = '\0';
		if (stock == NULL)
			stock = ft_strdup("");
		if (stock == NULL)
			return (NULL);
		tmp = stock;
		stock = ft_strjoin(tmp, buff);
		if (stock == NULL)
			return (NULL);
		free_and_null((void **)&tmp);
		if (check_end(buff))
			break ;
	}
	return (stock);
}

char	*get_next_line(int fd)
{
	static char	*stock;
	char		*line;
	char		*buff;

	buff = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (buff == NULL)
		return (NULL);
	if (fd < 0 || read(fd, 0, 0) < 0)
	{
		free(stock);
		free(buff);
		stock = NULL;
		buff = NULL;
	}
	line = get_line(fd, stock, buff);
	free(buff);
	buff = NULL;
	if (line == NULL)
		return (NULL);
	stock = set_line(line);
	return (line);
}
