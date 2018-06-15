/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/29 22:40:41 by pribault          #+#    #+#             */
/*   Updated: 2018/06/15 23:39:57 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

char	*get_hostname(struct iphdr *iphdr)
{
	struct hostent	*host;
	static char		hostname[32];

	if ((host = gethostbyaddr(&iphdr->saddr, 4, IPV4)))
		ft_strcpy(hostname, host->h_name);
	else
		inet_ntop(IPV4, &iphdr->saddr, hostname, sizeof(hostname));
	return ((char *)&hostname);
}

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
	char			buffer[32];
	struct timeval	now;

	(void)size;
	if (icmphdr->un.echo.id != getpid() ||
		g_e.sequence != icmphdr->un.echo.sequence)
		return ;
	gettimeofday(&now, NULL);
	if (!(g_e.sequence % g_e.probes))
		printf("%2hu  %s (%s)", (uint16_t)(g_e.sequence / g_e.probes + 1),
			get_hostname(iphdr), inet_ntop(IPV4, &iphdr->saddr, buffer,
			sizeof(buffer)));
	printf("  %.3f ms", (float)(now.tv_sec - g_e.prev.tv_sec) * 1000 +
		(float)(now.tv_usec - g_e.prev.tv_usec) / 1000);
	if (!(++g_e.sequence % g_e.probes))
	{
		printf("\n");
		exit(0);
	}
	else
		send_ping_request(g_e.client);
}

void	treat_icmphdr(struct iphdr *iphdr,
		struct icmphdr *icmphdr, size_t size)
{
	static t_icmp_hdlr	hdlrs[] = {
		{ICMP_ECHOREPLY, &icmp_echo_reply},
		{ICMP_DEST_UNREACH, &icmp_dest_unreach},
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
	if (g_e.opt & OPT_VERBOSE)
		icmp_other(iphdr, icmphdr, size);
}
