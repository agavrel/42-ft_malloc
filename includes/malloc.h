/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/14 18:01:12 by angavrel          #+#    #+#             */
/*   Updated: 2018/11/19 18:11:28 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# include <fcntl.h>
# include <unistd.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <stdlib.h>
# include <stdbool.h>
# include <stddef.h>
# include <limits.h>


/*
** malloc and free lib
*/

# include <sys/mman.h>


/*
** multithreading lib
*/

# include <pthread.h>

/*
** basic libc functions
*/

# include "libft.h"

/*
** malloc size thresholds to point to most suitable method
*/

typedef enum
{
    MALLOC_TINY,
    MALLOC_SMALL,
    MALLOC_LARGE
}		e_malloc_type;

# define ZONE_SMALL		(1024)
# define ZONE_TINY		(64)
# define MALLOC_ZONE	(128)


/*
** MALLOC_PAGE(size) returns new_size with
**     (new_size > size)
**     &&
**     (new_size % MALLOC_PAGE_S == 0)
*/
# define PAGE_S			(4096)
# define MALLOC_PAGE(x)	((x) + (!!((x) % PAGE_S) * (PAGE_S - (x) % PAGE_S)))

/*
** memory blocks struct
*/

# define MALLOC_MINIMAP	64

typedef struct				s_block
{
	struct s_block			*next;
	struct s_block			*prev;
	size_t					size;
}__attribute__((aligned(32)))t_block;

typedef struct				s_page
{
	struct s_page			*next;
	struct s_page			*prev;
	t_block					*alloc;
	t_block					*free;
}__attribute__((aligned(32)))t_page;

typedef struct				s_malloc_pages
{
	t_page					*tiny;
	t_page					*small;
	t_block					*large;
}							t_malloc_pages;

/*
** malloc pages and multithreading
*/

extern t_malloc_pages		g_malloc_pages;
extern pthread_mutex_t		g_malloc_mutex;

/*
** Main functions
*/

void				*malloc(size_t size);
void				free(void *ptr);
void				*realloc(void *ptr, size_t size);
void				*calloc(size_t count, size_t size);
void				*reallocf(void *ptr, size_t size);

/*
** Utility functions
*/

bool 				is_valid_block(const void *ptr, t_block *block);
void				show_alloc_mem();
void				show_alloc_mem_hex(void *ptr);
void				show_alloc_mem_minimap();
void				*malloc_error(const int err, const char *str);
void				putaddr(void *addr);
void				ft_putsizebase(size_t nb, int size_base);
size_t				page_size(size_t type);
size_t				ft_align(size_t size, size_t mask);

#endif
