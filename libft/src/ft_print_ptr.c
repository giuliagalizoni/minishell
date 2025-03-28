/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_ptr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcampo <marcampo@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 19:26:16 by marcampo          #+#    #+#             */
/*   Updated: 2024/07/18 19:56:47 by marcampo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_print_ptr_hex(unsigned long num);

int	ft_print_ptr(void *ptr)
{
	unsigned long	addr;
	int				len;

	addr = (unsigned long)ptr;
	if (!addr)
		return (ft_print_str("(nil)"));
	ft_print_str("0x");
	len = ft_print_ptr_hex(addr);
	return (len + 2);
}

static int	ft_print_ptr_hex(unsigned long num)
{
	char	hex_chars[17];
	char	buf[sizeof(void *) * 2 + 1];
	int		i;
	int		start;
	int		size;

	size = sizeof(unsigned long) * 2;
	ft_strcpy(hex_chars, "0123456789abcdef");
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
