/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcampo <marcampo@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 13:53:56 by marcampo          #+#    #+#             */
/*   Updated: 2024/11/28 17:50:53 by marcampo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long	ft_atol(const char *str)
{
	long	res;
	char	sign;

	res = 0;
	sign = '+';
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '+' || *str == '-')
		sign = *str++;
	while ((*str >= '0' && *str <= '9'))
		res = (res * 10) + (*str++ - '0');
	if (sign == '-')
		res *= -1;
	return (res);
}
