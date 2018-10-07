/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/17 22:14:34 by angavrel          #+#    #+#             */
/*   Updated: 2018/10/07 19:58:35 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

/*
** Remove page if it contains no other chunks left
*/

static inline void		free_page(t_page *page)
{
	if (page->prev)
		page->prev->next = page->next;
	if (page->next)
		page->next->prev = page->prev;
	page = NULL;
}
/*
** chunk being fred have their size set to 0 but they keep the max size info
*/

static inline void		free_chunk(t_chunk *chunk)
{
	chunk->size = 0;
}

/*
** The chunk should be contained in the page hence we now need to find it
*/

static inline void		get_chunk_that_has_to_be_fred(t_page *page, void *ptr)
{
	t_chunk		*chunk;

	chunk = (t_chunk *)page->first_chunk;
	while (chunk)
	{
		if (chunk == ptr)
		{
			free_chunk(ptr);
			--page->chunk_nb;
			if (page->chunk_nb == 0)
				free_page(page);
		}
		chunk = chunk->next;
	}
}

/*
** the page that contain
*/

static inline void		get_page_containing_chunk(void *ptr)
{
	t_page		*page;
	size_t		i;

	i = 0;
	while (i < 3)
	{
		page = g_page[i];
		while (page)
		{
			if ((char *)page < (char *)ptr \
						&& (char *)ptr < (char *)page + page->max_size)
				get_chunk_that_has_to_be_fred(page, ptr);
			page = page->next;
		}
		++i;
	}
}

/*
** first check that the *ptr is not null, if not we try to find a memory page
** containg the ptr as a memory block (t_chunk)
*/

void    				free(void *ptr)
{


	if (ptr)
		get_page_containing_chunk(ptr);
}
