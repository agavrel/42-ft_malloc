/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/09 22:16:06 by angavrel          #+#    #+#             */
/*   Updated: 2018/11/19 19:14:32 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

bool		malloc_error(const int err, const char *str)
{
	static const char	*msg[3] = {
		"  -> in _",
		"Invalid block: ",
		"mmap MAP_FAILED error: "
	};

	ft_dprintf(2, "%s%s\n", msg[err], str);
	return (false);
}
