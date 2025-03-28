/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcampo <marcampo@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 19:33:03 by marcampo          #+#    #+#             */
/*   Updated: 2024/08/01 12:47:46 by marcampo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	va_router(va_list *arg, const char *str);

static int	va_router(va_list *arg, const char *str)
{
	int	count;

	count = 0;
	if (str[0] == 'c')
		count = ft_print_char(va_arg(*arg, int));
	else if (str[0] == 's')
		count = ft_print_str(va_arg(*arg, char *));
	else if (str[0] == 'p')
		count = ft_print_ptr(va_arg(*arg, void *));
	else if (str[0] == 'd' || str[0] == 'i')
		count = ft_print_nbr(va_arg(*arg, int));
	else if (str[0] == 'u')
		count = ft_print_unsigned(va_arg(*arg, int));
	else if (str[0] == 'x')
		count = ft_print_hex(va_arg(*arg, int), 0);
	else if (str[0] == 'X')
		count = ft_print_hex(va_arg(*arg, int), 1);
	else if (str[0] == '%')
		count = ft_print_percent();
	return (count);
}

int	ft_printf(const char *str, ...)
{
	va_list		ap;
	int			i;
	int			count;

	if (!str)
		return (-1);
	i = 0;
	count = 0;
	va_start(ap, str);
	while (str[i] != 0)
	{
		if (str[i] == '%' && str[i + 1])
		{
			count += va_router(&ap, &str[i + 1]);
			i++;
		}
		else
		{
			ft_putchar_fd(str[i], 1);
			count++;
		}
		i++;
	}
	va_end(ap);
	return (count);
}
