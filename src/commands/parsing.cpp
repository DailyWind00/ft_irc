#include "main.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <string>

bool str_to_bool(std::string str)
{
	if (str == "true")
		return true;
	else if (str == "false")
		return false;
	throw std::invalid_argument("Invalid boolean");
	return false;
}
int get_command(const std::string& command) {
	if (command == "KICK") return KICK;
	if (command == "INVITE") return INVITE;
	if (command == "TOPIC") return TOPIC;
	if (command == "MODE") return MODE;
	if (command == "PRIVMSG") return MSG;
	if (command == "NICK") return NICK;
	if (command == "PART") return PART;
	if (command == "QUIT") return QUIT;
	if (command == "USER") return USER;
	if (command == "PING") return PING;
	if (command == "JOIN") return JOIN;
	if (command == "WHOIS") return WHOIS;
	if (command == "PASS") return PASS;
	return CMD_ERROR;
}
int get_mode(const std::string& command) {
	if (command.compare("-i") == 0)
		return minus_i;
	if (command.compare("+i") == 0)
		return plus_i;
	if (command.compare("+t") == 0)
		return plus_t;
	if (command.compare("-t") == 0)
		return minus_t;
	if (command.compare("+k") == 0)
		return plus_k;
	if (command.compare("-k") == 0)
		return minus_k;
	if (command.compare("-o") == 0)
		return minus_o;
	if (command.compare("+o") == 0)
		return plus_o;
	if (command.compare("-l") == 0)
		return minus_l;
	if (command.compare("+l") == 0)
		return plus_l;
	return mode_error;
}

bool check_if_username_exist(std::string str, Server & server)
{
	std::vector<Client> users_list = server.get_clients();
	// std::cout << "(to remove) Searching (bool): " << str << std::endl;   
	for (std::vector<Client>::iterator it = users_list.begin(); it != users_list.end(); ++it)
	{
		if (it->getUsername() == str)
			return true;
	}
	return false;
}

bool check_if_nickname_exist(std::string str, Server & server)
{
	std::vector<Client> users_list = server.get_clients();
	// std::cout << "(to remove) Searching (bool): " << str << std::endl;   
	for (std::vector<Client>::iterator it = users_list.begin(); it != users_list.end(); ++it)
	{
		if (it->getNickname() == str)
			return true;
	}
	return false;
}

Client Search_client_ID(std::string str, std::vector<Client> users_list)
{
	// std::cout << "(to remove) Searching : " << str << std::endl;   
	for (std::vector<Client>::iterator it = users_list.begin(); it != users_list.end(); ++it)
	{
		if (it->getUsername() == str)
		{
			// std::cout << "(to remove) Found : " << it->getUsername() << std::endl;
			return *it;
		}
	}
	throw std::invalid_argument("User does not exist");
}

Client Search_client_ID_Nick(std::string str, std::vector<Client> users_list)
{
	// std::cout << "(to remove) Searching : " << str << std::endl;   
	for (std::vector<Client>::iterator it = users_list.begin(); it != users_list.end(); ++it)
	{
		if (it->getNickname() == str)
		{
			// std::cout << "(to remove) Found : " << it->getNickname() << std::endl;
			return *it;
		}
	}
	throw std::invalid_argument("User does not exist");
}

