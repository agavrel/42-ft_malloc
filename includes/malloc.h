/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/14 18:01:12 by angavrel          #+#    #+#             */
/*   Updated: 2018/10/11 22:51:23 by angavrel         ###   ########.fr       */
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



# define M_TINY		256
# define M_SMALL	1024
# define M_LARGE	65536
# define TINY_NB	100
# define TINY_SMALL	50

/*
** memory blocks struct
*/

typedef struct		s_chunk
{
	struct s_chunk	*next;
	struct s_chunk	*prev;
	size_t			max_size;
	size_t			size;
	char			data[0];
}					t_chunk;

/*

** char[0] to take the address of the first chunk to save memory and easily
** manipulate pointer addresses. char[0] takes 0 bytes if null
*/

typedef struct		s_page
{
	struct s_page	*next;
	struct s_page	*prev;
	size_t			max_size;
	size_t			chunk_nb;
	char			first_chunk[0];
}					t_page;


/*
** explicit name... read man if needed
*/

void				*malloc(size_t size);
void				*realloc(void *ptr, size_t size);
void				free(void *ptr);
void				*calloc(size_t count, size_t size);
void				show_alloc_mem(void);
bool				errors(const int err, const char *str);
void				putaddr(void *addr);
void				ft_putsizebase(size_t nb, int size_base);
size_t				page_size(size_t type);

extern t_page			*g_page[3];
extern pthread_mutex_t	g_lock;

#endif
