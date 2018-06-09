/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/29 22:40:41 by pribault          #+#    #+#             */
/*   Updated: 2018/06/08 22:36:13 by pribault         ###   ########.fr       */
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
	struct hostent	*host;
	char			hostname[32];
	char			buffer[32];
	struct timeval	now;

	if (size < sizeof(struct icmphdr) + sizeof(struct iphdr) + 8)
		return ((g_e.opt & OPT_VERBOSE) ? ft_error(2,
		ERROR_INVALID_DEST_UNREACH, NULL) : (void)0);
		if (((struct icmphdr *)((void*)&icmphdr[1] +
		sizeof(struct iphdr)))->un.echo.id != getpid())
		return ;
	gettimeofday(&now, NULL);
	if ((host = gethostbyaddr(&iphdr->saddr, 4, IPV4)))
		printf("%2hu %s (%s)\n", ((struct icmphdr *)((void*)&icmphdr[1] +
		sizeof(struct iphdr)))->un.echo.sequence, host->h_name,
		inet_ntop(IPV4, &iphdr->saddr, buffer, sizeof(buffer)));
	else
		printf("%2hu %s (%s)\n", ((struct icmphdr *)((void*)&icmphdr[1] +
		sizeof(struct iphdr)))->un.echo.sequence, inet_ntop(IPV4,
		&iphdr->saddr, hostname, sizeof(hostname)), inet_ntop(IPV4,
		&iphdr->saddr, buffer, sizeof(buffer)));
	g_e.ttl++;
	send_ping_request(g_e.client);
}
