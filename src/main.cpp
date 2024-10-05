/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallais <mgallais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 09:54:06 by mgallais          #+#    #+#             */
/*   Updated: 2024/08/13 14:19:04 by mgallais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../include/main.hpp"
#include "Client.hpp"
#include "Channel.hpp"

int	main( int argc, char **argv )
{
	if ( argc != 3 ) {
		std::cerr << BRed;
		std::cerr << "Usage: " << argv[0] << " <port> <password>\n";
		std::cerr << Color_Off;
		return 1;
	}
	
	std::stringstream ss;
	int port;

	ss << argv[1];
	ss >> port;
	if ( ss.fail() ) {
		std::cerr << BRed;
		std::cerr << "Port must be a number\n";
		std::cerr << Color_Off;
		return 1;
	}
	
	Server *server = Server::get_instance(port, argv[2]);
	server->signal_handler();
	while (server->server_command()) {}; // loop until '/exit' command

	return 0;
}
