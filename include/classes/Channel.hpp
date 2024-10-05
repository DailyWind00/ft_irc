#pragma once 

#include <iostream>
#include <vector>
#include <map>
#include "main.hpp"
class Channel{
	private:
		int user_limit;
		std::string channel_name;
		std::string password;
		std::string topic;
		std::map<Client, bool> users_list;
		Client &creator;
		std::vector<int> invited_users;
		bool invite_only;
		bool topic_autorization;
	public:
		Channel(std::string channel_name, Client &creator);
		~Channel();
		//Channel(const Channel &c);
		Channel& operator=(const Channel &c);

		void invite_user_by_admin(int user_ID);
		void force_admin();
		void join_request(Client user_to_add, std::string password, std::string channel_name);
		void remove_user(Client user_to_remove, Client user_who_remove, Server &server);
		void add_admin(Client user_to_add, Client user_who_add);
		void remove_admin(Client user_to_remove, Client user_who_remove);
		void change_topic(std::string topic_name, Client client);
		void change_topic_autorization(bool topic_autorization, Client client);
		void set_password(std::string password, Client client);
		void unset_password(Client client);
		void change_user_limit(int user_limit, Client client);
		bool check_if_user_is_admin(Client client);
		bool check_if_user_is_in_channel(Client client);
		void set_invite_only(bool invite_only, Client client);
		void send_msg_to_channel(std::string msg, Client client, bool MSG_OR_OTHER);
		void send_private_msg(std::string msg, Client who_send , Client who_receive);
		std::map<Client, bool> get_users_map();
		std::vector<int> get_invite_map();
		bool is_invite(int client_id);
		bool is_invite_only();

		std::string get_channel_name();
		std::string get_topic();
		std::vector<Client> get_users_list();
		std::vector<Client> get_admin_users_list();
		Client get_creator();
		std::string get_password();
};
