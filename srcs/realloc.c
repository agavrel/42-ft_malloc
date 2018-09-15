/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/17 22:13:21 by angavrel          #+#    #+#             */
/*   Updated: 2018/02/20 18:55:31 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	*realloc(void *ptr, size_t size)
{
	(void)size;

    return (ptr);
}

void	*calloc(size_t count, size_t size)
{
	void		*ptr = NULL;
	
	(void)size;
	(void)count;

	return (ptr);
}
