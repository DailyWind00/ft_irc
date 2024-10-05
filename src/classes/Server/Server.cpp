/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aallou-v <aallou-v@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 12:03:17 by mgallais          #+#    #+#             */
/*   Updated: 2024/09/26 16:30:45 by aallou-v         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server* Server::singleton = NULL;

/// Constructors & Destructor :
Server::Server( const int port, std::string password )
{
	this->port = port;
	this->server_adress = "localhost";
	this->password = password;
	this->server_status = STOPPED;
	this->poll_size = 1;
	this->poll_count = 0;
	this->all_sockets = std::vector<struct pollfd>(max_clients);

	std::cout << BWhite;
	std::cout << "[Server] Server initialized\n";
	std::cout << Color_Off;
}

Server::~Server()
{
	std::cout << BWhite;
	std::cout << "[Server] Server destroyed\n";
	std::cout << Color_Off;
}

Server *Server::get_instance(int port, std::string password)
{
	if (!singleton)
		singleton = new Server(port, password);
	return singleton;
}

void Server::destroy_instance()
{
    if (singleton)
    {
		if (singleton->get_status() == RUNNING)
			singleton->stop();
        delete singleton;
        singleton = NULL;
    }
}
/// ---



/// Private Functions :
void Server::remove_user_from_all_channel(Client client)
{
	for (std::size_t i = 0; i < channels.size(); ++i)
	{
		if (channels[i]->check_if_user_is_in_channel(client))
		{
			channels[i]->remove_user(client, client, *this);
			if (channels[i]->get_users_list().size() == 0)
			{
				std::cout << BRed << "Channel : " << channels[i]->get_channel_name() << " is empty, deleted" << Color_Off << std::endl;
				channels.erase(channels.begin() + i);
			}
			if (channels[i]->get_admin_users_list().size() == 0)
			{
				std::cout << BRed << "Channel : " << channels[i]->get_channel_name() << " dont have OP anymore , add another one" << Color_Off << std::endl;
				if (channels[i]->get_users_list().size() > 0)
					channels[i]->force_admin();
			}
			std::string debug = client.getNickname() + "!" + client.getUsername() + "@" + this->server_adress +  " PART " + channels[i]->get_channel_name();
			std::cout << debug << std::endl;
			
			channels[i]->send_msg_to_channel((":" + client.getNickname() + "!" + client.getNickname() + "@" + client.getNickname() +  " PART " + channels[i]->get_channel_name() + " :" + "leave channel\r\n"), client, false);
			std::cout << BRed << "user : " << client.getNickname() << " removed from channel : " << channels[i]->get_channel_name() << Color_Off << std::endl;

		}
	}
}

void Server::delete_channel(Channel *channel)
{
	for (std::size_t i = 0; i < channels.size(); ++i)
	{
		if (channels[i] == channel)
		{
			delete channels[i];
			channels.erase(channels.begin() + i);
			break;
		}
	}
}

void Server::remove_user(Client client)
{
	for (int i = 0; i < (int)clients.size(); i++)
	{
		if (clients[i].getID() == client.getID())
		{
			close_client(clients[i].getFd());
			clients.erase(clients.begin() + i);
			break;
		}
	}
}
std::string Server::get_password() const
{
	return password;
}
// Create the server socket, and bind it
void	Server::create_server_socket()
{
	int		status;
	

	// Init of the sockaddr struct for socket creation
	struct addrinfo hints, *res;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;       // IPv4
	hints.ai_socktype = SOCK_STREAM; // TCP socket
	hints.ai_flags = AI_PASSIVE;     // For wildcard IP address

	// convertion of port to string
	std::stringstream ss;
	ss << port;

	status = getaddrinfo(NULL, ss.str().c_str(), &hints, &res);
	if (status != 0) {
		throw std::runtime_error( gai_strerror(status) );
	}

	// Socket creation
	server_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (server_socket == -1) {
		freeaddrinfo(res);
		throw std::runtime_error( strerror(errno) );
	}
	
	// Set socket options to reuse address
    int optval = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
        freeaddrinfo(res);
        close(server_socket);
        throw std::runtime_error(strerror(errno));
    }

	std::cout << BWhite;
	std::cout << "[Server] Created server socket fd: " << server_socket << std::endl;
	std::cout << Color_Off;

	// Binding of the socket to the address and port
	status = bind(server_socket, res->ai_addr, res->ai_addrlen);
	if (status != 0) {
		freeaddrinfo(res);
		close(server_socket);
		throw std::runtime_error( strerror(errno) );
	}

	std::cout << BWhite;
	std::cout << "[Server] Bound socket to localhost port " << port << std::endl;
	std::cout << Color_Off;

	// Listen for incoming connections
	status = listen(server_socket, 5);
	if (status != 0) {
		freeaddrinfo(res);
		close(server_socket);
		throw std::runtime_error( strerror(errno) );
	}

	// Set the server socket to non-blocking
	if (fcntl(server_socket, F_SETFL, O_NONBLOCK) == -1) 
		throw std::runtime_error("fcntl: " + std::string(strerror(errno)));

	// Add the server socket to the pollfd array
	poll_count = 1;
	all_sockets[0].fd = server_socket;
	all_sockets[0].events = POLLIN;
	all_sockets[0].revents = 0;

	std::cout << BWhite;
	std::cout << "[Server] Ready " << std::endl;
	std::cout << Color_Off;

	// Clean up
	freeaddrinfo(res);
}

Client& Server::get_client_by_socket(int client_socket)
{
	for (int i = 0; i < (int)clients.size(); i++)
	{
		if (clients[i].getFd() == client_socket)
			return clients[i];
	}
	throw std::invalid_argument("Client does not exist");
}

int Server::new_ID()
{
	return clients.size() + 1;
}

void	Server::close_all_clients()
{
	for (int i = 1; i < poll_count; i++)
		close(all_sockets[i].fd);
	
	clients.clear();
	for (std::size_t i = 0; i < channels.size(); ++i) {
		delete channels[i];
		if (DEBUG)
			std::cout << "Channel [" << i << "] deleted" << std::endl;
	}
	channels.clear();
	poll_count = 0;
}


/// Public Functions :
void	Server::start()
{
	try
	{
		create_server_socket();
		server_status = RUNNING;
		time = std::time(NULL);
		server_loop();
	}
	catch(const std::exception& e)
	{
		std::cerr << BRed;
		std::cerr << "[Server] Critical error : " << e.what() << " | Shutdown\n";
		std::cerr << Color_Off;
		stop(); // didn't test, may double free
	}
}

void	Server::stop()
{
	server_status = STOPPED;
	close_all_clients();
	close(server_socket);

	std::cout << BWhite;
	std::cout << "[Server] Server stopped\n";
	std::cout << Color_Off;
}

bool Server::server_command()
{
	static bool		first_run = true;
	static bool		program_running = true;
	std::string		buffer;

	if (!program_running)
		return program_running;

	// Set stdin to non-blocking
	if (fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("fcntl: " + std::string(strerror(errno)));
	
	std::getline(std::cin, buffer);
	std::cin.clear();
	
	if (buffer == "/start" && server_status == STOPPED)
		start();

	if (buffer == "/stop" && server_status == RUNNING)
		stop();

	if (buffer.substr(0, 5) == "/port")
	{
		if (server_status == RUNNING)
		{
			std::cerr << BRed << "[Server] Cannot change port while server is running\n" << Color_Off;
			return program_running;
		}
		if (buffer.size() < 6)
		{
			std::cerr << BRed << "[Server] Need a port number\n" << Color_Off;
			return true;
		}
		std::stringstream ss;
		unsigned short new_port;

		ss << buffer.substr(6);
		ss >> new_port;
		if (ss.fail())
			std::cerr << BRed << "[Server] Port must be a number\n" << Color_Off;
		else
		{
			port = new_port;
			std::cout << BWhite << "[Server] Port changed to " << port << std::endl << Color_Off;
		}
	}

	if (buffer.substr(0, 6) == "/passw")
	{
		if (server_status == RUNNING)
		{
			std::cerr << BRed << "[Server] Cannot change password while server is running\n" << Color_Off;
			return program_running;
		}
		if (buffer.size() < 8)
		{
			std::cerr << BRed << "[Server] Need a new password\n" << Color_Off;
			return true;
		}
		int i = 7;
		while (buffer[i])
		{
			if (std::isspace(buffer[i]))
			{
				std::cerr << BRed << "[Server] There are space is the password\n" << Color_Off;
				return program_running;
			}
			else
				i++;
		}
		
		std::string new_pwd(buffer.substr(7));
		password = new_pwd;
		std::cout << BWhite << "[Server] Password changed to " << password << "" << std::endl << Color_Off;
	}

	if (buffer == "/clear")
		system("clear");

	if (buffer == "/exit")
	{
		program_running = false;
		destroy_instance();
	}

	if (buffer == "/help" || first_run)
	{
		if (first_run)
			first_run = false;
			
		std::cout << BCyan;
		std::cout << "[Server] /start : start the server if not running\n";
		std::cout << "[Server] /stop : stop the server if running\n";
		std::cout << "[Server] /port <port> : change the port of the server\n";
		std::cout << "[Server] /passw <password>: Change password\n";
		std::cout << "[Server] /clear : clear the terminal\n";
		std::cout << "[Server] /exit : exit the program\n";
		std::cout << "[Server] /help : display this help\n";
		std::cout << Color_Off;
	}
	
	return program_running;
}
/// ---



/// Getters & Setters :
int		Server::get_status() const
{
	return server_status;
}

pollfd Server::get_server_socket() const
{
	return all_sockets[0];
}

std::vector<Client>	Server::get_clients() const
{
	return clients;
}

std::vector<Channel *>	Server::get_channels() const
{
	return channels;
}

void Server::add_channel(Channel *channel)
{
	channels.push_back(channel);
}

void Server::add_client(Client client)
{
	clients.push_back(client);
}

void Server::send_private_msg(std::string msg , Client who_send, Client who_receive)
{
	// std::cout << "(to remove) :" << " user : " << who_send.getUsername() << " send to : " << who_receive.getUsername() << " : " << msg << std::endl;
	std::string final_msg = ":" + who_send.getNickname() +"!" +who_send.getUsername() + "@" + who_send.getUsername() +" PRIVMSG " + who_send.getNickname() + " :"+ msg + "\r\n";
	// std::cout << final_msg << std::endl;
	send(who_receive.getFd(), final_msg.c_str() , final_msg.size(), 0);
}

std::string Server::get_server_adress()
{
	return server_adress;
}

void Server::set_server_adress(std::string server_adress)
{
	this->server_adress = server_adress;
}
/// ---
