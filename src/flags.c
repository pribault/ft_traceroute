/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/04 14:44:09 by pribault          #+#    #+#             */
/*   Updated: 2018/06/08 20:27:53 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void	get_flags(int argc, char **argv)
{
	static t_short_flag	short_flags[] = {
		{'h', (void *)&print_usage},
		{'v', (void *)&get_verbose},
		{'\0', NULL}
	};
	static t_long_flag	long_flags[] = {
		{"help", 0, {0}, (void *)&print_usage},
		{"verbose", 0, {0}, (void *)&get_verbose},
		{"packetsize", 1, {PARAM_UNSIGNED}, (void *)&get_packet_size},
		{"timeout", 1, {PARAM_FLOAT}, (void *)&get_timeout},
		{NULL, 0, {0}, NULL}
	};

	ft_get_flags(argc, argv, ft_get_flag_array((t_short_flag *)&short_flags,
		(t_long_flag *)&long_flags, (void *)&default_getter), &g_e);
}

void	default_getter(char *s, t_env *env)
{
	if (env->address)
		return (ft_error(2, ERROR_ADDRESS_SET, env->address));
	env->address = s;
}

void	get_verbose(t_env *env, char **args, int n)
{
	(void)args;
	(void)n;
	env->opt ^= OPT_VERBOSE;
}
