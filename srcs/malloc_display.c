/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_display.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/20 18:46:19 by angavrel          #+#    #+#             */
/*   Updated: 2018/09/15 19:15:33 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static inline size_t	show_alloc_tiny_small(t_page *page, const char *str)
{
	t_chunk		*chunk;
	size_t		total;
	void		*current_ptr;

	while (page)
	{
		ft_printf("%s : %p\n", str, page);
		chunk = &page->chunk;
		while (chunk)
		{
			current_ptr = (void *)chunk + sizeof(t_chunk);
			ft_printf("%p - %p : %lu bytes\n", current_ptr, current_ptr + \
			chunk->size, chunk->size);
			total += chunk->size;
			chunk = chunk->next;
		}
		page = page->next;
	}
	return total;
}

static inline size_t	show_alloc_large(void)
{
	t_chunk		*chunk;
	size_t		total;
	void		*current_ptr;

	ft_printf("LARGE : %p\n", g_page[2]);
	chunk = &g_page[2]->chunk;
	while (chunk)
	{
		current_ptr = (void *)chunk + sizeof(t_chunk);
		ft_printf("%p - %p : %lu bytes\n", current_ptr, current_ptr + \
		chunk->size, chunk->size);
		total += chunk->size;
		chunk = chunk->next;
	}
	return total;
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
