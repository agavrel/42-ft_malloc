/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/14 18:06:26 by angavrel          #+#    #+#             */
/*   Updated: 2018/10/05 22:46:48 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

t_page			*g_page[3] = {0, 0, 0};
pthread_mutex_t	g_lock;

/*
** Creates a chunk of memory
** if (size & 0x1f) is used to align memory to 32 bytes
** chunk + 1 to prevent erasing data of the struct
*/

static t_chunk *chunk_create(void* addr, const size_t size, t_chunk *prev)
{
//	ft_printf("%p Sucessfully created chunk\n", chunk);
	t_chunk		*chunk;

	chunk = addr;
	chunk->size = size;//(size & 0x1f) ? size - (size & 0x1f) + 0x20 : size;ft_printf("GAAAc\n");
//	ft_printf("%p %zu chunk size\n", (char*)chunk ,chunk->size);

	chunk->next = NULL;
	chunk->prev = prev;
	ft_printf("%lu\n", size);
	getchar();
	return (chunk);
}

size_t			page_size(size_t type)
{
	ft_printf("page type= %lu\n", type);
	if (type == 0)
		return (128);
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
	const size_t	type = (size > MALLOC_TINY) + (size > MALLOC_SMALL);


	if ((*page = mmap(NULL, type, PROT_READ | PROT_WRITE,
		MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
			return (NULL);

	(*page)->max_size = page_size(type);
	(*page)->chunk_nb = 1;
	(*page)->next = NULL;
	(*page)->prev = NULL;

	ft_printf("%p  page init didnt fail\n\n", *page);
	ft_printf("-----\npage current size: %zu\n", (*page)->max_size);

	return (chunk_create((*page)->first_chunk, size + sizeof(t_chunk), NULL));
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
** creates a chained-list of free memory areas
*/

static void		*malloc_handler(const size_t size, t_page **page, size_t type)
{
	t_chunk		*chunk;
	t_chunk		*prev;
	t_page		*current_page;

	if (!*page)
		return (page_init(size, page));
	current_page = *page;
	prev = NULL;
	while (current_page)
	{
		chunk = current_page->first_chunk;
		while (chunk)
		{
			if (chunk->size == 0 && chunk->max_size >= sizeof(t_chunk) + size)
			{
				current_page->chunk_nb += 1;
				return (chunk_create(chunk, size + sizeof(t_chunk), prev));
			}
			if (chunk->next)
				prev  = chunk;
			chunk = chunk->next;
		}
		if (current_page->next == NULL && chunk + sizeof(t_chunk) \
			+ size > (char*)current_page + page_size(type))
			return (page_create(size, page));
		current_page = current_page->next;
	}
	return (page_create(size, page));
}

/*
** allocates memory accordingly to the desired size
*/

static void 	*malloc_type(size_t size, int type)
{
	ft_printf("-----\ntiny: %zu\n", size);
	void	*ptr;
	t_page	**page = &g_page[type];

	ptr = malloc_handler(size, page, type);
	//ft_printf("page current size: %zu\n", (*page)->max_size);



	return (ptr);
}

/*
** malloc_type : using function pointer instead of 'if / else if'
** &lock is used for hyperthreading
*/

void			*ft_malloc(size_t size)
{
	void			*ptr;
	const size_t	type = (size > MALLOC_TINY) + (size > MALLOC_SMALL);

	//ft_printf("malloc 0 1 2 : %zu    size: %zu\n", i, size);//
	if (size == 0)
		return (NULL);
	pthread_mutex_init(&g_lock, NULL);
	pthread_mutex_lock(&g_lock);
	ptr = malloc_type(size, type);
//	g_page[0].chunk_nb = 1;
//	ft_printf("\n\n%p ~ Malloc done\n\n", ptr);//
	pthread_mutex_unlock(&g_lock);
//	show_alloc_mem();
	return ptr;
}
