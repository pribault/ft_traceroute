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
		{"wait", 1, {PARAM_FLOAT}, (void *)&get_timeout},
		{"max-hops", 1, {PARAM_UNSIGNED}, (void *)get_max_hops},
		{"first", 1, {PARAM_UNSIGNED}, (void *)&get_first_ttl},
		{NULL, 0, {0}, NULL}
	};

	ft_get_flags(argc, argv, ft_get_flag_array((t_short_flag *)&short_flags,
		(t_long_flag *)&long_flags, (void *)&default_getter), &g_e);
}

void	default_getter(char *s, t_env *env)
{
	static int	state = 0;

	if (!state)
	{
		env->address = s;
		state++;
	}
	else if (state == 1)
	{
		if (!ft_isunsigned(s))
			return (ft_error(2, ERROR_UNSIGNED, s));
		g_e.packet_size = ft_atou(s);
		if (g_e.packet_size < sizeof(struct iphdr) + sizeof(struct icmphdr))
			g_e.packet_size = sizeof(struct iphdr) + sizeof(struct icmphdr);
		state++;
	}
	else
		return (ft_error(2, ERROR_ARGS_SET, env->address));
}

void	get_max_hops(t_env *env, char **args, int n)
{
	(void)env;
	(void)n;
	g_e.hops = ft_atou(args[0]);
}

void	get_first_ttl(t_env *env, char **args, int n)
{
	(void)env;
	(void)n;
	if (!ft_atou(args[0]))
		return (ft_error(2, ERROR_INVALID_TTL, NULL));
	g_e.sequence = ft_atou(args[0]) * g_e.probes - g_e.probes;
}

void	get_verbose(t_env *env, char **args, int n)
{
	(void)args;
	(void)n;
	env->opt ^= OPT_VERBOSE;
}
