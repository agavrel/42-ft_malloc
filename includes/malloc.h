/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/14 18:01:12 by angavrel          #+#    #+#             */
/*   Updated: 2018/11/19 19:17:29 by angavrel         ###   ########.fr       */
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
** basic libc functions
*/

# include "libft.h"

/*
** multithreading lib
*/

# include <pthread.h>

extern pthread_mutex_t	g_malloc_mutex;

/*
** malloc size thresholds to point to most suitable malloc type functions
*/

# define ZONE_SMALL		(1 << 10)
# define ZONE_TINY		(1 << 6)
# define MALLOC_ZONE	(1 << 7)
# define MASK_0XFFF		(1 << 12) - 1

typedef	enum
{
	MALLOC_TINY,
	MALLOC_SMALL,
	MALLOC_LARGE
}	t_malloc_type;

/*
** memory blocks struct
*/

typedef struct			s_block
{
	struct s_block		*next;
	struct s_block		*prev;
	size_t				size;
	size_t				for_align_purpose;
}						t_block;

/*
** page struct used for tiny and small malloc
*/

typedef struct			s_page
{
	struct s_page		*next;
	struct s_page		*prev;
	t_block				*alloc;
	t_block				*free;
}						t_page;

/*
** struct containing all ptr addresses
*/

typedef struct			s_malloc_pages
{
	t_page				*tiny;
	t_page				*small;
	t_block				*large;
}						t_malloc_pages;
extern t_malloc_pages	g_malloc_pages;

/*
** Main functions
*/

void					*malloc(size_t size);
void					free(void *ptr);
void					*realloc(void *ptr, size_t size);
void					*calloc(size_t count, size_t size);
void					*reallocf(void *ptr, size_t size);

/*
** Utility functions
*/

size_t					ft_align(size_t size, size_t mask);
bool					is_valid_block(const void *ptr, size_t size);
void					show_alloc_mem();
void					show_alloc_mem_hex(void *ptr);
void					show_alloc_mem_minimap();
bool					malloc_error(const int err, const char *str);
void					putaddr(void *addr);
void					ft_putsizebase(size_t nb, int size_base);
size_t					page_size(size_t type);

#endif
