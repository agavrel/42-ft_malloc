/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/17 22:14:34 by angavrel          #+#    #+#             */
/*   Updated: 2018/11/16 21:49:06 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static void			free_unused_mem(const int malloc_size, t_page *mem)
{
	size_t const	zone_sizes[2] = {ZONE_TINY, ZONE_SMALL};

	if (mem->prev)
		mem->prev->next = mem->next;
	else if (malloc_size)
		g_malloc_pages.small = mem->next;
	else
		g_malloc_pages.tiny = mem->next;
	if (mem->next)
		mem->next->prev = mem->prev;
	munmap(mem, MALLOC_ZONE * zone_sizes[malloc_size]);
}

static inline void	free_tiny_small(t_block *chunk, \
						const int malloc_size, t_page *mem)
{
	if (chunk->prev)
		chunk->prev->next = chunk->next;
	else
		mem->alloc = chunk->next;
	if (chunk->next)
		chunk->next->prev = chunk->prev;
	chunk->prev = NULL;
	chunk->next = mem->free;
	if (mem->free)
		mem->free->prev = chunk;
	mem->free = chunk;
	if (!mem->alloc)
		free_unused_mem(malloc_size, mem);
}

static inline void	free_large(t_block *chunk)
{
	const size_t	msize = MALLOC_PAGE(chunk->size + sizeof(t_block));

	if (chunk->prev)
		chunk->prev->next = chunk->next;
	else
		g_malloc_pages.large = chunk->next;
	if (chunk->next)
		chunk->next->prev = chunk->prev;
	munmap(chunk, msize);
}

static void			free_chunk(t_block *chunk)
{
	const int		malloc_size = MALLOC_SIZE(chunk->size);
	size_t const	zone_sizes[2] = {ZONE_TINY, ZONE_SMALL};
	void			*mem_zones[2] = {g_malloc_pages.tiny, g_malloc_pages.small};
	t_page	*mem;

	if (malloc_size == MALLOC_LARGE)
		free_large(chunk);
	else
	{
		mem = mem_zones[malloc_size];
		while (!((void *)chunk < (void *)mem + MALLOC_ZONE * \
			zone_sizes[malloc_size] && (void *)chunk > (void *)mem))
			mem = mem->next;
		free_tiny_small(chunk, malloc_size, mem);
	}
}

void				free(void *ptr)
{
	pthread_mutex_lock(&g_malloc_mutex);

	if (!(!ptr || malloc_out_of_zones(ptr)))
		free_chunk(ptr - sizeof(t_block));
	pthread_mutex_unlock(&g_malloc_mutex);
}
