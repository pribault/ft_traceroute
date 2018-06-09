/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/29 22:40:41 by pribault          #+#    #+#             */
/*   Updated: 2018/06/08 22:36:03 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

char	*get_icmp_string(__u8 type)
{
	static t_icmp_str	strs[] = {
		{ICMP_ECHOREPLY, "Echo Reply"},
		{ICMP_DEST_UNREACH, "Desination Unreachable"},
		{ICMP_SOURCE_QUENCH, "Source Quench"},
		{ICMP_REDIRECT, "Redirect (change route)"},
		{ICMP_ECHO, "Echo"},
		{ICMP_TIME_EXCEEDED, "Time exceeded"},
		{ICMP_PARAMETERPROB, "Parameter Problem"},
		{ICMP_TIMESTAMP, "Timestamp Request"},
		{ICMP_TIMESTAMPREPLY, "Timestamp Reply"},
		{ICMP_INFO_REQUEST, "Information Request"},
		{ICMP_INFO_REPLY, "Information Reply"},
		{ICMP_ADDRESS, "Address Mask Request"},
		{ICMP_ADDRESSREPLY, "Address Mask Reply"}
	};
	__u8				idx;

	idx = (__u8)-1;
	while (++idx < sizeof(strs) / sizeof(t_icmp_str))
		if (type == strs[idx].type)
			return (strs[idx].str);
	return ("Unknown");
}

void	icmp_other(struct iphdr *iphdr,
		struct icmphdr *icmphdr, size_t size)
{
	char			buffer[32];
	struct timeval	now;

	if (size < sizeof(struct icmphdr) + sizeof(struct iphdr) + 8)
		return ((g_e.opt & OPT_VERBOSE) ? ft_error(2,
		ERROR_INVALID_DEST_UNREACH, NULL) : (void)0);
		if (((struct icmphdr *)((void*)&icmphdr[1] +
		sizeof(struct iphdr)))->un.echo.id != getpid())
		return ;
	gettimeofday(&now, NULL);
	printf("From %s icmp_seq=%hu %s\n",
		inet_ntop(IPV4, &iphdr->saddr, buffer, sizeof(buffer)),
		((struct icmphdr *)((void*)&icmphdr[1] +
			sizeof(struct iphdr)))->un.echo.sequence,
		get_icmp_string(icmphdr->type));
}

void	icmp_echo_reply(struct iphdr *iphdr,
		struct icmphdr *icmphdr, size_t size)
{
	char			hostname[32];
	char			buffer[32];
	struct timeval	now;

	(void)size;
	if (icmphdr->un.echo.id != getpid())
		return ;
	gettimeofday(&now, NULL);
	if (getnameinfo((void*)&g_e.client->addr.addr, g_e.client->addr.len,
		hostname, sizeof(hostname), NULL, 0, 0))
		hostname[0] = '\0';
	printf("%2hu %s (%s)\n", icmphdr->un.echo.sequence, (char *)&hostname,
		inet_ntop(IPV4, &iphdr->saddr, buffer, sizeof(buffer)));
	exit(0);
}

void	treat_icmphdr(struct iphdr *iphdr,
		struct icmphdr *icmphdr, size_t size)
{
	static t_icmp_hdlr	hdlrs[] = {
		{ICMP_ECHOREPLY, &icmp_echo_reply},
		{ICMP_TIME_EXCEEDED, &icmp_time_exceeded}
	};
	size_t				i;

	if (compute_sum(icmphdr, size))
		return ((g_e.opt & OPT_VERBOSE) ?
			ft_error(2, ERROR_INVALID_CHECKSUM, NULL) : (void)0);
		i = (size_t)-1;
	while (++i < sizeof(hdlrs) / sizeof(t_icmp_hdlr))
		if (icmphdr->type == hdlrs[i].type)
			return (hdlrs[i].function(iphdr, icmphdr, size));
	icmp_other(iphdr, icmphdr, size);
}
