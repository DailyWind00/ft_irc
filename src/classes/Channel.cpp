#include "Channel.hpp"
#include "Client.hpp"
#include "color.h"

Channel::Channel(std::string channel_name, Client &creator)
	: channel_name(channel_name), creator(creator), invited_users()
{
	Channel::add_admin(creator, creator);
	invite_only = false;
	topic = "No topic is set";
	topic_autorization = true;
	user_limit = 20;
	users_list.insert(std::pair<Client, bool>(creator, true));
	invited_users.push_back(creator.getID());
	if (DEBUG)
	{
		std::cout << "------------------" << std::endl;
		std::cout << "Channel created" << std::endl;
		std::cout << "Channel name : " << channel_name << std::endl;
		std::cout << "Creator : " << creator.getUsername() << " Creator ID : " << creator.getID() << std::endl;
		std::cout << "Topic : " << topic << std::endl;
		std::cout << "------------------" << std::endl;
		std::cout << "USER LIST : " << user_limit << std::endl;
		for (std::map<Client, bool>::iterator it = users_list.begin(); it != users_list.end(); it++)
			std::cout << "User : " << it->first.getUsername() << " Admin : " << it->second << std::endl;
		std::cout << "------------------" << std::endl;
		std::cout << "INVITED USERS" << std::endl;
		for (std::vector<int>::iterator it = invited_users.begin(); it != invited_users.end(); it++)
			std::cout << "(ID) - " << *it << std::endl;
		std::cout << "------------------" << std::endl;
	}
}

Channel::~Channel(){
	invited_users.clear();
}

Channel& Channel::operator=(const Channel &c){
	this->channel_name = c.channel_name;
	this->topic = c.topic;
	this->users_list = c.users_list;
	this->invited_users = c.invited_users;
	this->creator = c.creator;
	this->invite_only = c.invite_only;
	this->user_limit = c.user_limit;
	this->topic_autorization = c.topic_autorization;
	this->password = c.password;
	return *this;
}

void Channel::invite_user_by_admin(int user_ID){
	if (is_invite(user_ID))
		throw std::invalid_argument("User already invited");
	invited_users.push_back(user_ID);
}

void Channel::remove_user(Client user_to_remove, Client user_who_remove, Server &server) {
	if (user_to_remove == user_who_remove)
	{
		users_list.erase(user_to_remove);
		return ;
	}
	else if (users_list[user_who_remove] == false || user_to_remove.getID() != this->creator.getID())
		throw std::invalid_argument("You are not an admin or you can't remove the creator of the channel");
	this->users_list.erase(user_to_remove);
	(void)server;
}

bool Channel::check_if_user_is_admin(Client client) {
	if (this->users_list.find(client) == this->users_list.end())
		return false;
	if (users_list[client] == true)
		return true;
	return false;
}

bool Channel::check_if_user_is_in_channel(Client client){
	for (std::map<Client, bool>::iterator it = this->users_list.begin(); it != this->users_list.end(); it++){
		if (it->first.getID() == client.getID())
			return true;
	}
	return false;
}
void Channel::add_admin(Client user_to_add, Client user_who_add){

	if (users_list[user_who_add] == true || user_who_add.getID() == this->creator.getID())
	{
		if (users_list[user_to_add] == false)
			users_list[user_to_add] = true;
		else
			throw std::invalid_argument("User already admin in this channel");
	}
	else
		throw std::invalid_argument("You are not an admin");
}

void Channel::force_admin()
{
	if (users_list.size() == 0)
		return;
	users_list.begin()->second = true;
}
void Channel::remove_admin(Client user, Client admin){
	//if (admin.getAdmin() == true && user.getID() != this->creator.getID()){
	if (users_list[admin] == true){
		if (users_list[user] == true)
			users_list[user] = false;
		else
			throw std::invalid_argument("User is not an admin");
	}
	else if(user.getID() == this->creator.getID()){
		throw std::invalid_argument("you are trying to remove the creator of the channel (its the big boss)");
	}
	else{
		throw std::invalid_argument("You are not an admin");}
}

void Channel::set_password(std::string password, Client client){
	if (users_list[client] == true){
		if (DEBUG)
			std::cout << "Password set" << std::endl;
		this->password = password;
	}
}

std::string Channel::get_password(){
	return this->password;
}

void Channel::unset_password(Client client){
	if (users_list[client] == true){
		this->password.clear();
	}
}

bool Channel::is_invite_only(){
	return this->invite_only;
}

