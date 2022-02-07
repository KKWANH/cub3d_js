/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kimkwanho <kimkwanho@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/15 12:06:47 by kimkwanho         #+#    #+#             */
/*   Updated: 2021/02/20 18:32:17 by kimkwanho        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void				*ft_util_bzero(void *s, size_t n)
{
	size_t			i;
	unsigned char	*suc;

	i = 0;
	suc = (unsigned char *)s;
	while (i < n)
	{
		suc[i] = 0;
		i++;
	}
	s = suc;
	return (suc);
}

size_t				ft_util_strlen(const char *str)
{
	size_t			cnt;

	cnt = 0;
	if (!str)
		return (0);
	while (str[cnt])
		cnt++;
	return (cnt);
}

void				*ft_util_memcpy(void *d, const void *s, size_t n)
{
	char		*dstc;
	const char	*srccc;

	if ((d == s) || n == 0)
		return (d);
	if (!d && !s)
		return (0);
	dstc = (char *)d;
	srccc = (const char *)s;
	while (n--)
		dstc[n] = srccc[n];
	return (d);
}

char				*ft_util_strjoin(char *st1, char *st2)
{
	char			*rst;
	size_t			ln1;
	size_t			ln2;

	if (!st1 && !st2)
		return (NULL);
	if (!st1 || !st2)
		return (!st1 ? ft_util_strdup(st2) : ft_util_strdup(st1));
	ln1 = ft_util_strlen(st1);
	ln2 = ft_util_strlen(st2);
	if (!(rst = (char *)malloc(sizeof(char) * (ln1 + ln2 + 1))))
		return (NULL);
	ft_util_strlcpy(rst, st1, ln1 + 1);
	free(st1);
	ft_util_strlcpy(rst + ln1, st2, ln2 + 1);
	return (rst);
}

char				*ft_util_strdup(const char *st1)
{
	char	*result;
	int		i;

	i = ft_util_strlen(st1);
	result = (char *)malloc(sizeof(char) * (i + 1));
	if (!result)
		return (0);
	i = 0;
	while (st1[i])
	{
		result[i] = st1[i];
		i++;
	}
	result[i] = '\0';
	return (result);
}
