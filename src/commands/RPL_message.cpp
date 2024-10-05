#include "main.hpp"

std::string int_to_string(int number)
{
	std::string result;
	
	std::stringstream ss;
	ss << number;
	ss >> result;
	if (number < 6)
	{
		result = "00" + result;
	}
	return result;
}
void send_RPL_message(int RPL_number ,Server *server, Client client, Channel *channel ,std::string msg_utils = "")
{	
	std::string message = "wrong RPL number";
	std::string error_message = "Unknow error";
	if (DEBUG)
		std::cout << "RPL_number: " << RPL_number << std::endl;
	bool throww = true;
	std::string server_name = "IRC_server";
	std::string base_msg = ":" + server_name + " " + int_to_string(RPL_number) + " " + client.getNickname();
	#define PONG 6
	switch(RPL_number)
	{
		case 1:
		{
			message = base_msg + " :Welcome to the " + server_name + " Network, " + client.getNickname() + "!" + client.getUsername() + "@" + client.getUsername() + "\r\n";
			throww = false;
			break;
		}
		case 2:
		{
			message = base_msg + " :Your host is " + server_name + ", running version 8.0\r\n";
			throww = false;
			break;
		}
		case 3:
		{
			std::tm* now = std::localtime(&server->time);
			message = base_msg + " :This server was created " + int_to_string((now->tm_year + 1900)) + "/" + int_to_string((now->tm_mon + 1)) + "/" + int_to_string(now->tm_mday) + " " + int_to_string(now->tm_hour) + ":" + int_to_string(now->tm_min) + ":" + int_to_string(now->tm_sec) + "\r\n";
			throww = false;
			break;
		}
		case 4:
		{
			message = base_msg + " :There are " + int_to_string(server->get_clients().size()) + " users connected\r\n";
			throww = false;
			break;
		}
		case 5:
		{
			message = base_msg + " :The channels must begin with a '#'\r\nNicklen = 30\r\n";
			throww = false;
			break;
		}
		case PONG:
		{
			message = "PONG " + server_name + "\r\n";
			throww = false;
			break;
		}
		case 461:
			message = base_msg + " " + msg_utils + " :Not enough parameters\r\n";
			error_message = "Not enough parameters";
			break;
		case 462:
		{
			message = base_msg + " :You may not reregister\r\n";
			throww = false;
			break;
		}
		case 464:
			message = base_msg + " :Password incorrect\r\n";
			throww = false;
			break;
		case 431:
			message = base_msg + " :No nickname given\r\n";
			error_message = "No nickname given";
			break;
		case 432:
			message = base_msg + msg_utils + " :Erroneous nickname\r\n";
			error_message = "Erroneous nickname";
			break;
		case 433:
			message = base_msg + msg_utils + " :Nickname is already in use\r\n";
			throww = false;
			break;
		case 409:
			message = base_msg + " :No origin specified\r\n";
			error_message = "No origin specified";
			break;
		case 402:
			message = base_msg + server_name + " :No such server\r\n";
			error_message = "No such server";
			break;
		case 332:
			throww = false;
			if (channel->get_topic().empty())
				message = base_msg + " " + channel->get_channel_name() + " :No topic is set\r\n";
			else
				message = base_msg + " " + channel->get_channel_name() + " :" + channel->get_topic() + "\r\n";
			break;
		case 345:
			message = ":" + server_name + " " + int_to_string(RPL_number) + " " + channel->get_channel_name() + " :" + msg_utils + " has been invited by " + client.getUsername() + "\r\n";
			error_message = "Client has been invited by client";
			throww = false;
			break;
		case 353:
		{
			throww = false;
			message = base_msg + " = " + channel->get_channel_name() + " :";
			std::map<Client, bool> users = channel->get_users_map();
			for (std::map<Client, bool>::iterator it = users.begin(); it != users.end(); ++it)
			{
				if (it->second == true)
					message += "@";
				message += it->first.getNickname() + " ";
			}
			message += "\r\n";
			break;
		}
		case 366:
			throww = false;
			message = base_msg + " " + channel->get_channel_name() + " :End of /NAMES list\r\n";
			break;
		case 475:
			message = base_msg + " " + msg_utils + " :Cannot join channel (+k)\r\n";
			error_message = "Cannot join channel (+k)";
			break;
		case 403:
			message = base_msg + msg_utils + " :No such channel\r\n";
			error_message = "No such channel";
			break;
		case 442:
			message = base_msg + msg_utils + " :You're not on that channel\r\n";
			error_message = "Client is not on that channel";
			break;
		case 331:
			message = base_msg + msg_utils + " :No topic is set\r\n";
			error_message = "No topic is set";
			break;
		case 482:
			message = base_msg + " " + channel->get_channel_name() + " :You're not channel operator\r\n";
			error_message = "Client is not channel operator";
			break;
		case 401:
			message = base_msg + msg_utils + " :No such nick/channel\r\n";
			error_message = "No such nick/channel";
			break;
		case 341:
		{
			message = base_msg + " " + msg_utils + " " +channel->get_channel_name() + "\r\n";
			throww = false;
			break;
		}
		case 473:
			message = base_msg + " " + msg_utils + " :Cannot join channel (+i)\r\n";
			error_message = "Cannot join channel (+i)";
			break;
		case 471:
			message = base_msg + " " + msg_utils + " :Cannot join channel (+l)\r\n";
			error_message = "Cannot join channel (+l)";
			break;
		case 696:
			message = base_msg + msg_utils + "\r\n";
			error_message = "Channel name is too long";
			break;
		case 525:
			message = base_msg + channel->get_channel_name() + " :Key is not well-formed\r\n";
			error_message = "Key is not well-formed";
			break;
		case 502:
			message = base_msg + " :Cant change mode for other users\r\n";
			error_message = "Cant change mode for other users";
			break;
		case 472:
			message = base_msg + msg_utils + " :is unknown mode char to me\r\n";
			error_message = "is unknown mode char to me";
			break;
	}
	send(client.getFd(), message.c_str(), message.size(), MSG_NOSIGNAL);
	std::cout << BYellow << message << Color_Off;
	if (throww == true)
		throw std::invalid_argument(error_message);

}
