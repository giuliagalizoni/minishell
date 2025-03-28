/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcampo <marcampo@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 19:00:51 by marcampo          #+#    #+#             */
/*   Updated: 2024/07/18 19:54:48 by marcampo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_digit_count(long n)
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

void	ft_putchar_hex(int start, char *buf, int size)
{
	if (start == size - 1)
		write(1, buf + start, 1);
	else
		write(1, buf + start, size - start);
}
