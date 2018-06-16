/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/04 14:44:09 by pribault          #+#    #+#             */
/*   Updated: 2018/06/08 20:04:07 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void	get_timeout(t_env *env, char **args, int n)
{
	(void)n;
	env->timeout = ft_atof(args[0]) * 1000000;
}

void	print_usage(void)
{
	printf("Usage:\n  ft_traceroute [ -hv ] [ --help ] ");
	printf("[--wait waittime] host [ packetlen ]\nOptions:\n");
	printf("  %-27s Set the number of seconds to wait", "--wait waittime");
	printf(" for response to\n%30sa probe (default is 5.0).", "");
	printf(" Non-integer (float\n%30spoint) values allowed to\n", "");
	printf("Arguments:\n+     host          The host to traceroute to\n");
	printf("      packetlen     The full packet length (default is the");
	printf(" length of an IP\n%20sheader plus 40). Can be ignored or", "");
	printf(" increased to a minimal\n%20sallowed value\n", "");
	exit(1);
}
