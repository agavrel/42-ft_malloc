/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/17 22:14:34 by angavrel          #+#    #+#             */
/*   Updated: 2018/11/19 19:20:19 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

/*static void			free_unused_mem(const int malloc_size, t_page *mem)
{
}*/

static inline void	free_not_large(t_block *block, \
						const int malloc_size, t_page *mem)
{
	if (block->prev)
		block->prev->next = block->next;
	else
		mem->alloc = block->next;
	if (block->next)
		block->next->prev = block->prev;
	block->prev = NULL;
	block->next = mem->free;
	if (mem->free)
		mem->free->prev = block;
	mem->free = block;
	if (!mem->alloc)
	{
		if (mem->prev)
			mem->prev->next = mem->next;
		else if (malloc_size)
				g_malloc_pages.tiny = mem->next;
		else
				g_malloc_pages.small = mem->next;
		if (mem->next)
			mem->next->prev = mem->prev;
		munmap(mem, MALLOC_ZONE << (6 + (malloc_size << 2)));
	}
}

/*
** the large block is a big one
** we free the area aligned on 4096 bytes
*/

static inline void	free_large(t_block *block)
{
	const size_t	msize = ft_align(block->size + sizeof(t_block), MASK_0XFFF);

	if (block->prev)
		block->prev->next = block->next;
	else
		g_malloc_pages.large = block->next;
	if (block->next)
		block->next->prev = block->prev;
	munmap(block, msize);
}

/*
** We free block according to the types
** g_malloc_pages.tiny + sizeof(t_page) * type' is used to access tiny or small
*/

static void			free_block(t_block *block)
{
	const int		type = page_size(block->size);
	size_t const	zone_sizes[2] = {ZONE_TINY, ZONE_SMALL};
	t_page			*mem;

	if (type == MALLOC_LARGE)
		free_large(block);
	else
	{
		mem = g_malloc_pages.tiny + sizeof(t_page) * type;
		while ((void *)mem + MALLOC_ZONE * zone_sizes[type] <= (void *)block \
						&& (void *)block <= (void *)mem)
			mem = mem->next;
		free_not_large(block, type, mem);
	}
}

void				free(void *ptr)
{
	pthread_mutex_lock(&g_malloc_mutex);
	if (ptr && is_valid_block(ptr, ZONE_SMALL + 1))
		free_block(ptr - sizeof(t_block));
	pthread_mutex_unlock(&g_malloc_mutex);
}
