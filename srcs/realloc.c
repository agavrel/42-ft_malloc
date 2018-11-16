/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/17 22:13:21 by angavrel          #+#    #+#             */
/*   Updated: 2018/11/16 22:17:00 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

/*
** The realloc() function tries to change the size of the allocation pointed to
** by ptr to size, and returns ptr.
** If there is not enough room to enlarge the memory allocation pointed to by
** ptr, realloc() creates a new allocation, copies as much of the old data
** pointed to by ptr as will fit to the new allocation, frees the old
** allocation, and returns a pointer to the allocated memory.
** If ptr is NULL, realloc() is identical to a call to malloc() for
** size bytes.  If size is zero and ptr is not NULL, a new, minimum sized object
** is allocated and the original object is freed.  When extending a region
** allocated with calloc(3), realloc(3) does not guarantee that the
** additional memory is also zero-filled.
*/

static inline int	is_reallocable(t_block *block, size_t size)
{
	const int		malloc_size = MALLOC_SIZE(block->size);

	if (malloc_size == MALLOC_TINY)
		return (size <= ZONE_TINY);
	else if (malloc_size == MALLOC_SMALL)
		return (size <= ZONE_SMALL);
	return (size + sizeof(t_block) <= \
			MALLOC_PAGE(block->size + sizeof(t_block)));
}

static inline void	*bitter_realloc(void *ptr, size_t size)
{
	void			*new;
	size_t			old_size;

	if (!(new = malloc(size)))
		return (NULL);
	old_size = ((t_block *)(ptr - sizeof(t_block)))->size;
	ft_memcpy(new, ptr, old_size);
	free(ptr);
	return (new);
}

void				*realloc(void *ptr, size_t size)
{
	if (!ptr || !size)
		return (malloc(size));
	pthread_mutex_lock(&g_malloc_mutex);
	if (malloc_out_of_zones(ptr))
	{
		pthread_mutex_unlock(&g_malloc_mutex);
		return (NULL);
	}
	if (is_reallocable(ptr - sizeof(t_block), size))
	{
		((t_block *)(ptr - sizeof(t_block)))->size = size;
		pthread_mutex_unlock(&g_malloc_mutex);
		return (ptr);
	}
	pthread_mutex_unlock(&g_malloc_mutex);
	return (bitter_realloc(ptr, size));
}

void				*reallocf(void *ptr, size_t size)
{
	void			*new;

	if (!(new = realloc(ptr, size)))
		free(ptr);
	return (new);
}

void				*calloc(size_t count, size_t size)
{
	const size_t	area = count * size;
	void			*ptr;


	if ((ptr = malloc(area)))
		ft_bzero(ptr, area);
	return (ptr);
}
