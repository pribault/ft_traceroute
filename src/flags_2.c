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

void	get_source_address(t_env *env, char **args, int n)
{
	(void)env;
	(void)n;
	inet_pton(AF_INET, args[0], &g_e.source);
}

void	print_usage(void)
{
	printf("Usage:\n  ft_traceroute [ -hv ] [ --first first_ttl ] [ --help ]");
	printf(" [ --max-hops max_ttl ] [ --source src_addr ] [--wait waittime]");
	printf(" host [ packetlen ]\n");
	printf("Options:\n  %-27s Start from the first_ttl hop (instead from 1)\n",
		"--first first_ttl");
	printf("  %-27s Set the max number of hops (max TTL to be\n%30sreached).",
		"--max-hops max_ttl", "");
	printf(" Default is 30\n");
	printf("  %-27s Use source src_addr for outgoing packets\n",
		"--source src_addr");
	printf("  %-27s Set the number of seconds to wait", "--wait waittime");
	printf(" for response to\n%30sa probe (default is 5.0).", "");
	printf(" Non-integer (float\n%30spoint) values allowed to\n", "");
	printf("Arguments:\n+     host          The host to traceroute to\n");
	printf("      packetlen     The full packet length (default is the");
	printf(" length of an IP\n%20sheader plus 40). Can be ignored or", "");
	printf(" increased to a minimal\n%20sallowed value\n", "");
	exit(1);
}
