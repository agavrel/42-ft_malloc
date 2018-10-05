/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/17 22:14:34 by angavrel          #+#    #+#             */
/*   Updated: 2018/10/05 21:44:20 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

/*
** chunk being fred have their size set to 0
*/

static inline void	free_chunk(t_chunk *chunk)
{
	chunk->size = 0;
}

/*
** The chunk should be contained in the page hence we now need to find it
*/

static inline void	get_chunk_to_free(t_chunk *chunk, void *ptr)
{
	while (chunk)
	{
		if (chunk == ptr)
		{
			ft_printf("freeing chunk: %p       %p\n", ptr, chunk);
			return free_chunk(ptr);
		}
		chunk = chunk->next;
	}
}

/*
** the page that contain
*/

static inline void	get_page_to_free(void *ptr)
{
	t_page	*page;

	size_t	i;

	i = 0;
	while (i < 3)
	{
		page = g_page[i];
		while (page)
		{
			if (page < ptr && ptr < (char *)page + page->max_size)
				get_chunk_to_free(page->first_chunk, ptr);
			page = page->next;
		}
		++i;
	}
}

void    			free(void *ptr)
{
	ft_printf("\n\n%p chunk\n\n\n", ptr);
	size_t	type;
	t_chunk	*chunk;

	if (ptr)
	{
		chunk = ptr;
	//	type = (chunk->size > MALLOC_TINY) \
		 	+ (chunk->size > MALLOC_SMALL);
		get_page_to_free(ptr);
	}
}
