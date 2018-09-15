/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/15 14:56:24 by angavrel          #+#    #+#             */
/*   Updated: 2018/09/15 17:27:44 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/malloc.h"
#include <stdio.h>

int main(int ac, char **av)
{
	char	 *ptr;

	puts("Test");
	if (!(ptr = ft_malloc(1000)))
		puts("fail\n");
	for (int i = 1; i < 999; i++)
	{
		ptr[i] = (char)ft_malloc(i);
	}
	puts("COOL");
    //malloc_display();
    //free(ptr);
    return (0);
}
