/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribault <pribault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/08 16:24:21 by pribault          #+#    #+#             */
/*   Updated: 2018/06/15 23:50:23 by pribault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TRACEROUTE_H
# define FT_TRACEROUTE_H

/*
****************
**	includes  **
****************
*/

# include "libft.h"
# include "libsocket.h"
# include <linux/ip.h>
# include <linux/icmp.h>
# include <sys/time.h>
# include <arpa/inet.h>
# include <stdio.h>
# include <math.h>

/*
***************
**	defines  **
***************
*/

/*
**	options
*/

# define OPT_VERBOSE	BYTE(0)

/*
**	default values
*/

# define DEFAULT_PACKET_SIZE	60
# define DEFAULT_TIMEOUT		5000000
# define DEFAULT_START			1
# define DEFAULT_PROBES			3
# define DEFAULT_HOPS			30

/*
*************
**	enums  **
*************
*/

typedef enum		e_ping_errors
{
	ERROR_CANNOT_CONNECT = ERROR_FT_MAX,
	ERROR_CANNOT_CREATE_SOCKET,
	ERROR_PACKET_TOO_SMALL,
	ERROR_INVALID_CHECKSUM,
	ERROR_INVALID_IHL,
	ERROR_INVALID_DEST_UNREACH,
	ERROR_PROTOCOL_NOT_HANDLED,
	ERROR_ARGS_SET,
	ERROR_NO_ADDRESS,
	ERROR_CANNOT_FIND_ADDRESS,
	ERROR_ALLOCATION_2,
	ERROR_CANNOT_SET_OPTION,
	ERROR_MEMORY_CORRUPTED,
	ERROR_MINIMAL_INTERVAL,
	ERROR_INVALID_TTL
}					t_ping_errors;

typedef enum		e_ipv4_protocol
{
	IPV4_PROTOCOL_ICMP = 1
}					t_ipv4_protocol;

/*
******************
**	structures  **
******************
*/

typedef struct		s_env
{
	t_socket		*socket;
	char			*address;
	t_client		*client;
	struct timeval	prev;
	__be32			prev_addr;
	struct in_addr	source;
	size_t			packet_size;
	size_t			timeout;
	uint8_t			hops;
	uint8_t			probes;
	uint8_t			sequence;
	uint8_t			opt;
}					t_env;

typedef struct		s_icmp_hdlr
{
	uint8_t			type;
	void			(*function)(struct iphdr *, struct icmphdr *,
					size_t);
}					t_icmp_hdlr;

typedef struct		s_icmp_str
{
	uint8_t			type;
	char			*str;
}					t_icmp_str;

/*
******************
**	prototypes  **
******************
*/

/*
**	flag getters
*/

void				get_flags(int argc, char **argv);
void				default_getter(char *s, t_env *env);
void				get_verbose(t_env *env, char **args, int n);
void				get_timeout(t_env *env, char **args, int n);
void				get_max_hops(t_env *env, char **args, int n);
void				get_first_ttl(t_env *env, char **args, int n);
void				get_source_address(t_env *env, char **args, int n);
void				print_usage(void);

/*
**	libsocket callbacks
*/

void				client_add(t_socket *socket, t_client *client);
void				client_del(t_socket *socket, t_client *client);
void				client_excpt(t_socket *socket, t_client *client);
void				msg_recv(t_socket *socket, t_client *client, t_msg *msg);
void				msg_send(t_socket *socket, t_client *client, t_msg *msg);
void				msg_trash(t_socket *socket, t_client *client, t_msg *msg);

/*
**	endian functions
*/

void				endian(void *ptr, size_t size);
void				endian_iphdr(struct iphdr *iphdr);

/*
**	packet treatment
*/

__sum16				compute_sum(void *ptr, size_t size);
void				treat_iphdr(struct iphdr *iphdr,
					size_t size);
void				treat_icmphdr(struct iphdr *iphdr,
					struct icmphdr *icmphdr, size_t size);

/*
**	ping functions
*/

void				send_ping_request(t_client *client);

/*
**	icmp functions
*/

void				icmp_echo_reply(struct iphdr *iphdr,
					struct icmphdr *icmphdr, size_t size);
void				icmp_time_exceeded(struct iphdr *iphdr,
					struct icmphdr *icmphdr, size_t size);
void				icmp_dest_unreach(struct iphdr *iphdr,
					struct icmphdr *icmphdr, size_t size);
void				icmp_other(struct iphdr *iphdr,
					struct icmphdr *icmphdr, size_t size);

/*
**	others
*/

char				*get_hostname(struct iphdr *iphdr);
char				*get_client_addr(t_client *client);

extern t_env	g_e;

#endif
