/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_chartab.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/14 15:07:46 by angavrel          #+#    #+#             */
/*   Updated: 2017/05/01 02:08:59 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** function to free char ** tab
*/

void	ft_free_chartab(char **tab)
{
	while (*tab)
		free(*tab++);
	free(tab);
}
