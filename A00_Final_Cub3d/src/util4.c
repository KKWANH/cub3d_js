/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kimkwanho <kimkwanho@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/15 12:06:58 by kimkwanho         #+#    #+#             */
/*   Updated: 2021/02/28 21:30:58 by kimkwanho        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

size_t				ft_util_num_length(int nbr)
{
	int				len;

	len = 0;
	if (nbr == 0)
		return (1);
	if (nbr < len)
		len += 1;
	while (nbr != 0)
	{
		nbr = nbr / 10;
		len++;
	}
	return (len);
}

char				*ft_util_itoa(int n)
{
	size_t			i;
	size_t			n_size;
	char			*str;

	i = 0;
	if (n == -2147483648)
		return (ft_util_strdup("-2147483648"));
	n_size = ft_util_num_length(n);
	str = (char *)malloc(sizeof(char) * (n_size + 1));
	if (str == 0)
		return (0);
	str[n_size] = 0;
	if (n < 0)
	{
		str[0] = '-';
		n *= -1;
		i += 1;
	}
	while (i < n_size--)
	{
		str[n_size] = (n % 10) + '0';
		n /= 10;
	}
	return (str);
}

int					ft_util_atoi(const char *str)
{
	int					sgn;
	unsigned long int	rst;
	size_t				idx;

	idx = 0;
	sgn = 1;
	rst = 0;
	while (str[idx] == '\t' || str[idx] == '\n' || str[idx] == '\v' ||
			str[idx] == '\f' || str[idx] == '\r' || str[idx] == ' ')
		++idx;
	if (str[idx] == '-' || str[idx] == '+')
	{
		if (str[idx] == '-')
			sgn *= -1;
		++idx;
	}
	while ((str[idx] >= '0' && str[idx] <= '9'))
	{
		rst = rst * 10 + (str[idx] - '0');
		++idx;
	}
	return (rst * sgn);
}

char				*ft_util_strjoin_nofree(char *st1, char *st2)
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
	ft_util_strlcpy(rst + ln1, st2, ln2 + 1);
	return (rst);
}
