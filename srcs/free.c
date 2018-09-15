/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/17 22:14:34 by angavrel          #+#    #+#             */
/*   Updated: 2018/09/15 23:43:26 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

t_chunk	*get_page(t_chunk *chunk, size_t *type)
{
	t_page	*page;
	size_t	i;
	size_t	j;

	ft_printf("\n\n%p chunk\n\n\n", chunk);
	while (i < 3)
	{
		page = g_page[i];
		j = 0;
		while (j < page->chunk_nb)
		{
			ft_printf("\n\ni %zu    (j %zu < max %zu)\n\n\n", i, j, page->chunk_nb);
			if (&page->chunk == chunk)
			{
				ft_printf("\n\nTROUVE %p\n\n\n", &page->chunk);
				--page->chunk_nb;
				*type = i;
				return (&page->chunk);
			}
			if (page->chunk.size)
			{
				ft_printf("\n\nchunksize %zu\n\n\n", page->chunk.size);
				++j;
			}
			page->chunk = *page->chunk.next;
		}
		++i;
	}
	return (NULL);
}


static inline void	free_chunk(t_chunk *chunk)
{
	t_chunk		*chunk_to_be_fred;
	size_t		size;
	size_t		type;


	chunk_to_be_fred = get_page(chunk, &type);
	size = chunk->size;
	ft_printf("size to be removed: %zu\n", size);
	chunk->size = 0;
	munmap(chunk, size);
	chunk = NULL;
}


void    free(void *ptr)
{
	ft_printf("\n\n%p chunk\n\n\n", ptr);
	if (ptr)
		free_chunk(ptr - sizeof(t_chunk));
}
