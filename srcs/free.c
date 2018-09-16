/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/17 22:14:34 by angavrel          #+#    #+#             */
/*   Updated: 2018/09/16 21:26:49 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

t_chunk	*get_page(t_chunk *chunk, size_t *type)
{
	t_page	*page;
	size_t	i;
	size_t	j;
	t_chunk	*current_chunk;

	ft_printf("\n\n%p chunk\n\n\n", chunk);
	i = 0;
	while (i < 3)
	{
		page = g_page[i];
		j = page->chunk_nb;
		current_chunk = page->chunk;
		while (j)
		{ft_printf("\n\n%p %pchunk\n\n\n", current_chunk, chunk);
		//	ft_printf("\ni %zu    (%zu)\n\n", i, j);
			if (current_chunk->size)
			{
			//	ft_printf("\nchunksize %zu\n\n", current_chunk->size);
				--j;
				if (current_chunk == chunk)
				{
				//	printf("\nTROUVE %p\n\n", &current_chunk);
					--page->chunk_nb;
					page->current_size -= current_chunk->size;
				//	printf("\n %zu chunks left\n\n", page->chunk_nb);
					*type = i;
					if (current_chunk->next)
					{
						current_chunk->next->prev = current_chunk->prev;

					}
					if (current_chunk->prev)
					{
						current_chunk->prev->next = current_chunk->next;
					//	current_chunk->prev = NULL;
					}
					else
						ft_printf("WAAAAAZAAAAA\n");////*type |= 0b100;

				//	ft_bzero(current_chunk, sizeof(*current_chunk));
					return (chunk);
				}
			}
			current_chunk = current_chunk->next;
		}
		++i;
	}
	return (NULL);
}

/*
** type is 0 for tiny, 1 for small and 2 for large, adding 4 mask if it was the
** very first link.
*/

static inline void	free_chunk(t_chunk *chunk)
{
	t_chunk		*chunk_to_be_fred;
	size_t		type;

	type = 0;
	chunk_to_be_fred = get_page(chunk, &type);
	ft_printf("chunk: %p\n", chunk);
	ft_printf("size to be removed: %zu\n", chunk->size);

//	ft_printf("type: %d\n", type & 0b100);
	if (type & 0b100)
	{
		ft_printf("\n\n\npage chunk: %p     : page chunk next %p\n\n\n", g_page[type & 0b11]->chunk, g_page[type & 0b11]->chunk->next);
		g_page[type & 0b11]->chunk = g_page[type & 0b11]->chunk->next;
		if (g_page[type & 0b11]->chunk != NULL)
			g_page[type & 0b11]->chunk->prev = NULL;

	}
	munmap(chunk, chunk->size);// reduce use of this <
	chunk = NULL;
}


void    free(void *ptr)
{
//	ft_printf("\n\n%p chunk\n\n\n", ptr);
	if (ptr)
		free_chunk(ptr - sizeof(t_chunk));
}
