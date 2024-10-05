#include "Client.hpp"
/**********************Canonic form(useless)***************************/
Client::Client(std::string username, int fd,int ID, bool admin){
	if (username.length() > 30)
		username.resize(30);
	this->username = username;
	this->nickname = "default";
	this->fd = fd;
	this->admin = admin;
	this->user_ID = ID;
	this->put_pwd = false;
	this->topic_autorization = false;
	if (DEBUG)
		std::cout << "Username : " << this->username << std::endl;
}

Client::~Client(){
}

Client::Client(const Client &c){
	this->realname = c.realname;
	this->username = c.username;
	this->nickname = c.nickname;
	this->ip = c.ip;
	this->topic_autorization = c.topic_autorization;
	this->messageBuffer = c.messageBuffer;
	this->fd = c.fd;
	this->admin = c.admin;
	this->put_pwd = c.put_pwd;
	this->user_ID = c.user_ID;
	this->current_channel = c.current_channel;
}

Client& Client::operator=(const Client &c){
	this->realname = c.realname;
	this->username = c.username;
	this->nickname = c.nickname;
	this->put_pwd = c.put_pwd;
	this->ip = c.ip;
	this->topic_autorization = c.topic_autorization;
	this->messageBuffer = c.messageBuffer;
	this->fd = c.fd;
	this->admin = c.admin;
	this->user_ID = c.user_ID;
	this->current_channel = c.current_channel;
	return *this;
}

/************************geter & seter******************************/
bool Client::getput_pwd() const{
	return this->put_pwd;
}

void Client::setput_pwd(bool put_pwd){
	this->put_pwd = put_pwd;
}

std::string Client::getUsername() const{
	return this->username;

}
std::string Client::getUsername() {
	return this->username;
}

std::string Client::getNickname() const{
	return this->nickname;
}

Channel Client::getCurrentChannel() const{
	if (this->current_channel == NULL)
		throw std::runtime_error("Client is not in a channel");
	return *this->current_channel;
}

void Client::setCurrentChannel(Channel *channel){
	this->current_channel = channel;
}

void Client::setNickname(std::string nickname){
	if (nickname.length() > 30)
		nickname.resize(30);
	this->nickname = nickname;
}

void Client::setUsername(std::string username){
	if (username.length() > 30)
		username.resize(30);
	this->username = username;
	if (DEBUG)
		std::cout << "Username : " << this->username << std::endl;
}
void Client::setRealname(std::string realname){
	this->realname = realname;
}

std::string Client::getRealname() const{
	return this->realname;
}

int Client::getFd() const{
	return this->fd;
}

std::string Client::getIp(){
	return this->ip;
}

void Client::setTopicAutorization(bool topic_autorization){
	this->topic_autorization = topic_autorization;
}

bool Client::getTopicAutorization(){
	return this->topic_autorization;
}

int Client::getID() const{
	return this->user_ID;
}

std::string& Client::getMessageBuffer(){
	return this->messageBuffer;
}

void Client::setMessageBuffer(std::string& message){
	this->messageBuffer.clear();
	this->messageBuffer.append(message);
}