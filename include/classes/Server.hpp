/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aallou-v <aallou-v@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 09:27:02 by mgallais          #+#    #+#             */
/*   Updated: 2024/09/26 16:16:40 by aallou-v         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include "main.hpp"

enum server_status_e
{
	STOPPED = 0,
	RUNNING
};

# define POLL_TIMEOUT 10
# define RECV_BUFFER_SIZE 32767

#include <sstream>

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()
class Server {
	private :
		/// Attributes :
		static const unsigned short		max_clients = 500;
		std::string 					password;
		int								server_status;

		// Server socket :
		unsigned short	port;
		int				server_socket;
		std::string 	server_adress;

		// To monitor client sockets :
		std::vector<struct pollfd>	all_sockets;   // Array of client sockets (0 is the server socket)
		int 						poll_size;     // Size of the descriptor array
		int 						poll_count;    // Current number of descriptors in the array
		std::vector<Client>			clients; 	   // Array of clients
		std::vector<Channel *>		channels;	   // Array of channels

		/// Private Functions :
		// Start
		void	create_server_socket(); // done
		void	server_loop();
		void	accept_new_client();
		void	receive_data( int client_socket );
		Client&	get_client_by_socket( int client_socket );

		// Stop
		void	close_all_clients();
	
		// Singleton
		Server( int port , std::string password );
		static Server *singleton;

	public :
		/// Constructors & Destructor :
		std::time_t time;
		bool new_client;
		static	Server *get_instance( int port = 0, std::string password = "" ); // Singleton
		static	void	destroy_instance();
		~Server();

		/// Public Functions :
		void	start();
		void	close_client( int client_socket );
		void	stop();
		bool	server_command();
		int		new_ID();
		void	signal_handler();

		/// Getters & Setters :
		void remove_user(Client client);
		std::string get_password() const;
		int		get_status() const;
		std::vector<Client> get_clients() const;
		std::vector<Channel *> get_channels() const;
		void add_channel(Channel *channel);
		void add_client(Client client);
		pollfd get_server_socket(void) const;
		void send_private_msg(std::string msg , Client who_send, Client who_receive);
		std::string get_server_adress();
		void remove_user_from_all_channel(Client client);
		void set_server_adress(std::string server_adress);
		void delete_channel(Channel *channel);

};
