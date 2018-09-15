/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strappend.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/01 02:21:20 by angavrel          #+#    #+#             */
/*   Updated: 2017/05/01 02:25:12 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

/*
** adds a character to an array
*/

char	*ft_strappend(char *origin, char add)
{
	char	*str;
	int		len;

	if (origin == NULL)
		return (NULL);
	len = ft_strlen(origin);
	str = ft_realloc(origin, len + 2);
	if (str == NULL)
		return (NULL);
	str[len] = add;
	str[len + 1] = '\0';
	return (str);
}