int parsing_mode(std::vector<std::string> args, Channel *channel, Client &client)
{
	std::stringstream test;
	int num;
	std::string msg;

	if (channel != NULL && channel->check_if_user_is_admin(client) == false)
	{
		send_RPL_message(482, NULL, client, channel, "You are not an admin");
		return 0;
	}
	if (DEBUG)
	{
		for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); ++it)
		{
			std::cout << "args: " << *it << std::endl;
		}
	}
	int mode_temp;
	for (size_t i = 0; i < args.size(); i++)
	{
		mode_temp = get_mode(args[i]);
		if (mode_temp != mode_error)
			break;
	}
	switch(mode_temp)
	{
		case minus_i:
			if (args.size() != 3)
			{
				msg = "Wrong number of arguments\r\n";
				send(client.getFd(), msg.c_str(), msg.size(), 0);
				throw std::invalid_argument("wrong number of arguments");
			}
			channel->set_invite_only(false, client);
			msg = "The channel " + channel->get_channel_name() + " is no longer invite only\r\n";
            send(client.getFd(), msg.c_str(), msg.size(), 0);
			break;
		case plus_i:
			if (args.size() != 3)
			{
				msg = "Wrong number of arguments\r\n";
				send(client.getFd(), msg.c_str(), msg.size(), 0);
				throw std::invalid_argument("wrong number of arguments");
			}
			if (channel == NULL)
				break;
			channel->set_invite_only(true, client);
			msg = "The channel " + channel->get_channel_name() + " is now invite only\r\n";
			send(client.getFd(), msg.c_str(), msg.size(), 0);
			break;
		case minus_t:
			if (args.size() != 3)
			{
				msg = "Wrong number of arguments\r\n";
				send(client.getFd(), msg.c_str(), msg.size(), 0);
				throw std::invalid_argument("wrong number of arguments");
			}
			channel->change_topic_autorization(false, client);
			msg = "The channel " + channel->get_channel_name() + " is no longer topic protected\r\n";
            send(client.getFd(), msg.c_str(), msg.size(), 0);
			break;
		case plus_t:
			if (args.size() != 3)
			{
				msg = "Wrong number of arguments\r\n";
				send(client.getFd(), msg.c_str(), msg.size(), 0);
				throw std::invalid_argument("wrong number of arguments");
			}
			channel->change_topic_autorization(true, client);
			msg = "The channel " + channel->get_channel_name() + " is now topic protected\r\n";
            send(client.getFd(), msg.c_str(), msg.size(), 0);
			break;
		case plus_k:
			if (args.size() != 4)
			{
				msg = "Wrong number of arguments\r\n";
				send(client.getFd(), msg.c_str(), msg.size(), 0);
				send_RPL_message(461, NULL, client, channel, "Wrong number of arguments");
			}
			channel->set_password(args[3], client);
			msg = "The channel " + channel->get_channel_name() + " is now password protected\r\n";
            send(client.getFd(), msg.c_str(), msg.size(), 0);
			break;
		case minus_k:
			if (args.size() != 3)
			{
				msg = "Wrong number of arguments\r\n";
				send(client.getFd(), msg.c_str(), msg.size(), 0);
				throw std::invalid_argument("wrong number of arguments");
			}
			channel->unset_password(client);
			msg = "The channel " + channel->get_channel_name() + " is no longer password protected\r\n";
			send(client.getFd(), msg.c_str(), msg.size(), 0);
			break;
		case plus_o:
			if(args.size() != 4)
			{
				msg = "Wrong number of arguments\r\n";
				send(client.getFd(), msg.c_str(), msg.size(), 0);
				throw std::invalid_argument("wrong number of arguments");
			}
			channel->add_admin(Search_client_ID_Nick(args[3], channel->get_users_list()), client);
			msg = "The user " + args[3] + " is now an admin of the channel " + channel->get_channel_name() + "\r\n";
            send(client.getFd(), msg.c_str(), msg.size(), 0);
			break;
		case minus_o:
			if(args.size() != 4)
			{
				msg = "Wrong number of arguments\r\n";
				send(client.getFd(), msg.c_str(), msg.size(), 0);
				throw std::invalid_argument("wrong number of arguments");
			}
			channel->remove_admin(Search_client_ID_Nick(args[3], channel->get_users_list()), client);
			msg = "The user " + args[3] + " is no longer an admin of the channel " + channel->get_channel_name() + "\r\n";
            send(client.getFd(), msg.c_str(), msg.size(), 0);
			break;
		case minus_l:
			if(args.size() != 3)
			{
				msg = "Wrong number of arguments\r\n";
				send(client.getFd(), msg.c_str(), msg.size(), 0);
				throw std::invalid_argument("wrong number of arguments");
			}
			channel->change_user_limit(0, client);
			msg = "The user limit of the channel " + channel->get_channel_name() + " has been removed\r\n";
            send(client.getFd(), msg.c_str(), msg.size(), 0);
			break;
		case plus_l:
			if(args.size() != 4)
			{
				msg = "Wrong number of arguments\r\n";
				send(client.getFd(), msg.c_str(), msg.size(), 0);
				throw std::invalid_argument("Not enough arguments");
			}
			test << args[3];
			test >> num;
			if (test.fail())
			{
				msg = "The user limit of the channel " + channel->get_channel_name() + " must be a number\r\n";
            	send(client.getFd(), msg.c_str(), msg.size(), 0);
				throw std::invalid_argument("User limit must be a number");
			}
			channel->change_user_limit(num, client);
			msg = "The user limit of the channel " + channel->get_channel_name() + " has been set to " + args[3] + "\r\n";
            send(client.getFd(), msg.c_str(), msg.size(), 0);
			break;
		case mode_error:
			msg = "The mode " + args[2] + " does not exist\r\n";
            send(client.getFd(), msg.c_str(), msg.size(), 0);
			throw std::invalid_argument("Mode does not exist");
	}
	return 1;
}

