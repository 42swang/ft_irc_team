#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <vector>
#include "Channel.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include <string>
#include "Util.hpp"

class Server;

std::vector<Channel *>::iterator findChannel(std::vector<Channel *>::iterator begin, std::vector<Channel *>::iterator end, std::string item)
{
	std::vector<Channel *>::iterator it = begin;
	while (it != end)
	{
		if ((*it)->getChannelName() == item)
			return (it);
		it++;
	}
	return (end);
}

std::vector<Client *>::iterator findClient(std::vector<Client *>::iterator begin, std::vector<Client *>::iterator end, std::string item)
{
	std::vector<Client *>::iterator it = begin;
	while (it != end)
	{
		if ((*it)->getNickName() == item)
			return (it);
		it++;
	}
	return (end);
}
// ERR_NONICKNAMEGIVEN             ERR_ERRONEUSNICKNAME
// ERR_NICKNAMEINUSE               ERR_NICKCOLLISION

class Command
{
private:
	bool isLetter(char c)
	{
		if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'))
			return true;
		else
			return false;
	};
	bool isNumber(char c)
	{
		if ('0' <= c && c <= '9')
			return true;
		else
			return false;
	};
	bool isSpecial(char c)
	{
		if (c == '-' || c == '[' || c == ']' || c == '\\' || c == '`' || c == '^' || c == '{' || c == '}')
			return true;
		else
			return false;
	};
	bool isDuplication(std::string s, std::vector<Client *> &clientList)
	{
		// clientList 돌면서 s랑 똑같은 게 있는 지 찾아서 t/f 반환
		for (int i = 0; i < (int)(clientList.size()); i++)
		{
			if (clientList[i]->getNickName() == s)
				return true;
		}
		return false;
	}
	bool nickValidate(std::string s)
	{
		if (0 >= s.length() || s.length() > 9)
			return false;
		if (isLetter(s[0]) == false)
			return false;
		for (int i = 1; i < (int)(s.length()); i++)
		{
			if (!isLetter(s[i]) && !isSpecial(s[i]) && !isNumber(s[i]))
				return false;
		}
		return true;
	}

public:
	void nick(std::vector<std::string> s, std::vector<Client *> &clientList, Client *client)
	{

		if (isDuplication(s[1], clientList))
		{
			std::cout << "nick dup\n";
			// 변경시 중복이면 중복프로토콜메시지(닉변경실패)
			return;
		}
		if (!nickValidate(s[1]))
		{
			if (s[1].find('\r'))
				std::cout << "캐리지리턴 포함" << std::endl;
			std::cout << "s1 : " << s[1] << "\n" << "닉네임규칙안맞음\n";
			return;
		}

		client->appendMsgBuffer(":" + client->getNickName() + " NICK " + s[1] + "\r\n");
		client->setNickName(s[1]);
		//중복 체크 ->응답
		//닉네임 규칙 체크 ->응답

		//맞으면 닉네임 바꾸고 응답 보내기

		std::cout << "i am nick\n";
		print_clientList(clientList);

	};																	   // NICK <parameter>
	void user(std::vector<std::string> s, Client *client) {
		client->setUser(s[1], s[2], s[3], appendStringColon(4, s));
		std::cout << "user called" << std::endl;

	}; // USER <username> <hostname> <servername> <realname>
	void join(std::vector<std::string> s, Client *client, std::vector<Channel *> &channelList)
	{
		// JOIN ( <channel> *( "," <channel> ) [ <key> *( "," <key> ) ] ) / "0"
		// JOIN #foo,#bar
		std::vector<std::string> joinChannel = split(s[1], ",");
		std::vector<std::string>::iterator it = joinChannel.begin();
		while (it != joinChannel.end())
		{
			//클라이언트.채널리스트 갱신, 채널.클라이언트리스트 갱신//join

			std::vector<Channel *>::iterator channelBegin = channelList.begin();
			std::vector<Channel *>::iterator channelEnd = channelList.end();
			std::vector<Channel *>::iterator findIter = findChannel(channelBegin, channelEnd, *it);

			if (findIter != channelList.end())
			{
				// 채널 객체가 이미 존재하는 경우
				(*findIter)->addMyClientList(client);
				client->addChannelList(*findIter);
				std::cout << "채널에 추가됨\n";
				//TODO: 채널접속시 채널에 접속해 있는 사람들에게 알림메세지
				client->appendMsgBuffer(":" + client->getNickName() + "!" + client->getUserName() + "@" + client->getServerName() +  " JOIN " + (*findIter)->getChannelName() + "\r\n");
				//:이름 JOIN 채널명
			}
			else
			{
				// 새 채널 만들기; 서버.채널리스트에(( 추가))
				channelList.push_back(new Channel(*it));
				// 위에서 생성된 채널 클래스 객체에 유저 추가
				channelList.back()->addMyClientList(client);
				// 클라이언트.채널리스트 갱신, 채널.클라이언트리스트 갱신
				client->addChannelList(channelList.back());
				client->appendMsgBuffer(":" + client->getNickName() + "!" + client->getUserName() + "@" + client->getServerName() + " JOIN " + channelList.back()->getChannelName() + "\r\n");
			}
			it++;
		}
		//채널의 클라이언트 리스트 돌면서 다 메세지 보내기


		print_channelList(channelList);

		// //connect 127.0.0.1 6667 0000
		//파싱해서 채널 이름 따오고
		//서버.채널리스트에 없으면 채널 객체 만들어서 채널 리스트에 추가
		//클라이언트.채널리스트 갱신, 채널.클라이언트리스트 갱신

		std::cout << "called" << s[0] << std::endl;
	};
	void kick(std::vector<std::string> s) {
		std::cout << "called" << s[0] << std::endl;
	}; // KICK <channel> <user> [<comment>]//KICK <channel> <user> [<comment>]
	void privmsg(std::vector<std::string> s, Client *client, std::vector<Client *> clientList, std::vector<Channel *> channelList)
	{
		// privmsg <target> :text
		std::vector<std::string> target = split(s[1], ",");
		std::vector<std::string>::iterator targetIt = target.begin();
		while (targetIt != target.end())
		{
			if((*targetIt)[0] == '#')
			{
				std::vector<Channel *>::iterator channelIt = findChannel(channelList.begin(), channelList.end(), *targetIt);
				//TODO :find 실패시 예외처리
				channelMessage(appendStringColon(2, s), client, *channelIt);
			}
			else
			{
				std::vector<Client *>::iterator clientIt = findClient(clientList.begin(), clientList.end(), *targetIt);
				//TODO :find 실패시 예외처리
				personalMessage(appendStringColon(2, s), client->getNickName(), *clientIt);
			}
			targetIt++;
		}
		std::cout << "i am privmsg\n";
	};
	void  personalMessage(std::string msg, std::string senderName, Client * receiver)
	{
		receiver->appendMsgBuffer(":" + senderName + " PRIVMSG " + receiver->getNickName() + " " + msg + "\r\n");
		std::cout << "fd : " << receiver->getClientFd() << std::endl;
	};

