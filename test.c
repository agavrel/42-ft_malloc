/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/15 14:56:24 by angavrel          #+#    #+#             */
/*   Updated: 2018/10/07 19:43:43 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/malloc.h"
#include <stdio.h>

int main(int ac, char **av)
{
	//void	 **ptr;
	void	*ptr[1000];

	//puts("Test");



	ptr[1] = malloc(600);
	ptr[2] = malloc(600);
	ptr[3] = malloc(600);
	ptr[4] = malloc(200);

	free(ptr[1]);
	free(ptr[2]);
	free(ptr[3]);
	//free(ptr[3]);
//	show_alloc_mem();
	puts("COOL");
    //malloc_display();
    //free(ptr);
    return (0);
}


/*
ma page
 -> premier chunk   avec sa taille
 -> deuxieme chunk a null

 comme le deuxieme chunk est null on regarde si la taille de la page ; A= adresse de la page + size de la page
 on regarde si A est bien superieur a
 B = addresse du dernier chunk existant + size of chunk + data + size of chunk + new data
 */
