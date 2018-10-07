/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/14 18:06:26 by angavrel          #+#    #+#             */
/*   Updated: 2018/10/07 20:05:22 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

/*
** g_page contains TINY, SMALL and LARGE areas of memory
*/

t_page			*g_page[3] = {0, 0, 0};
pthread_mutex_t	g_lock;

/*
** chunk_create is to creates a chunk ~ a block of memory
** if (size & 0x1f) is used to align memory to 32 bytes
** chunk + 1 to prevent erasing data of the struct
*/

static t_chunk *chunk_create(void* addr, const size_t size, t_chunk *prev)
{
//	ft_printf("%p Sucessfully created chunk\n", chunk);
	t_chunk		*chunk;

	chunk = addr;
	chunk->size = size;//(size & 0x1f) ? size - (size & 0x1f) + 0x20 : size;ft_printf("GAAAc\n");
	//ft_printf("%p %zu chunk size\n", (char*)chunk ,chunk->size);
	if (!chunk->max_size)
	 	chunk->max_size = chunk->size;
	chunk->next = NULL;
	chunk->prev = prev;
	if (chunk->prev)
		chunk->prev->next = chunk;
	return (chunk);
}

/*
** returns page's size according to the type: Tiny, 1: Small, 2: Large
*/

size_t			page_size(size_t type)
{
	if (type == 0)
		return (256);
	if (type == 1)
		return (1024);
	return (65536);
}

/*
** Initialize new memory areas called 'page'
** while (g_page[0]) is to mmap a new area
** Then mmap is used to allocate memory
** Finally we create a chunk corresponding to the size requested by the user
*/

static t_chunk *page_init(const size_t size, t_page **page)
{
	t_chunk			*chunk;
	t_page			*this_page;
	const size_t	type = (size - sizeof(t_chunk) > MALLOC_TINY) \
		+ (size  - sizeof(t_chunk) > MALLOC_SMALL);

	this_page = *page;
	if ((this_page = mmap(NULL, page_size(type), PROT_READ | PROT_WRITE,
		MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
			return (NULL);
	this_page->max_size = page_size(type);
	this_page->chunk_nb = 1;
	this_page->next = NULL;
	this_page->prev = NULL;
	return (chunk_create(this_page->first_chunk, size, NULL));
}

static t_chunk *page_create(const size_t size, t_page **page)
{
	t_chunk		*chunk;
	t_page		*prev;
	t_page		*current;

	current = *page;
	while (current && current->next)
		current = current->next;
	prev = current;
	current = current->next;
	chunk = page_init(size, &current);
	prev->next = current;
	current->prev = prev;

	return (chunk);
}

/*
** malloc_handler function looks for of free memory area
** The first page has already been initialized in malloc_type
** Hence we know that we have to create a chunk if 1/ there is enough mem space
** with if (chunk->size == 0 && chunk->max_size >= sizeof(t_chunk) + size)
** or 2/ if there is enough memory space left on the page with if ((char *)chunk
** + sizeof(t_chunk) + size <= (char*)current_page +  page_size(type))
*/

static void		*malloc_handler(const size_t size, t_page **page, size_t type)
{
	t_chunk		*chunk;
	t_chunk		*prev;
	t_page		*cur_page;
	size_t		page_size;

	cur_page = *page;
	page_size = sizeof(t_page);
	while (cur_page)
	{
		chunk = (t_chunk *)cur_page->first_chunk;
		while (chunk)
		{
			if (chunk->size == 0 && chunk->max_size >= size \
				&& ++cur_page->chunk_nb)
				return (chunk_create(chunk, size, prev));
			page_size += chunk->max_size;
			prev  = chunk;
			chunk = chunk->next;
		}
		if (page_size + size < cur_page->max_size && ++cur_page->chunk_nb)
		{
			ft_printf("heeey");
			return (chunk_create((char *)cur_page->first_chunk \
				+ page_size, size, prev));
		}

		cur_page = cur_page->next;
	}
	return (page_create(size, page));
}

/*
** allocates memory accordingly to the desired size
*/

static void 	*malloc_type(size_t size, int type)
{
	t_page	**page;

	page = &g_page[type];
	if (!(*page))
	{
		ft_printf("\nheeaaaey %p\n", page);
		return (page_init(size + sizeof(t_chunk), page));
	}

	return (malloc_handler(size + sizeof(t_chunk), page, type));
}

/*
** malloc_type : using function pointer instead of 'if / else if'
** &lock is used for hyperthreading
*/

void			*malloc(size_t size)
{
	void			*ptr;
	const size_t	type = (size > MALLOC_TINY) + (size > MALLOC_SMALL);
	ft_printf("ENTERING MALLOC OF SIZE %lu\n", size);//
	if (size == 0)
		return (NULL);
	pthread_mutex_init(&g_lock, NULL);
	pthread_mutex_lock(&g_lock);
	ptr = malloc_type(size, type);
	pthread_mutex_unlock(&g_lock);

	ft_printf("checking malloc\n", ptr);//

	int i = 0;
	while (i < size)
	{ft_printf("%d\n", i);//
		((char *)ptr)[i] = 42;
		i++;
	}

	ft_printf("EXITING MALLOC %p\n", ptr);//
	return (ptr);
}