void Channel::join_request(Client user_to_add, std::string password, std::string channel_name){
	if (DEBUG)
		std::cout << "channel pwd : "<< this->password << " user pwd :" << password << std::endl;
	if (this->users_list.size() == static_cast<size_t>(this->user_limit) && this->user_limit != 0){
		send_RPL_message(471, NULL, user_to_add, NULL, channel_name);
		return;
	}
	if (this->invite_only == true && is_invite(user_to_add.getID()) == false)
	{
		send_RPL_message(473, NULL, user_to_add, NULL, channel_name);
		return;
	}
	if (this->password == password || this->password.empty()){
		if (DEBUG)
			std::cout << "User " << user_to_add.getUsername() << " joined the channel" << std::endl;
		this->users_list.insert(std::pair<Client, bool>(user_to_add, false));
	}
	else{
		send_RPL_message(475, NULL, user_to_add, NULL, channel_name);
	}
}

void Channel::change_topic(std::string topic_name, Client client){
	if (this->topic_autorization == false){
		Channel::send_msg_to_channel("You are not allowed to change the topic\n", client, false);
		throw std::invalid_argument("You are not allowed to change the topic(/mode t)");
		return;

	}
	if (users_list[client] == true){
		this->topic = topic_name;
	}
	else{
		Channel::send_msg_to_channel("You are not an admin or the creator of the channel\n", client, false);
		throw std::invalid_argument("You are not an admin or the creator of the channel");
	}
}

void Channel::change_topic_autorization(bool topic_autorization, Client client){
	if (users_list[client] == true){
		this->topic_autorization = topic_autorization;
	}
	else{
		throw std::invalid_argument("You are not an admin");
	}
}
void Channel::set_invite_only(bool invite_only, Client client){
	if (users_list[client] == true){
		this->invite_only = invite_only;
	}
}

void Channel::change_user_limit(int user_limit, Client client){
	if (user_limit < 0){
		throw std::invalid_argument("User limit can't be negative");
	}
	if (users_list[client] == true){
		this->user_limit = user_limit;
	}
	else{
		throw std::invalid_argument("You are not an admin");
	}
}

void Channel::send_msg_to_channel(std::string msg, Client client, bool MSG_OR_OTHER){
	if (DEBUG)
		std::cout << "sending msg to channel " << msg << std::endl;
	std::string temp;
	if (MSG_OR_OTHER == true)
		temp = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getRealname() + " PRIVMSG " + this->channel_name + " :" + msg + "\r\n";
	else
		temp = msg;
	if (DEBUG)
	{
		std::cout << temp << std::endl;
		std::cout << "user list size :" << this->users_list.size() << std::endl;
	}
	for (std::map<Client, bool>::iterator it = this->users_list.begin(); it != this->users_list.end(); ++it) {
		if (MSG_OR_OTHER == true)
		{
			if (it->first.getFd() != client.getFd())
			{
				send(it->first.getFd(), temp.c_str(), temp.size(), 0);
				if (DEBUG)
					std::cout << "fd: "<< it->first.getFd() << std::endl;
			}
		}
		else
			send(it->first.getFd(), temp.c_str(), temp.size(), 0);
	}
}

std::map<Client, bool> Channel::get_users_map(){
	return this->users_list;
}

void Channel::send_private_msg(std::string msg , Client who_send, Client who_receive)
{
	std::string final_msg;
	if (who_send.getID() == who_receive.getID())
	{
		final_msg = msg;
	}
	else
	{
		final_msg = "user : " + who_send.getUsername() + " send you : " + msg;
	}
	send(who_receive.getFd(), final_msg.c_str() , final_msg.size(), 0);
}

std::string Channel::get_channel_name(){
	return this->channel_name;
}

std::string Channel::get_topic(){
	return this->topic;
}

std::vector<Client> Channel::get_users_list(){
	std::vector<Client> users_list;
	if (this->users_list.empty())
		return users_list;
	for (std::map<Client, bool>::iterator it = this->users_list.begin(); it != this->users_list.end(); it++){
		users_list.push_back(it->first);
	}
	return users_list;
}

std::vector<Client> Channel::get_admin_users_list(){
	std::vector<Client> admin_list;
	for (std::map<Client, bool>::iterator it = this->users_list.begin(); it != this->users_list.end(); it++){
		if (it->second == true){
			admin_list.push_back(it->first);
		}
	}
	return admin_list;
}

Client Channel::get_creator(){
	return this->creator;
}


bool Channel::is_invite(int client_id) {
	for (std::vector<int>::iterator it = this->invited_users.begin(); it != this->invited_users.end(); it++){
		if (*it == client_id)
			return true;
	}
	return false;
}
