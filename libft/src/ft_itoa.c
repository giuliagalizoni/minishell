/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcampo <marcampo@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 13:02:55 by marcampo          #+#    #+#             */
/*   Updated: 2024/06/11 13:03:49 by marcampo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	digit_count(long n)
{
	size_t	len;

	len = 1;
	while (n / 10)
	{
		len++;
		n = n / 10;
	}
	return (len);
}

static char	*pos_int(long n, size_t len)
{
	char	*ascii;
	size_t	i;

	ascii = malloc(len * sizeof(char));
	if (ascii == NULL)
		return (NULL);
	i = 0;
	while (i < (len - 1))
	{
		ascii[len - i - 2] = (n % 10) + '0';
		n = n / 10;
		i++;
	}
	ascii[len - 1] = '\0';
	return (ascii);
}

static char	*neg_int(long n, size_t len)
{
	char	*ascii;
	size_t	i;

	i = 0;
	n = n * -1;
	len++;
	ascii = malloc(len * sizeof(char));
	if (ascii == NULL)
		return (NULL);
	while (i < (len - 2))
	{
		ascii[(len - 1) - i - 1] = (n % 10) + '0';
		n = n / 10;
		i++;
	}
	ascii[0] = '-';
	ascii[len -1] = '\0';
	return (ascii);
}

char	*ft_itoa(int n)
{
	char	*ascii;
	size_t	len;
	long	num;

	num = (long)n;
	len = digit_count(num) + 1;
	if (num < 0)
		ascii = neg_int(num, len);
	else if (num >= 0)
		ascii = pos_int(num, len);
	else
		ascii = NULL;
	return (ascii);
}
