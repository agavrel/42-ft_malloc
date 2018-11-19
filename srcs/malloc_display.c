/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_display.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/20 18:46:19 by angavrel          #+#    #+#             */
/*   Updated: 2018/11/19 17:24:51 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static inline size_t	show_alloc_tiny_small(t_page *mem, \
								const char *size_str)
{
	t_block				*block;
	size_t				total;

	total = 0;
	while (mem)
	{
		ft_printf("%s : %p\n", size_str, mem);
		block = mem->alloc;
		while (block)
		{
			ft_printf("%p - %p : %lu bytes\n", (void *)block + \
			sizeof(t_block), (void *)block + sizeof(t_block) + \
			block->size, block->size);
			total += block->size;
			block = block->next;
		}
		mem = mem->next;
	}
	return (total);
}

static inline size_t	show_alloc_large(void)
{
	t_block			*block;
	size_t			total;

	total = 0;
	ft_printf("LARGE : %p\n", g_malloc_pages.large);
	block = g_malloc_pages.large;
	while (block)
	{
		ft_printf("%p - %p : %lu bytes\n", (void *)block + \
		sizeof(t_block), (void *)block + sizeof(t_block) + \
		block->size, block->size);
		total += block->size;
		block = block->next;
	}
	return (total);
}

void					show_alloc_mem(void)
{
	size_t				total;

	total = 0;
	pthread_mutex_lock(&g_malloc_mutex);
	if (g_malloc_pages.tiny)
		total += show_alloc_tiny_small(g_malloc_pages.tiny, "TINY");
	if (g_malloc_pages.small)
		total += show_alloc_tiny_small(g_malloc_pages.small, "SMALL");
	if (g_malloc_pages.large)
		total += show_alloc_large();
	ft_printf("Total : %lu bytes\n", total);
	pthread_mutex_unlock(&g_malloc_mutex);
}

static inline size_t	dump_line(const char *ptr, const size_t size)
{
	size_t				i;

	ft_printf("%.16p: ", ptr);
	i = 0;
	while (i < 0x10)
	{
		if (!(i & 1))
			ft_printf(" ");
		if (size <= i)
			ft_printf("  ");
		else
			ft_printf("%02x", ptr[i]);
		i++;
	}
	ft_printf("  ");
	i = 0;
	while (i < 0x10 && size > i)
	{
		ft_isprint(ptr[i]) ? ft_printf("%c", ptr[i]) : ft_printf(".");
		i++;
	}
	return (i);
}

void					show_alloc_mem_hex(void *ptr)
{
	const char			*zone_str[3] = {"Tiny", "Small", "Large"};
	size_t				size;
	size_t				i;

	pthread_mutex_lock(&g_malloc_mutex);
	if (ptr)
	{
		size = ((t_block *)(ptr - sizeof(t_block)))->size;
		ft_printf("%s memory area of %lu bytes starting at %p:\n", \
			zone_str[page_size(size)], size, ptr);
		while (size)
		{
			i = dump_line(ptr, size);
			size -= i;
			ptr += i;
			ft_printf("\n");
		}
	}
	else
		ft_printf("Memory address [%p] was not allocated by malloc\n", ptr);
	pthread_mutex_unlock(&g_malloc_mutex);
}
