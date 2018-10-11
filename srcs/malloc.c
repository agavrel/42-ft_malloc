/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/14 18:06:26 by angavrel          #+#    #+#             */
/*   Updated: 2018/10/11 21:05:11 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

/*
** g_page contains TINY, SMALL and LARGE areas of memory
*/

t_page			*g_page[3] = {NULL, NULL, NULL};
pthread_mutex_t	g_lock;

/*
** Align memory on (2 ^ align) bytes with 'align' > 0
*/


static	size_t	ft_align(size_t size, size_t align)
{
	return (((size + align - 1) >> align) << align);
}

/*
** chunk_create is to creates a chunk ~ a block of memory
*/

static t_chunk *chunk_create(t_chunk *chunk, size_t size, t_chunk *prev)
{
	chunk->size = ft_align(size, 4);
	ft_printf("\n\n\nsuccessfully created chunk at address %p of size %zu and previous chunk:   %p\n\n\n\n", (char*)chunk->data, chunk->size, prev);
	if (!chunk->max_size)
	 	chunk->max_size = chunk->size;
	chunk->next = NULL;
	chunk->prev = prev;
	if (prev)
		prev->next = chunk;
	return (chunk);
}

/*
** returns page's size according to the type: Tiny if (type == 0),
** Small if (type == 1), Large if (type == 2)
*/

size_t			page_size(size_t type)
{
	static const size_t	malloc_size[3] = {M_TINY, M_SMALL, M_LARGE};

	return (malloc_size[type]);
}

/*
** Initialize new memory areas called 'page'
** while (g_page[0]) is to mmap a new area
** Then mmap is used to allocate memory
** Finally we create a chunk corresponding to the size requested by the user
*/

static t_page **page_init(const size_t size)
{
	t_page		*page;
	size_t		mmap_size;
	const size_t	type = (size + sizeof(t_chunk) > M_TINY) \
		+ (size + sizeof(t_chunk) > M_SMALL);

	mmap_size = page_size(type);
	if ((page = mmap(NULL, size, PROT_READ | PROT_WRITE,
		MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
			return (NULL);
	page->max_size = mmap_size;
	page->chunk_nb = 1;
	page->next = NULL;
	page->prev = NULL;
	chunk_create((t_chunk *)page->first_chunk, size, NULL);
	return (&page);
}

static t_chunk *page_create(const size_t size, t_page **page)
{
	t_chunk		**chunk;
	t_page		*prev;
	t_page		*current;

	ft_printf("\n\nhelooooo\n\n\n");//
	current = *page;
	while (current && current->next)
		current = current->next;
	prev = current;
	current = current->next;
	prev->next = current;
	current->prev = prev;

	return (*page_init(size));
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

	chunk = (t_chunk *)cur_page->first_chunk;
	ft_printf("entering malloc handler\n\n");
	cur_page = *page;//ft_printf("chunk size %lu\n\n", chunk->size);
	page_size = sizeof(t_page);
	while (cur_page)
	{
		chunk = (t_chunk *)cur_page->first_chunk;
		while (chunk)
		{
			ft_printf("chunk size %lu\n\n", chunk->size);
			if (chunk->size == 0 && chunk->max_size >= size \
				&& ++cur_page->chunk_nb)
				return (chunk_create(chunk, size, (t_chunk *)chunk->prev->data));

			page_size += chunk->max_size;
			prev  = chunk;
			chunk = chunk->next;
		}
		if (page_size + size < cur_page->max_size && ++cur_page->chunk_nb)
		{
			ft_printf("creating new chunk in the page\n");
			return (chunk_create((char *)cur_page->first_chunk \
				+ page_size, size, (t_chunk *)prev->data));
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

	page = &g_page[type];
	if (*page)
		return (malloc_handler(size, page, type));
	ft_printf("\nCREATING NEW PAGE %p %lu\n", page, type);//
	return (page = page_init(size));
}

/*
** malloc_type : using function pointer instead of 'if / else if'
** &lock is used for hyperthreading
*/

void			*malloc(size_t size)
{ft_putstr("ENTERING budsOF SIZE \n");//
	void			*ptr;
	const size_t	type = (size > M_TINY) + (size > M_SMALL);
	ft_printf("ENTERING MALLOC OF SIZE %lu\n", size);//
	if (size == 0)
		return (NULL);
	pthread_mutex_init(&g_lock, NULL);
	pthread_mutex_lock(&g_lock);
	ptr = malloc_type(size + sizeof(t_chunk), type);
	pthread_mutex_unlock(&g_lock);

	ft_printf("checking malloc\n", ptr);//

	int i = 0;
	while (i < size)
	{


		((char *)ptr)[i] = 42;
		if (i % 20)
			ft_printf("%d\n", i);//
		i++;
	}

	ft_printf("EXITING MALLOC %p\n", ptr);//
	return (ptr);
}
