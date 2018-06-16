/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/29 22:40:41 by pribault          #+#    #+#             */
/*   Updated: 2018/06/16 13:31:24 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void	icmp_time_exceeded(struct iphdr *iphdr,
		struct icmphdr *icmphdr, size_t size)
{
	char			buffer[32];
	struct timeval	now;

	if (size < sizeof(struct icmphdr) + sizeof(struct iphdr) + 8)
		return ((g_e.opt & OPT_VERBOSE) ? ft_error(2,
			ERROR_INVALID_DEST_UNREACH, NULL) : (void)0);
		if (((struct icmphdr *)((void*)&icmphdr[1] +
		sizeof(struct iphdr)))->un.echo.id != getpid() ||
		g_e.sequence != ((struct icmphdr *)((void*)&icmphdr[1] +
			sizeof(struct iphdr)))->un.echo.sequence)
		return ;
	gettimeofday(&now, NULL);
	if (!(g_e.sequence % g_e.probes))
		printf("%2hu  ", (uint16_t)(g_e.sequence / g_e.probes + 1));
	if (!(g_e.sequence % g_e.probes) || iphdr->saddr != g_e.prev_addr)
		printf("%s (%s) ", get_hostname(iphdr), inet_ntop(IPV4, &iphdr->saddr,
		buffer, sizeof(buffer)));
	printf(" %.3f ms ", (float)(now.tv_sec - g_e.prev.tv_sec) * 1000 +
		(float)(now.tv_usec - g_e.prev.tv_usec) / 1000);
	if (!(++g_e.sequence % g_e.probes))
		printf("\n");
	g_e.prev_addr = iphdr->saddr;
	send_ping_request(g_e.client);
}

char	*get_icmp_dest_unreach_string(__u8 code)
{
	static t_icmp_str	strs[] = {
		{ICMP_NET_UNREACH, "!N"},
		{ICMP_HOST_UNREACH, "!H"},
		{ICMP_PROT_UNREACH, "!P"},
		{ICMP_FRAG_NEEDED, "!F"},
		{ICMP_SR_FAILED, "!S"},
	};
	__u8				idx;

	idx = (__u8)-1;
	while (++idx < sizeof(strs) / sizeof(t_icmp_str))
		if (code == strs[idx].type)
			return (strs[idx].str);
	return ("!?");
}

void	icmp_dest_unreach(struct iphdr *iphdr,
		struct icmphdr *icmphdr, size_t size)
{
	char			buffer[32];
	struct timeval	now;

	if (size < sizeof(struct icmphdr) + sizeof(struct iphdr) + 8)
		return ((g_e.opt & OPT_VERBOSE) ? ft_error(2,
			ERROR_INVALID_DEST_UNREACH, NULL) : (void)0);
		if (((struct icmphdr *)((void*)&icmphdr[1] +
		sizeof(struct iphdr)))->un.echo.id != getpid() ||
		g_e.sequence != ((struct icmphdr *)((void*)&icmphdr[1] +
			sizeof(struct iphdr)))->un.echo.sequence)
		return ;
	gettimeofday(&now, NULL);
	if (!(g_e.sequence % g_e.probes))
		printf("%2hu  ", (uint16_t)(g_e.sequence / g_e.probes + 1));
	if (!(g_e.sequence % g_e.probes) || iphdr->saddr != g_e.prev_addr)
		printf("%s (%s) ", get_hostname(iphdr), inet_ntop(IPV4, &iphdr->saddr,
		buffer, sizeof(buffer)));
	printf(" %.3f ms %s ", (float)(now.tv_sec - g_e.prev.tv_sec) * 1000 +
		(float)(now.tv_usec - g_e.prev.tv_usec) / 1000,
		get_icmp_dest_unreach_string(icmphdr->code));
	if (!(++g_e.sequence % g_e.probes))
		exit(printf("\n"));
	g_e.prev_addr = iphdr->saddr;
	send_ping_request(g_e.client);
}
