/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_display.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/20 18:46:19 by angavrel          #+#    #+#             */
/*   Updated: 2018/10/06 18:19:48 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static inline size_t	show_alloc_tiny_small(t_page *page, const char *str)
{
	t_chunk		*chunk;
	size_t		total;
	size_t		pages_nb;

	total = 0;
	pages_nb = 0;
	while (page)
	{
		ft_printf("%s : %p\n", str, page);
		chunk = (t_chunk *)page->first_chunk;
		while (chunk)
		{
			putaddr((void *)chunk);
			ft_printf(" - ");
			putaddr((void *)chunk + chunk->size);
			ft_printf(" : %lu bytes\n", chunk->size);
			total += chunk->size;
			chunk = chunk->next;
		}
		page = page->next;
		++pages_nb;
	}
	ft_printf("Number of pages : %lu\n", pages_nb);
	return (total);
}

static inline size_t	show_alloc_large(void)
{
	t_chunk		*chunk;
	size_t		total;

	total = 0;
	ft_printf("LARGE : %p\n", g_page[2]);
	chunk = (t_chunk *)g_page[2]->first_chunk;
	while (chunk)
	{
		putaddr((void *)chunk);
		ft_printf(" - ");
		putaddr((void *)chunk + chunk->size);
		ft_printf(" : %lu bytes\n", chunk->size);
		total += chunk->size;
		chunk = chunk->next;
	}
	return (total);
}

void					show_alloc_mem(void)
{
	size_t		total;

	total = 0;
	pthread_mutex_lock(&g_lock);
	if (g_page[0])
		total += show_alloc_tiny_small(g_page[0], "TINY");
	if (g_page[1])
		total += show_alloc_tiny_small(g_page[1], "SMALL");
	if (g_page[2])
		total += show_alloc_large();
	ft_printf("Total : %lu bytes\n", total);
	pthread_mutex_unlock(&g_lock);
}
