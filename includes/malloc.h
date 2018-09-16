/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/14 18:01:12 by angavrel          #+#    #+#             */
/*   Updated: 2018/09/16 17:08:23 by angavrel         ###   ########.fr       */
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
**
*/

# include "libft.h"

/*
** malloc size thresholds to point to most suitable method
*/



# define MALLOC_TINY	128
# define MALLOC_SMALL	1024

# define TINY_NB	100
# define TINY_SMALL	50

#define MALLOC_TINY_MAX_SIZE	(1000 * 4096)

/*
** memory area struct
*/

typedef struct		s_chunk
{
	struct s_chunk	*next;
	struct s_chunk	*prev;
	size_t			size;
}					t_chunk;

typedef struct		s_page
{
	struct s_page	*next;
	struct s_page	*prev;
	size_t			current_size;
	size_t			chunk_nb;
	struct s_chunk	chunk;
}					t_page;


/*
** explicit name... read man if needed
*/

void				*ft_malloc(size_t size);
void				*realloc(void *ptr, size_t size);
void				free(void *ptr);
void				*calloc(size_t count, size_t size);
void				show_alloc_mem(void);
bool				errors(const int err, const char *str);

extern t_page			*g_page[3];
extern pthread_mutex_t	g_lock;

#endif
