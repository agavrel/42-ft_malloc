/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/14 18:06:26 by angavrel          #+#    #+#             */
/*   Updated: 2018/09/16 16:20:44 by angavrel         ###   ########.fr       */
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

static void *chunk_create(const size_t size, t_chunk *chunk)
{
	while (chunk->size){
	//	ft_printf("%p current chunk %zu\n", chunk, chunk->size);
		chunk = chunk->next;
	//	ft_printf("%p test chunk2 \n", chunk);
	}
//	ft_printf("%p Sucessfully created chunk\n", chunk);
	chunk->size = size;//(size & 0x1f) ? size - (size & 0x1f) + 0x20 : size;ft_printf("GAAAc\n");
//	ft_printf("%p %zu chunk size\n", (char*)chunk ,chunk->size);

	//getchar();
	chunk->next = (void *)chunk + chunk->size;

//	ft_printf("%p chunk next\n", chunk->next);
	chunk->next->prev = chunk;

	return (chunk + 1);
}

/*
** Initialize new memory areas called 'page'
** while (g_page[0]) is to mmap a new area
** Then mmap is used to allocate memory
** Finally we create a chunk corresponding to the size requested by the user
*/

static bool	page_init(t_page **page)
{

	if ((*page = mmap(NULL, MALLOC_TINY_MAX_SIZE, PROT_READ | PROT_WRITE,
		MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
			return (false);

//	ft_printf("%p  page init didnt fail\n\n", *page);
	return (true);
}

/*
** creates a chained-list of free memory areas
*/

static void		*malloc_handler(const size_t size, t_page **page)
{
	//if (page && (*page).current_size + size > MALLOC_TINY_MAX_SIZE)
	//	page = (*page).next;
	//getchar();
	if (!*page && !page_init(page))
		return (NULL);
	(*page)->current_size += size + sizeof(t_chunk);
	(*page)->chunk_nb += 1;
	return (chunk_create(size + sizeof(t_chunk), &(*page)->chunk));
}

/*
** allocates memory accordingly to the desired size
*/

static void 	*malloc_tiny(size_t size)
{
	ft_printf("-----\ntiny: %zu\n", size);
	void	*ptr;
	t_page	**page = &g_page[0];

	ptr = malloc_handler(size, page);
	//ft_printf("page current size: %zu\n", (*page)->current_size);

	ft_printf("-----\npage current size: %zu\n", (*page)->current_size);

	return (ptr);
}

static void 	*malloc_small(size_t size)
{
	void		*ptr = NULL;

	ft_printf("small:  %zu\n", size);
	t_page	*page = g_page[1];

	return (malloc_handler(size, &page));//tmp
	return (ptr);
}

/*
** call large malloc and keep a way to free them
*/

static void 	*malloc_large(size_t size)
{
	void		*ptr = NULL;
	ft_printf("large: %zu\n", size);
	t_page	*page = g_page[2];

	return (malloc_handler(size, &page));//tmp
	/*
	if (ptr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, -1, 0)) < 0)
		return (NULL);
	*/
	return (ptr);
}

/*
** malloc_type : using function pointer instead of 'if / else if'
** &lock is used for hyperthreading
*/

void			*ft_malloc(size_t size)
{
	void			*ptr;
	void 			*(*malloc_type[3])(size_t) = \
								{&malloc_tiny, &malloc_small, &malloc_large};
	const size_t	i = (size > MALLOC_TINY) + (size > MALLOC_SMALL);

	//ft_printf("malloc 0 1 2 : %zu    size: %zu\n", i, size);//
	if (size == 0)
		return (NULL);
	pthread_mutex_init(&g_lock, NULL);
	pthread_mutex_lock(&g_lock);
	ptr = malloc_tiny(size);
//	g_page[0].chunk_nb = 1;
//	ft_printf("\n\n%p ~ Malloc done\n\n", ptr);//
	pthread_mutex_unlock(&g_lock);
//	show_alloc_mem();
	return ptr;
}
