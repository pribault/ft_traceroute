/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/30 21:04:29 by pribault          #+#    #+#             */
/*   Updated: 2018/06/15 22:22:06 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void	fill_ip_header(t_client *client, struct iphdr *iphdr)
{
	iphdr->ihl = 5;
	iphdr->version = 4;
	iphdr->tos = 0;
	iphdr->tot_len = sizeof(struct iphdr) + sizeof(struct icmphdr) +
	g_e.packet_size;
	iphdr->id = 0;
	iphdr->frag_off = 0;
	iphdr->ttl = g_e.sequence / g_e.probes + 1;
	iphdr->protocol = IPV4_PROTOCOL_ICMP;
	ft_memcpy(&iphdr->saddr, &g_e.source, 4);
	ft_memcpy(&iphdr->daddr,
		&((struct sockaddr_in *)&client->addr.addr)->sin_addr, 4);
	endian_iphdr(iphdr);
	iphdr->check = compute_sum(iphdr, sizeof(struct iphdr));
}

void	fill_icmp_header(struct icmphdr *icmphdr)
{
	icmphdr->type = ICMP_ECHO;
	icmphdr->un.echo.id = getpid();
	icmphdr->un.echo.sequence = g_e.sequence;
	icmphdr->checksum = compute_sum(icmphdr, (sizeof(struct icmphdr) +
		g_e.packet_size));
	icmphdr->checksum = (icmphdr->checksum << 8) | (icmphdr->checksum >> 8);
}

void	fill_queue(void *queue, size_t size, struct timeval *now)
{
	size_t	i;
	uint8_t	c;

	c = 0x20;
	if (size >= sizeof(struct timeval))
	{
		i = sizeof(struct timeval);
		ft_memcpy(queue, now, sizeof(struct timeval));
	}
	else
		i = 0;
	while (i < size)
		*(uint8_t *)(queue + i++) = c++;
}

void	send_ping_request(t_client *client)
{
	struct timeval	timestamp;
	t_msg			msg;

	msg.size = sizeof(struct iphdr) + sizeof(struct icmphdr) +
	g_e.packet_size;
	if (!(msg.ptr = malloc(msg.size)))
		return (ft_error(2, ERROR_ALLOCATION_2, NULL));
	ft_bzero(msg.ptr, msg.size);
	fill_ip_header(client, msg.ptr);
	fill_queue(msg.ptr + sizeof(struct iphdr) + sizeof(struct icmphdr),
		g_e.packet_size, &timestamp);
	fill_icmp_header(msg.ptr + sizeof(struct iphdr));
	socket_enqueue_write(g_e.socket, client, &msg);
}
