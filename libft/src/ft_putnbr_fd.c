/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcampo <marcampo@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 13:18:00 by marcampo          #+#    #+#             */
/*   Updated: 2024/06/11 13:18:13 by marcampo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	digit_count(long n)
{
	int	len;

	len = 1;
	while (n / 10)
	{
		len++;
		n = n / 10;
	}
	return (len);
}

static long	negative_case(long n, int fd)
{
	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		n = n * -1;
	}
	return (n);
}

void	ft_putnbr_fd(int n, int fd)
{
	int		i;
	int		j;
	int		len;
	long	tmp;
	long	nbr;

	i = 0;
	j = 0;
	nbr = (long)n;
	nbr = negative_case(nbr, fd);
	tmp = nbr;
	len = digit_count(nbr);
	while (i < len)
	{
		while (j < (len - i - 1))
		{
			tmp = (tmp / 10);
			j++;
		}
		ft_putchar_fd((tmp % 10) + '0', fd);
		j = 0;
		i++;
		tmp = nbr;
	}
}
