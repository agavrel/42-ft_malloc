/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/03 20:09:34 by angavrel          #+#    #+#             */
/*   Updated: 2018/11/19 18:11:43 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static void	prefix_nbr(char *buff, int size_base)
{
	if (size_base == 2)
	{
		buff[0] = '0';
		buff[1] = 'b';
	}
	else if (size_base == 16)
	{
		buff[0] = '0';
		buff[1] = 'x';
	}
}

void		ft_putsizebase(size_t nb, int size_base)
{
	static const char	base[16] = {"0123456789abcdef"};
	char				buff[66];
	long				power;
	size_t				tmp;
	int					i;

	if (size_base < 2 || size_base > 16)
		return ;
	power = 1;
	tmp = nb;
	while (tmp /= size_base)
		power *= size_base;
	i = size_base == 2 || size_base == 16 ? 2 : 0;
	prefix_nbr(buff, size_base);
	while (power)
	{
		buff[i++] = base[nb / power];
		nb -= (nb / power) * power;
		power /= size_base;
	}
	write(1, buff, i);
}

void		putaddr(void *addr)
{
	ft_putsizebase((size_t)addr, 16);
}

/*
** Align memory on (mask + 1) bytes with (mask + 1) being a power of 2
*/

size_t		ft_align(size_t size, size_t mask)
{
	return ((size + mask) & ~mask);
}

/*
** returns page's size according to the type: Tiny if (type == 0),
** Small if (type == 1), Large if (type == 2)
*/

size_t		page_size(size_t size)
{
	static const size_t	malloc_size[3] = {MALLOC_TINY, MALLOC_SMALL, \
											MALLOC_LARGE};
	const size_t	type = (size > (1 << 6)) + (size > (1 << 10));

	return (malloc_size[type]);
}
