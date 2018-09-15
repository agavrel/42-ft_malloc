/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/09 22:16:06 by angavrel          #+#    #+#             */
/*   Updated: 2018/06/09 22:19:51 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

bool				errors(const int err, const char *str)
{
	static const char	*msg[3] = {
		"  -> in _",
		"Bad file: ",
		"Error: "
	};

	ft_dprintf(2, "%s%s", msg[err], str);
	return (false);
}
