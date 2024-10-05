/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aallou-v <aallou-v@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 09:55:33 by mgallais          #+#    #+#             */
/*   Updated: 2024/09/19 14:06:03 by aallou-v         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/// This is the main header file, which will be included in all the source files.
/// It is used to include all the necessary libraries and classes and to define some useful macros.
#pragma once

/*###########################################################################*/
/*                              Defines & Enums                              */
/*###########################################################################*/
enum command
{
	KICK,
	INVITE,
	TOPIC,
	MODE,
	MSG,
	NICK,
	PART,
	QUIT,
	USER,
	PING,
	JOIN,
	PASS,
	WHOIS,
	CMD_ERROR
};
enum mode{
	minus_i,
	plus_i,
	minus_t,
	plus_t,
	minus_k,
	plus_k,
	minus_o,
	plus_o,
	minus_l,
	plus_l,
	mode_error
};

# ifndef DEBUG
#  define DEBUG 0
# endif

/*###########################################################################*/
/*                                  Includes                                 */
/*###########################################################################*/
// IRC needed librairies
# if defined(__APPLE__)
#  include <sys/event.h>   // For kqueue, kevent (only on BSD systems like MacOS)
# endif

# include <unistd.h>       // For execve, dup, dup2, pipe, chdir, close, read, write, access, opendir, readdir, closedir
# include <string.h>       // For strerror
# include <errno.h>        // For errno
# include <sys/types.h>    // For socketpair, waitpid, kill, stat, open
# include <sys/socket.h>   // For socket, accept, listen, send, recv, bind, connect, getaddrinfo, freeaddrinfo, setsockopt, getsockname, getprotobyname
# include <netinet/in.h>   // For htons, htonl, ntohs, ntohl
# include <sys/select.h>   // For select
# include <poll.h>         // For poll
# include <sys/epoll.h>    // For epoll_create, epoll_ctl, epoll_wait
# include <netdb.h>        // For gai_strerror, getaddrinfo, freeaddrinfo, getprotobyname
# include <fcntl.h>        // For fcntl
# include <sys/wait.h>     // For waitpid
# include <signal.h>       // For signal
# include <sys/stat.h>     // For stat, open
# include <dirent.h>    
# include <vector>  	   // For opendir, readdir, closedir
# include <ctime>


// Custom libraries
# include "color.h"       // For color definitions

# define DAILYLIB_C_LIB 0 // C libraries not needed
# include "dailylib.hpp"  // For custom defines and structures

/*###########################################################################*/
/*                            Structures & Classes                           */
/*###########################################################################*/
// Classes
class Server;
class Client;
class Channel;
# include "Channel.hpp"
# include "Client.hpp"
# include "Server.hpp"

// Structures

/*###########################################################################*/
/*                            Functions Prototypes                           */
/*###########################################################################*/
std::vector<std::string>	splitCommands(const std::string& str);
void						pre_parsing(const std::string& str, std::vector<Channel*> channels, Client &client, Server &server);
int							parsing_command(const std::string& str, std::vector<Channel*> channels, Client &client, Server &server);
void						send_RPL_message(int RPL_number ,Server *server, Client client, Channel *channel, std::string msg_utils);
