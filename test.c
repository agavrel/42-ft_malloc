/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/15 14:56:24 by angavrel          #+#    #+#             */
/*   Updated: 2018/10/03 20:59:50 by angavrel         ###   ########.fr       */
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
	/*for (int i = 1; i <3; i++)
	{
		ptr[i] = ft_malloc(i);
	}
	for (int i = 1; i <3; i++)
	{
		free(ptr[i]);
	}*/
	ptr[1] = ft_malloc(4000);
	ptr[2] = ft_malloc(400);
/*	ptr[3] = ft_malloc(3);
	show_alloc_mem();
	ft_printf("\n\n\n\n");
	free(ptr[2]);
	show_alloc_mem();
		ft_printf("\n\n\n\n");
	free(ptr[1]);*/
	show_alloc_mem();
		ft_printf("\n\n\n\n");
	//free(ptr[3]);
	//show_alloc_mem();
	puts("COOL");
    //malloc_display();
    //free(ptr);
    return (0);
}