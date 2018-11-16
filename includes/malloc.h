/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/14 18:01:12 by angavrel          #+#    #+#             */
/*   Updated: 2018/11/16 21:49:06 by angavrel         ###   ########.fr       */
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
# define MALLOC_TINY	0
# define MALLOC_SMALL	1
# define MALLOC_LARGE	2
# define MALLOC_SIZE(x)	(((x) > ZONE_TINY) + ((x) > ZONE_SMALL))
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
	struct s_block		*next;
	struct s_block		*prev;
	size_t					size;
}							t_block;

typedef struct				s_page
{
	struct s_page			*next;
	struct s_page			*prev;
	t_block				*alloc;
	t_block				*free;
}							t_page;

typedef struct				s_malloc_pages
{
	t_page					*tiny;
	t_page					*small;
	t_block				*large;
}							t_malloc_pages;

extern t_malloc_pages		g_malloc_pages;
extern pthread_mutex_t		g_malloc_mutex;

int							malloc_out_of_zones(const void *ptr);

/*
** ***************************** malloc public *********************************
*/

void						free(void *ptr);
void						*malloc(size_t size);
void						*calloc(size_t count, size_t size);
void						*realloc(void *ptr, size_t size);
void						*reallocf(void *ptr, size_t size);

void						show_alloc_mem();
void						show_alloc_mem_hex(void *ptr);
void						show_alloc_mem_minimap();

bool				errors(const int err, const char *str);
void				putaddr(void *addr);
void				ft_putsizebase(size_t nb, int size_base);
size_t				page_size(size_t type);
size_t				ft_align(size_t size, size_t mask);

#endif
