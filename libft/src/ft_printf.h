/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcampo <marcampo@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 18:59:24 by marcampo          #+#    #+#             */
/*   Updated: 2024/07/18 19:54:06 by marcampo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stdio.h>
# include <stdlib.h>
# include <stdarg.h>
# include <unistd.h>
# include <limits.h>
# include "../libft/src/libft.h"

int		ft_print_hex(long num, int upper);
int		ft_printf(const char *str, ...);
int		ft_print_char(char c);
int		ft_print_str(char *str);
int		ft_digit_count(long n);
int		ft_print_percent(void);
int		ft_print_nbr(int num);
int		ft_print_unsigned(unsigned int n);
int		ft_print_ptr(void *ptr);
void	ft_putchar_hex(int start, char *buf, int size);
char	*ft_strcpy(char *dst, const char *src);

#endif