bool is_channel(const std::vector<Channel*>& channels, const std::string& channel_name) {
	if (channel_name.empty() || channel_name[0] != '#') {
	   return false;
	}

	for (std::vector<Channel*>::const_iterator it = channels.begin(); it != channels.end(); ++it) {
		if ((*it)->get_channel_name() == channel_name) {
			return true;
		}
	}
	return false;
}

Channel *Search_channel(const std::vector<Channel*> &channels, const std::string& channel_name) {
	if (channel_name.empty() || channel_name[0] != '#') {
		return NULL;
	}
	if (channels.empty()) {
		throw std::invalid_argument("No channels available");
	}
	for (std::vector<Channel*>::const_iterator it = channels.begin(); it != channels.end(); ++it) {
		if ((*it)->get_channel_name() == channel_name) {
			return *it; 
		}
	}
	throw std::invalid_argument("Channel not found");
}


int switch_search_command(std::vector<std::string> args , const std::vector<Channel*> &channels,Client &client, Server &server)
{
    Channel *channel = Search_channel(channels, args[1]);
    std::vector<Client> users_list = server.get_clients();
    std::map<Client, bool> users_map;
    std::string tmp;

    switch (get_command(args[0])) {
        case PASS:
            if (args[1] != server.get_password())
            {
                send_RPL_message(464, &server, client, NULL, "Wrong password");
                throw std::invalid_argument("Wrong password 1");
            }
            client.setput_pwd(true);
            break;
        case KICK:
            if (args.size() <= 3)
                send_RPL_message(461, &server, client, channel, "Wrong number of arguments");
            users_map = channel->get_users_map();
			if (DEBUG)
			{
				for(std::map<Client, bool>::iterator it = users_map.begin(); it != users_map.end(); ++it)
				{
					std::cout << "user: " << it->first.getNickname() << " is admin :" << it->second <<std::endl;
				}
			}
            if (channel->check_if_user_is_admin(client) == false)
                send_RPL_message(482, &server, client, channel, "You are not an admin");
            if (args.size() < 4)
            {
                for (size_t i = 3; i < args.size(); i++)
                {
                    tmp += args[i];
                    if (i != args.size() - 1)
                        tmp += " ";
                }
                channel->send_msg_to_channel((":IRC_server KICK " + args[1] + " " + args[2] + " :" + tmp + "\r\n"), client, false);
            }
            else
                channel->send_msg_to_channel((":IRC_server KICK " + args[1] + " " + args[2] + " :" + tmp + "\r\n"), client, false);
            channel->remove_user(Search_client_ID_Nick(args[2], channel->get_users_list()), client, server);
            break;
        case INVITE:
            if (args.size() != 3)
                send_RPL_message(461, &server, client, channel, "Wrong number of arguments");
			if (DEBUG)
				std::cout << "args[1] = " << args[1] << std::endl;
			channel = Search_channel(channels, args[2]);
			channel->invite_user_by_admin(Search_client_ID_Nick(args[1], server.get_clients()).getID());
			send_RPL_message(345, &server, client, channel, args[1]);
			tmp =  "You have invited " + args[1] + " to the channel " + channel->get_channel_name() + "\r\n";
			send(Search_client_ID_Nick(args[1], server.get_clients()).getFd(), tmp.c_str(), tmp.size(), 0);
            break;
        case TOPIC:
            if (args.size() <= 2)
                send_RPL_message(461, &server, client, channel, "Wrong number of arguments");
            channel->change_topic(args[2], client);
            channel->send_msg_to_channel(":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getRealname() + " TOPIC " + channel->get_channel_name() + " " + args[2] + "\r\n", client, false);
            break;
        // vsoltys!vsoltys@vsoltys TOPIC #channel :test
        case MODE:
			if (DEBUG)
        		std::cout << "args size :" << args.size() << std::endl;
            if (args.size() < 2)
                send_RPL_message(461, &server, client, channel, "Wrong number of arguments");
            if (args.size() == 2)
            {
                tmp = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getRealname() + " MODE " + channel->get_channel_name() + "itkol"+ "\r\n";
                send(client.getFd(), tmp.c_str() , tmp.size(), 0);
                break;
            }
            return parsing_mode(args, channel, client);
            break;
        case MSG:
            if (args.size() < 3)
                send_RPL_message(461, &server, client, channel, "Wrong number of arguments");
            if (args[2][0] == ':')
                args[2].erase(args[2].begin());
            for (size_t i = 2; i < args.size(); i++)
            {
                tmp += args[i];
                if (i != args.size() - 1)
                    tmp += " ";
            }
            if (args[1][0] != '#')
                server.send_private_msg(tmp, client, Search_client_ID_Nick(args[1], server.get_clients()));
            else
            {
                if (channel->check_if_user_is_in_channel(client) == false)
                    send_RPL_message(442, &server, client, channel, "");
                else
                	channel->send_msg_to_channel(tmp, client, true);
            }
            break;
        case NICK:
            if (args.size() != 2)
                send_RPL_message(461, &server, client, channel, "Wrong number of arguments");
            for (size_t i = 0 ; i < users_list.size(); i++)
            {
                if (users_list[i].getNickname() == args[1] && client.getFd() != users_list[i].getFd())
                {
					args[1] += "_";
                    // send_RPL_message(433, &server, client, channel," " + args[1]);
                    // break;
                    i = 0;
                }
            }
            tmp = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getRealname() + " NICK " + args[1] + "\r\n";
            send(client.getFd(),tmp.c_str(), tmp.size(), 0);
            client.setNickname(args[1]);
            break;
        case PART:
	        std::cout << "PART" << channel->get_channel_name() << std::endl;
            if (args.size() != 2)
                send_RPL_message(461, &server, client, channel, "Wrong number of arguments");
			channel->send_msg_to_channel((":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getRealname() +  " PART " + channel->get_channel_name() + " :" + "leave channel\r\n"), client, false);
            channel->remove_user(client, client, server);
			if (channel->get_admin_users_list().size() == 0)
			{
				std::cout << BRed << "Channel : " << channel->get_channel_name() << " dont have OP anymore , add another one" << Color_Off << std::endl;
				if (channel->get_users_list().size() > 0)
					channel->force_admin();
			}
			if (channel->get_users_list().size() == 0)
			{
				std::cout << BRed << "Channel : " << channel->get_channel_name() << " is empty, deleted" << Color_Off << std::endl;
				server.delete_channel(channel);
			}
            break;
        case QUIT:
            if (args.size() > 3)
                send_RPL_message(461, &server, client, channel, "Wrong number of arguments");
            tmp = ":" + client.getNickname() + "!" + client.getNickname() + "@" + client.getNickname() +  " QUIT " + ":" + "leave server\r\n";
            send(client.getFd(), tmp.c_str(), tmp.size(), 0);
            server.close_client(client.getFd());

            server.remove_user_from_all_channel(client);
            break;
        case USER:
            if (args.size() <= 3)
                send_RPL_message(461, &server, client, channel, "Wrong number of arguments");
            for (size_t i = 0 ; i < users_list.size(); i++)
            {
                while (check_if_username_exist(args[2], server))
                {
                    args[2] += "_";
                }
            }
            client.setUsername(args[2]);
            for (size_t i = 0 ; i < users_list.size(); i++)
            {
                if (users_list[i].getRealname() == args[1])
                {
					if (DEBUG)
                   		std::cout << "getnick = " << users_list[i].getRealname() << "args[1] = " << args[1] << std::endl;
                    args[1] += "_";
                    break;
                }
            }
            client.setRealname(args[1]);
            break;
        case PING:
            if (args.size() != 2)
                send_RPL_message(461, &server, client, channel, "Wrong number of arguments");
            send_RPL_message(6, NULL, client, NULL, "");
			// channel->send_private_msg("PONG " + args[1] + "\r\n", client, client);
            break;
        case JOIN:
            if (args.size() < 2 || args.size() > 3)
                send_RPL_message(461, &server, client, channel, "Wrong number of arguments");
            if (args.size() == 2)
            {
                channel->join_request(client, "", channel->get_channel_name());
                tmp = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getRealname() + " JOIN " + channel->get_channel_name() + "\r\n";
                channel->send_msg_to_channel(tmp, client, false);
                send_RPL_message(332, NULL, client, channel, "topic");
		        send_RPL_message(353, NULL, client, channel, "users");
		        send_RPL_message(366, NULL, client, channel, "end of /NAMES list");
            }
            else
            {
                channel->join_request(client, args[2], channel->get_channel_name());
                tmp = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getRealname() + " JOIN " + channel->get_channel_name() + "\r\n";
                channel->send_msg_to_channel(tmp, client, false);
                channel->send_msg_to_channel(tmp, client, false);
                send_RPL_message(332, NULL, client, channel, "topic");
		        send_RPL_message(353, NULL, client, channel, "users");
		        send_RPL_message(366, NULL, client, channel, "end of /NAMES list");
            }
                
            break;
        case WHOIS:
            if (args.size() != 2)
                send_RPL_message(461, &server, client, channel, "Wrong number of arguments");
            Client target = Search_client_ID_Nick(args[1], server.get_clients());
            std::string msg = target.getUsername() + "@42.fr\r\n";
            send(target.getFd(), msg.c_str(), msg.size(), 0);
            break;
        //case CMD_ERROR:
            //client.getCurrentChannel().send_msg_to_channel(args[1], client);
    }
    return 1;
}


int parsing_command(const std::string& str, std::vector<Channel*> channels, Client &client, Server &server) {
	// Check if the command starts with '/'
	if (str.empty()) {
		throw std::invalid_argument("empty string"); // Not a valid command
	}
	if (str == "CAP LS")
	{   Client temp_client("server", 0, 0, false);
		Channel temp_channel("temp", client);
		std::ostringstream oss;
		oss << server.get_server_socket().fd;
		std::string str = oss.str();
		temp_channel.send_private_msg((":" + str + " CAP * LS :none\r\n"), temp_client ,client);
		return 0;
	}
	// Split the command into arguments
	std::istringstream stream(str);
	std::vector<std::string> args;
	std::string arg;


	while (stream >> arg) {
		args.push_back(arg);
	}

	// Check if the arguments are empty
	if (args.empty()) {
		send_RPL_message(461, &server, client , NULL, "wrong arguments");
	}
	// Check for channel name presence
	if (args.size() < 2 || (args[1].empty() && args[0] != "QUIT")) {
		send_RPL_message(461, &server, client , NULL, "wrong arguments");
	}

	if (args[0] == "JOIN" && !is_channel(channels, args[1]))
	{
		std::string tmp;
		Channel *new_channel = new Channel(args[1], client);
		server.add_channel(new_channel);
		channels.push_back(new_channel);
		tmp = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getRealname() + " JOIN " + new_channel->get_channel_name() + "\r\n";
		
		// new_channel->send_msg_to_channel(tmp, client, false);
		new_channel->send_msg_to_channel(tmp, client, false);
		send_RPL_message(332, NULL, client, new_channel, "topic");
		send_RPL_message(353, NULL, client, new_channel, "users");
		send_RPL_message(366, NULL, client, new_channel, "end of /NAMES list");
		if (DEBUG)
			std::cout << "new channel created" << std::endl;
		return 1;
	}

	// Fetch the channel
	// Output channel details
	switch_search_command(args, channels, client, server); // Pass channel as reference

	return 1; // Command parsed successfully
}

void pre_parsing(const std::string& str, std::vector<Channel*> channels, Client &client, Server &server)
{
	std::vector<std::string> commands = splitCommands(str);
	if (client.getput_pwd() == false)
	{
		bool pwd_state = true;
		if (client.getput_pwd() == false)
		{
			pwd_state = false;
			if (server.get_password() != "")
			{	
				
				for (size_t i = 0; i < commands.size(); i++)
				{
					if (commands[i].find("CAP") != std::string::npos)
					{
						pwd_state = true;
						break;
					}
					if (commands[i].find("PASS") != std::string::npos)
					{
						pwd_state = true;
						client.setput_pwd(true);
						break;
					}
				}
			}
		}
		if (pwd_state == false) 
		{
			send_RPL_message(464, NULL, client, NULL, "Wrong password");
			throw std::invalid_argument("Wrong password 3");
		}
	}
	
	
	for (std::vector<std::string>::iterator it = commands.begin(); it != commands.end(); ++it)
	{
		if (DEBUG)
			std::cout << "command: " << *it << std::endl;
		if (*it != "JOIN :")
			parsing_command(*it, channels, client, server);
	}
	std::vector<Client> users_list = server.get_clients();
	if (DEBUG)
	{
		for (std::vector<Client>::iterator it = users_list.begin(); it != users_list.end(); ++it)
		{
			std::cout << "client Nick : " << it->getNickname() << " client user : " << it->getUsername() << std::endl;
		}
	}
}
