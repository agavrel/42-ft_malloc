/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/17 22:13:21 by angavrel          #+#    #+#             */
/*   Updated: 2018/10/13 21:15:42 by angavrel         ###   ########.fr       */
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

void	*realloc(void *ptr, size_t size)
{
	void		*new_ptr;
	t_chunk		*original_chunk;

	new_ptr = malloc(size);
	if (new_ptr != ptr)
	{
		original_chunk = ptr;
		if (size > original_chunk->max_size)
			size = original_chunk->max_size;
		ft_memcpy(new_ptr, ptr, size);
		free(ptr);
	}
	return (new_ptr);
}

/*
** The calloc() function contiguously allocates enough space for count objects
** that are size bytes of memory each and returns a pointer to the allocated
** memory.  The allocated memory is filled with bytes of value zero.
*/

void			*calloc(size_t count, size_t size)
{
	void			*ptr;

	if ((ptr = malloc(size)))
		ft_memset(ptr, 0, ft_align(count * size, 0xf));
	return (ptr);
}
