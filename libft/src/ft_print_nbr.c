/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_nbr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcampo <marcampo@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 19:01:05 by marcampo          #+#    #+#             */
/*   Updated: 2024/07/18 19:01:07 by marcampo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_print_nbr(int num)
{
	int	count;

	if (num < 0)
		count = ft_digit_count(num) + 1;
	else
		count = ft_digit_count(num);
	ft_putnbr_fd(num, 1);
	return (count);
}

int	ft_print_unsigned(unsigned int n)
{
	int		i;
	int		j;
	int		len;
	long	tmp;
	long	nbr;

	i = 0;
	j = 0;
	nbr = (long)n;
	tmp = nbr;
	len = ft_digit_count(nbr);
	while (i < len)
	{
		while (j < (len - i - 1))
		{
			tmp = (tmp / 10);
			j++;
		}
		ft_putchar_fd((tmp % 10) + '0', 1);
		j = 0;
		i++;
		tmp = nbr;
	}
	return (len);
}
