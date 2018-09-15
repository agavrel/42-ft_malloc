/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/15 14:56:24 by angavrel          #+#    #+#             */
/*   Updated: 2018/09/15 22:30:34 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/malloc.h"
#include <stdio.h>

int main(int ac, char **av)
{
	//void	 **ptr;
	void	*ptr[1000];

	puts("Test");
	//if (!(ptr = ft_malloc(1000)))
	//	puts("fail\n");
	for (int i = 1; i <4; i++)
	{
		ptr[i] = ft_malloc(i);
	}
	for (int i = 1; i <4; i++)
	{
		free(ptr[i]);
	}
	puts("COOL");
    //malloc_display();
    //free(ptr);
    return (0);
}
