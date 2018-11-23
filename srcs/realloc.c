/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/17 22:13:21 by angavrel          #+#    #+#             */
/*   Updated: 2018/11/19 19:25:09 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

/*
** Go through list of block and check if we can identify a valid block address
** corresponding to the pointer sent. We add the header sizeof(t_block).
*/

bool	is_valid_block(const void *ptr, size_t size)
{
	void		*block_addr;
	t_block		*block;

	if (size > (1 << 10))
		block = g_malloc_pages.large;
	else if (size > (1 << 6))
		block =  (t_block*)g_malloc_pages.small + sizeof(t_page);
	else
		block =  (t_block*)g_malloc_pages.tiny + sizeof(t_page);
	block_addr = block + sizeof(t_block);
	while (block && block_addr < ptr)
	{
		if (block_addr == ptr)
			return (true);
		block = block->next;
		block_addr = block + sizeof(t_block);
	}
	return (false);
}

/*
** The realloc() function tries to change the size of the allocation pointed to
** by ptr to size, and returns ptr.  If there is not enough room to enlarge the
** memory allocation pointed to by ptr, realloc() creates a new allocation,
** copies as much of the old data pointed to by ptr as will fit to the new
** allocation, frees the old allocation, and returns a pointer to the allocated
** memory.  If ptr is NULL, realloc() is identical to a call to malloc() for
** size bytes.  If size is zero and ptr is not NULL, a new, minimum sized object
** is allocated and the original object is freed.  When extending a region
** allocated with calloc(3), realloc(3) does not guarantee that the additional
** memory is also zero-filled.
**
** 1/ Check that ptr and size are not NULL
** 2/ Check that the ptr is a valid one corresponding to a memory block
** 3/ Check that the new size is inferior to the reallocating ptr size
** and if so proceed to a succesful memory reallocation
** 4/ if not execute a regular malloc and then free the reallocating ptr
*/

void	*realloc(void *ptr, size_t size)
{
	void		*new;
	t_block		*block_header;

	if (!ptr || !size)
		return (malloc(size));
	pthread_mutex_lock(&g_malloc_mutex);
	if (!is_valid_block(ptr, size))
	{
		pthread_mutex_unlock(&g_malloc_mutex);
		return (NULL);//(void*)malloc_error(1, "could not reallocate its memory"));
	}
	block_header = ptr - sizeof(t_block);
	if (size <= block_header->size)
	{
		block_header->size = size;
		pthread_mutex_unlock(&g_malloc_mutex);
		return (ptr);
	}
	pthread_mutex_unlock(&g_malloc_mutex);
	if (!(new = malloc(size)))
		return (NULL);
	ft_memcpy(new, ptr, block_header->size);
	free(ptr);
	return (new);
}

/*
** The reallocf() function is identical to the realloc() function, except that
** it will free the passed pointer when the requested memory cannot be
** allocated. This is a FreeBSD specific API designed to ease the problems with
** traditional coding styles for realloc causing memory leaks in libraries.
*/

void	*reallocf(void *ptr, size_t size)
{
	void		*new;

	if (!(new = realloc(ptr, size)))
		free(ptr);
	return (new);
}

/*
** The calloc() function contiguously allocates enough space for count objects
** that are size bytes of memory each and returns a pointer to the allocated
** memory.  The allocated memory is filled with bytes of value zero.
*/

void	*calloc(size_t count, size_t size)
{
	void			*ptr;

	if ((ptr = malloc(count * size)))
		ft_bzero(ptr, count * size);
	return (ptr);
}
