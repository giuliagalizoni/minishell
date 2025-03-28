/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcampo <marcampo@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 19:21:53 by marcampo          #+#    #+#             */
/*   Updated: 2024/07/22 15:52:54 by marcampo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_handle_hex_zero(void);

static int	ft_handle_hex_zero(void)
{
	write(1, "0", 1);
	return (1);
}

static void	*ft_fill_hex_chars(char *hex_chars, int upper)
{
	if (upper)
		ft_strcpy(hex_chars, "0123456789ABCDEF");
	else
		ft_strcpy(hex_chars, "0123456789abcdef");
	return (0);
}

static size_t	ft_get_num_size(long num)
{
	size_t	size;

	if (num > 2147483647 || num < -2147483648)
		size = sizeof(unsigned long) * 2;
	else
		size = sizeof(unsigned int) * 2;
	return (size);
}

int	ft_print_hex(long num, int upper)
{
	char	hex_chars[17];
	char	buf[sizeof(void *) * 2 + 1];
	int		i;
	int		start;
	int		size;

	if (num == 0)
		return (ft_handle_hex_zero());
	if (num < 0)
		num = ~(num * -1) + 1;
	size = ft_get_num_size(num);
	ft_fill_hex_chars(hex_chars, upper);
	i = size - 1;
	while (i >= 0)
	{
		buf[i] = hex_chars[num & 0xf];
		if (buf[i] != '0')
			start = i;
		num >>= 4;
		i--;
	}
	buf[size] = '\0';
	ft_putchar_hex(start, buf, size);
	return (ft_strlen(buf + start));
}