	void  channelPersonalMessage(std::string msg, std::string senderName, Client *client, std::string channelName)
	{
		client->appendMsgBuffer(":" + senderName + " PRIVMSG " + channelName + " " + msg + "\r\n");
		std::cout << "fd : " << client->getClientFd() << std::endl;
	};

	void channelMessage(std::string msg, Client *client, Channel * channel)
	{
		std::vector<Client *> clients = channel->getMyClientList();
		std::vector<Client *>::iterator clientsIt = clients.begin();
		while(clientsIt != clients.end())
		{
			if (client->getClientFd() != (*clientsIt)->getClientFd())
				channelPersonalMessage(msg, client->getNickName(), *clientsIt, channel->getChannelName());
			clientsIt++;
		}
	};																   // PRIVMSG <msgtarget> <text to be sent>
	void pass(std::vector<std::string> s) {
		std::cout << "called" << s[0] << std::endl;
	}; // PASS <password>
	void part(std::vector<std::string> s) {
		std::cout << "called" << s[0] << std::endl;
	}; // PART <channel> *( "," <channel> ) [ <Part Message> ]
	void quit(std::vector<std::string> s)
	{
		//소켓지우고
		//리스트에서 지우고
		std::cout << "called" << s[0] << std::endl;
	}; // QUIT [<Quit message>]

	/*
	:irc.example.com 001 borja :Welcome to the Internet Relay Network borja!borja@polaris.cs.uchicago.edu
	:irc.example.com 433 * borja :Nickname is already in use.
	:irc.example.org 332 borja #cmsc23300 :A channel for CMSC 23300 students

	:<server> 311 <nick> <nick> <user> <host> * :<real name>
	*/

	// void whois(std::vector<std::string> s, Client *client)
	// {
	// 	client->appendMsgBuffer("311 " + client->getNickName() + " " + client->getNickName() + " ~" + client->getUserName() + " " + client->getServerName() + " * " + client->getRealName() + "\r\n");//todo~~

	// 	//ircname, 서버 어딘지, ~()@ Endofwhois
	// 	std::cout << "called" << s[0] << std::endl;
	// };
	void welcome(std::vector<std::string> cmd, Client *client, std::vector<Client *> clientList)
	{
		//닉 체크하고 최초 닉네임 설정..
		//TODO : clientList 길이 체크하고 3개 명령어 다 처리한 후 welcome 전송
		//유저 설정
		std::vector<std::string>::iterator cmd_it = cmd.begin();
		while (cmd_it != cmd.end())
		{
			std::vector<std::string> result = split(*cmd_it, " ");
			if (result[0] == "PASS")
			{
				std::cout << "pass\n";
			}
			else if (result[0] == "NICK")
			{
				if (!nickValidate(result[1]))
				{
					std::cout << "닉네임규칙안맞음\n";
					return;
				}
				if (isDuplication(result[1], clientList))
				{
					std::cout << "nick dup\n";
					result[1].append("_");
					// 중복이면 마지막에 '_' 추가하기()->웰컴단계에서
					return;
				}
				client->setNickName(result[1]);
			}
			else if (result[0] == "USER")
			{
				user(result, client);
			}
			cmd_it++;
		}
		// print_clientList(clientList);
		client->appendMsgBuffer("001 " + client->getNickName() + " :Welcome to the Internet Relay Network " + client->getNickName() + "\r\n");
		std::cout << "welcome\n";
	}
};

#endif
