/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/14 18:06:26 by angavrel          #+#    #+#             */
/*   Updated: 2018/10/13 21:17:20 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

/*
** g_page contains TINY, SMALL and LARGE areas of memory
*/

t_page			*g_page[3] = {NULL, NULL, NULL};
pthread_mutex_t	g_lock;

/*
** chunk_create is to creates a chunk ~ a block of memory
*/

static void		*chunk_create(t_chunk *chunk, size_t size, t_chunk *prev)
{
	//ft_printf("\n\naddress %p", chunk);
	chunk->size = ft_align(size, 0xf);
	if (!chunk->max_size)
	 	chunk->max_size = chunk->size;
	chunk->next = NULL;
	if (prev)
	{
		chunk->prev = prev;
		prev->next = chunk;
		//ft_printf("and previous (*chunk):   %p\n\n\n\n", prev);
	}


	//ft_printf("\nsuccessfully created (*chunk) at address %p of size %zu\n", chunk, chunk->size);
	return (chunk->data);
}

/*
** Initialize new memory areas called 'page'
** while (g_page[0]) is to mmap a new area
** Then mmap is used to allocate memory
** Finally we create a chunk corresponding to the size requested by the user
*/

static t_chunk *page_init(const size_t size, t_page **page)
{
	size_t		mmap_size;
	t_chunk		*chunk;
	const size_t	type = (size - sizeof(t_chunk) > M_TINY) \
		+ (size - sizeof(t_chunk) > M_SMALL);

	mmap_size = page_size(type);
	if ((*page = mmap(NULL, mmap_size, PROT_READ | PROT_WRITE,
		MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
			return (NULL);
	(*page)->max_size = mmap_size;
	(*page)->chunk_nb = 1;
	(*page)->next = NULL;
	(*page)->prev = NULL;
	chunk = chunk_create((t_chunk *)(*page)->first_chunk, size, NULL);

	return (chunk);
}

static t_chunk *page_create(const size_t size, t_page **page)
{
	t_page		*prev;
	t_page		*current;


	current = *page;
	while (current && current->next)
		current = current->next;
	prev = current;
	current = current->next;
	prev->next = current;
	current->prev = prev;

	return (page_init(size, &current) - sizeof(t_page));
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
	t_page		*prev_page;


	//ft_printf("entering malloc handler \n\n");
	cur_page = *page;////ft_printf("chunk size %lu\n\n", chunk->size);
	page_size = sizeof(t_page);
	while (cur_page)
	{
		chunk = (t_chunk *)cur_page->first_chunk;
		//ft_printf("size %lu\naddress %p\n", chunk->size, cur_page->first_chunk);
		while (chunk)
		{
			//ft_printf("chunk size %lu\n\n", chunk->size);
			if (chunk->size == 0 && chunk->max_size >= size \
				&& ++cur_page->chunk_nb)
				return (chunk_create(chunk, size, (t_chunk*)chunk->prev));

			page_size += chunk->max_size;
			prev  = chunk;
			chunk = chunk->next;
		}
		if (page_size + size < cur_page->max_size && ++cur_page->chunk_nb)
		{
			//ft_printf("creating new chunk in the page %p\n", (char*)prev + prev->size);
			return (chunk_create((t_chunk *)(prev + prev->size), size, NULL));
		}
		prev_page = cur_page;
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
	t_chunk	*chunk;

	page = &g_page[type];
	if (*page)
		return (malloc_handler(size, page, type));
	//ft_printf("\nCREATING NEW PAGE %p of type %lu\n", page, type);//


	return page_init(size, page);
}

/*
** malloc_type : using function pointer instead of 'if / else if'
** &lock is used for hyperthreading
*/

void			*malloc(size_t size)
{
	void			*ptr;
	const size_t	type = (size > M_TINY) + (size > M_SMALL);

	ft_printf("\n\n\n---------------------\nENTERING MALLOC OF SIZE %lu\n", size);//
	if (size == 0)
		return (NULL);
	pthread_mutex_init(&g_lock, NULL);
	pthread_mutex_lock(&g_lock);
	ptr = malloc_type(size + sizeof(t_chunk), type);
	pthread_mutex_unlock(&g_lock);


	int i = 0;
	char *s = ptr;
	while (i < size)
	{
		s[i] = 42;
		i++;
	}


	ft_printf("EXITING MALLOC %p\n", ptr);//
	return (ptr);
}
