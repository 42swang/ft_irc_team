/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: swang <swang@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 16:26:45 by swang             #+#    #+#             */
/*   Updated: 2022/06/02 16:34:44 by swang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"
#include <iostream>

Client::Client(int clientFd) : _clientFd(clientFd), _regist(0) {}

std::string Client::getMsgBuffer()
{
	return _msgBuffer;
}

std::vector<std::string>::iterator Client::findChannel(std::string item)
{
	std::vector<std::string>::iterator it = _myChannelList.begin();
	while (it != _myChannelList.end())
	{
		if ((*it) == item)
			return (it);
		it++;
	}
	return (_myChannelList.end());
}

void Client::removeChannelList(std::vector<std::string>::iterator it)
{
	_myChannelList.erase(it);
}

void Client::removeChannel(std::string serverName)
{
	std::vector<std::string>::iterator it = findChannel(serverName);
	if (it != _myChannelList.end())
		removeChannelList(it);
}

std::vector<std::string> & Client::getMyChannelList()
{
	return(_myChannelList);
}

void Client::clearMsgBuffer()
{
	_msgBuffer = "";
}

void Client::appendMsgBuffer(std::string msgBuffer)
{
	_msgBuffer.append(msgBuffer);
}

std::string &Client::getRecvBuffer()
{
	return _recvBuffer;
}

void Client::appendRecvBuffer(std::string recvBuffer)
{
	_recvBuffer.append(recvBuffer);
}


void Client::clearRecvBuffer()
{
	_recvBuffer = "";
}


int Client::getClientFd()
{
	return _clientFd;
}

void Client::addChannelList(std::string channelName)
{
	_myChannelList.push_back(channelName);
}

void Client::setNickName(std::string nickName)
{
	_nickName = nickName;
}

std::string Client::getNickName()
{
	return _nickName;
}

void Client::setUser(std::string userName, std::string hostName, std::string serverName, std::string realName){
	_userName = userName;
	_hostName = hostName;
	_serverName = serverName;
	_realName = realName;
}

std::string Client::getUserName()
{
	return _userName;
}

std::string Client::getHostName()
{
	return _hostName;
}

std::string Client::getServerName()
{
	return _serverName;
}

std::string Client::getRealName()
{
	return _realName;
}

void Client::DebugPrint()
{
	std::cout << "nickName : " << _nickName << std::endl;
	std::cout << "userName : " << _userName << std::endl;
	std::cout << "hostName : " << _hostName << std::endl;
	std::cout << "serverName : " << _serverName << std::endl;
	std::cout << "realName : " << _realName << std::endl;
	std::cout << "clientFd : " << _clientFd << std::endl;
	std::cout << "msgBuffer : " << _msgBuffer << std::endl;
}

void Client::setRegist(int bit)
{
	_regist |= bit;
}

unsigned char Client::getRegist()
{
	return (_regist);

}