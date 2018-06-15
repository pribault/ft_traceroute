/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/29 22:40:41 by pribault          #+#    #+#             */
/*   Updated: 2018/06/15 23:16:01 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

char	*get_icmp_time_exceeded_string(__u8 code)
{
	static t_icmp_str	strs[] = {
		{ICMP_EXC_TTL, "Time to live exceeded"},
		{ICMP_EXC_FRAGTIME, "Fragment Reassembly time exceeded"}
	};
	__u8				idx;

	idx = (__u8)-1;
	while (++idx < sizeof(strs) / sizeof(t_icmp_str))
		if (code == strs[idx].type)
			return (strs[idx].str);
	return ("Time Exceeded");
}

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
		printf("%2hu  %s (%s)", (uint16_t)(g_e.sequence / g_e.probes + 1),
			get_hostname(iphdr), inet_ntop(IPV4, &iphdr->saddr, buffer,
			sizeof(buffer)));
	printf("  %.3f ms", (float)(now.tv_sec - g_e.prev.tv_sec) * 1000 +
		(float)(now.tv_usec - g_e.prev.tv_usec) / 1000);
	if (!(++g_e.sequence % g_e.probes))
		printf("\n");
	send_ping_request(g_e.client);
}
