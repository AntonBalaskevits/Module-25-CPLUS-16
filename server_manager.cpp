#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <vector>

#include "server_manager.h"
#include "client_obj.h"
#include "data_base_manager.h"

//#include "global_objects.h"
// this header contains global client, envelope and data_base_object

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define ONE_BYTE 1
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

WSADATA wsaData;
int iResult;

SOCKET ListenSocket = INVALID_SOCKET;
SOCKET ClientSocket = INVALID_SOCKET;

struct addrinfo* result = NULL;
struct addrinfo hints;

char message[DEFAULT_BUFLEN];

char receiver_email[DEFAULT_BUFLEN];
// used only in ServerManager::when_clinet_sign() method

char flag[ONE_BYTE];

int iSendResult;
char recvbuf[DEFAULT_BUFLEN];
int recvbuflen = DEFAULT_BUFLEN;

int ServerManager::prepare_server_and_data_base_for_connection(DataBaseManager& db_manager_obj, 
	const char* db_pass, const char* db_name) {
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);

	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}
	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}


	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}


	std::cout << "server is listening for new connections" << std::endl;
	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	std::cout << "connection successfull" << std::endl;
	// No longer need server socket
	closesocket(ListenSocket);

	db_manager_obj.perpare_data_base_for_connection(db_pass, db_name);
}

void ServerManager::send_client_mail(DataBaseManager& db_manager_obj, Client& client_obj) {
	db_manager_obj.extract_only_delivered_msgs_from_db(client_obj.get_login());
	// extracting only delivered messages from data base and preparing them for sending it to client
	// receiver at this point is a client that signed in successfully
	client_obj.set_mail_box(db_manager_obj.get_reciver_mail_obj());
	// obtaining reference from db_manager_obj after extraction of data from chat data base

	int send_mail_size = 0;
	// this size will be used in client and server send/receive loop
	send_mail_size = client_obj.get_mail_box_size();
	// obtaining mail box size from client mail_box
	ZeroMemory(message, DEFAULT_BUFLEN);
	std::string string_value;
	string_value = std::to_string(send_mail_size);
	//string_value = std::to_string(client_obj.get_mail_box().size());
	// converting int value to char 
	for (auto i = 0; i < string_value.size(); i++) {
		message[i] = string_value[i];
	} // filling up message buffer
	iResult = send(ClientSocket, message, sizeof(message), 0);
	// sending size to a client so client program will know the loop size as well
	if (iResult == SOCKET_ERROR) {
		printf("send faild whit error: %d\n", WSAGetLastError());
		return;
	}
	if (send_mail_size == 0) {
		return;
	} // stoping program flow at this point if there is no delivered messages in data base

	int i = 0;
	int j = 0;
	std::vector<struct envelope> tmp_mail_box = client_obj.get_mail_box();
	// obtaining reference of mail_box from client
	ZeroMemory(message, DEFAULT_BUFLEN);
	for (i; i < client_obj.get_mail_box_size(); i++) {

		for (j = 0; j < tmp_mail_box[i].get_from_email().size(); j++) {
			message[j] = tmp_mail_box[i].get_from_email()[j];
		} // preparing email data

		iResult = send(ClientSocket, message, sizeof(message), 0);
		// sending sender email data to client
		if (iResult == SOCKET_ERROR) {
			printf("send faild whit error: %d\n", WSAGetLastError());
			return;
		}
		else {
			std::cout << "\n";
			std::cout << "send successfull! 1" << std::endl;
			std::cout << message << std::endl;
		}

		ZeroMemory(message, DEFAULT_BUFLEN);

		for (j = 0; j < tmp_mail_box[i].get_message().size(); j++) {
			message[j] = tmp_mail_box[i].get_message()[j];
		} // preparing message data 
		iResult = send(ClientSocket, message, sizeof(message), 0);
		// sending message data to client
		if (iResult == SOCKET_ERROR) {
			printf("send faild whit error: %d\n", WSAGetLastError());
			return;
		}
		else {
			std::cout << "\n";
			std::cout << "send successfull! 2" << std::endl;
			std::cout << message << std::endl;
		}

		ZeroMemory(message, DEFAULT_BUFLEN);

		for (j = 0; j < tmp_mail_box[i].get_date_time().size(); j++) {
			message[j] = tmp_mail_box[i].get_date_time()[j];
		} // preparing date and time data 
		iResult = send(ClientSocket, message, sizeof(message), 0);
		// sending date time data to client
		if (iResult == SOCKET_ERROR) {
			printf("send faild whit error: %d\n", WSAGetLastError());
			return;
		}
		else {
			std::cout << "\n";
			std::cout << "send successfull! 3" << std::endl;
			std::cout << message << std::endl;
		}

		ZeroMemory(message, DEFAULT_BUFLEN);
	}
	db_manager_obj.set_message_flags_to_sent(client_obj.get_login());
	client_obj.clean_mail_box();

}

void ServerManager::client_registration(DataBaseManager& db_manager_obj, Client& client_obj) {
	std::cout << "client registration" << std::endl;
	std::cout << "waiting for incoming data!" << std::endl;
	ZeroMemory(message, DEFAULT_BUFLEN);
	iResult = recv(ClientSocket, message, sizeof(message), 0);
	if (iResult == SOCKET_ERROR) {
		printf("recv faild whit error: %d\n", WSAGetLastError());
		return;
	}
	else {
		std::cout << "client name recived!" << std::endl;
		client_obj.set_name(message);
	} // setting name

	std::cout << "waiting for incoming data" << std::endl;
	ZeroMemory(message, DEFAULT_BUFLEN);
	iResult = recv(ClientSocket, message, sizeof(message), 0);
	if (iResult == SOCKET_ERROR) {
		printf("recv faild whit error: %d\n", WSAGetLastError());
		return;
	}
	else {
		std::cout << "client surname recived!" << std::endl;
		client_obj.set_surname(message);
	} // setting surname

	std::cout << "waiting for incoming data" << std::endl;
	ZeroMemory(message, DEFAULT_BUFLEN);
	iResult = recv(ClientSocket, message, sizeof(message), 0);
	if (iResult == SOCKET_ERROR) {
		printf("recv faild whit error: %d\n", WSAGetLastError());
		return;
	}
	else {
		std::cout << "client email recived!" << std::endl;
		client_obj.set_email(message);
	} // setting email

	std::cout << "waiting for incoming data" << std::endl;
	ZeroMemory(message, DEFAULT_BUFLEN);
	iResult = recv(ClientSocket, message, sizeof(message), 0);
	if (iResult == SOCKET_ERROR) {
		printf("recv faild whit error: %d\n", WSAGetLastError());
		return;
	}
	else {
		std::cout << "client login recived!" << std::endl;
		client_obj.set_login(message);
	} // setting login

	std::cout << "waiting for incoming data" << std::endl;
	ZeroMemory(message, DEFAULT_BUFLEN);
	iResult = recv(ClientSocket, message, sizeof(message), 0);
	if (iResult == SOCKET_ERROR) {
		printf("recv faild whit error: %d\n", WSAGetLastError());
		return;
	}
	else {
		std::cout << "client hash data recived!" << std::endl;
		client_obj.set_hash(message);
	} 	// and finaly setting hash
	
	bool check_result = false;
	int email_and_login_incorrect = 0;
	// email_and_login will stay ar '0' it will mean email is free and login is free to take
	ZeroMemory(flag, sizeof(flag));
	flag[0] = '1';
	// if flag will stay '1' it will mean that email and login are not exists
	// and it will notify client of successful registration
	check_result = db_manager_obj.check_email(client_obj.get_email());
	if (check_result == true) {
		std::cout << "client email exists, setting echo flag to '2'" << std::endl;
		ZeroMemory(flag, sizeof(flag));
		flag[0] = '2'; // email exists
		email_and_login_incorrect++;
	}
	check_result = db_manager_obj.check_login(client_obj.get_login());
	if (check_result == true) {
		std::cout << "client login exists, setting echo flag to '3'" << std::endl;
		ZeroMemory(flag, sizeof(flag));
		flag[0] = '3'; // login exists
		email_and_login_incorrect++;
	}
	if (email_and_login_incorrect == 2) {
		ZeroMemory(flag, sizeof(flag));
		flag[0] = '4'; // email and login is already taken
		iResult = send(ClientSocket, flag, sizeof(flag), 0);
		if (iResult == SOCKET_ERROR) {
			printf("send faild whit error: %d\n", WSAGetLastError());
		}
		return;
	}

	if (flag[0] != '1') { // if flag is not '1' something went wrong, sending echo back to client and return		
		iResult = send(ClientSocket, flag, sizeof(flag), 0);
		if (iResult == SOCKET_ERROR) {
			printf("send faild whit error: %d\n", WSAGetLastError());
		}
		return;
	}

	bool insert_result = false;
	insert_result = db_manager_obj.insert_client_data(client_obj.get_name(), client_obj.get_surname(), 
		client_obj.get_email(),client_obj.get_login());
	if (insert_result == false) {
		flag[0] = '5'; // setting flag to '5' will mean error on inserting data
		iResult = send(ClientSocket, flag, sizeof(flag), 0);
		if (iResult == SOCKET_ERROR) {
			printf("send faild whit error: %d\n", WSAGetLastError());
		}
		return;
	}
	insert_result = db_manager_obj.insert_client_hash(client_obj.get_email(), client_obj.get_hash());
	// writing hash into a hash_table
	if (insert_result == false) {
		flag[0] = '6'; // setting flag to '6' will mean error on inserting data
		iResult = send(ClientSocket, flag, sizeof(flag), 0);
		if (iResult == SOCKET_ERROR) {
			printf("send faild whit error: %d\n", WSAGetLastError());
		}
		return;
	}
	else {
		// and finaly sending flag back to client on successfull registration
		// at this point flag should remain '1' as it was set at begining -
		// - of checking and writing procedure
		iResult = send(ClientSocket, flag, sizeof(flag), 0);
		if (iResult == SOCKET_ERROR) {
			printf("send faild whit error: %d\n", WSAGetLastError());
			return;
		}
		else {
			client_obj.client_clean_up();
			// clening up after succsessfull registration
		}
	}
}

void ServerManager::auto_register(DataBaseManager& db_manager_obj) {
	Client client_obj;
	ZeroMemory(message, DEFAULT_BUFLEN);
	iResult = recv(ClientSocket, message, sizeof(message), 0);
	if (iResult == SOCKET_ERROR) {
		printf("recv faild whit error: %d\n", WSAGetLastError());
		return;
	}
	else {
		client_obj.set_name(message);
	}
	ZeroMemory(message, DEFAULT_BUFLEN);
	iResult = recv(ClientSocket, message, sizeof(message), 0);
	if (iResult == SOCKET_ERROR) {
		printf("recv faild whit error: %d\n", WSAGetLastError());
		return;
	}
	else {
		client_obj.set_surname(message);
	}
	ZeroMemory(message, DEFAULT_BUFLEN);
	iResult = recv(ClientSocket, message, sizeof(message), 0);
	if (iResult == SOCKET_ERROR) {
		printf("recv faild whit error: %d\n", WSAGetLastError());
		return;
	}
	else {
		client_obj.set_email(message);
	}
	ZeroMemory(message, DEFAULT_BUFLEN);
	iResult = recv(ClientSocket, message, sizeof(message), 0);
	if (iResult == SOCKET_ERROR) {
		printf("recv faild whit error: %d\n", WSAGetLastError());
		return;
	}
	else {
		client_obj.set_login(message);
	}
	ZeroMemory(message, DEFAULT_BUFLEN);
	iResult = recv(ClientSocket, message, sizeof(message), 0);
	if (iResult == SOCKET_ERROR) {
		printf("recv faild whit error: %d\n", WSAGetLastError());
		return;
	}
	else {
		client_obj.set_hash(message);
	}

	bool result = false;
	result = db_manager_obj.insert_client_data(client_obj.get_name(), client_obj.get_surname(),
		client_obj.get_email(), client_obj.get_login());
	if (result == false) {
		std::cout << "client data insert faild into data base!" << std::endl;
		return;
	}
	result = db_manager_obj.insert_client_hash(client_obj.get_email(), client_obj.get_hash());
	if (result == false) {
		std::cout << "client data insert faild into data base!" << std::endl;
		return;
	}
}

void ServerManager::client_sign_in(DataBaseManager& db_manager_obj, Client& client_obj) {
	std::cout << "client sign in" << std::endl;
	std::cout << "waiting for incoming data!" << std::endl;
	ZeroMemory(message, DEFAULT_BUFLEN);
	iResult = recv(ClientSocket, message, sizeof(message), 0);
	if (iResult == SOCKET_ERROR) {
		printf("recv faild whit error: %d\n", WSAGetLastError());
	}
	else {
		std::cout << "client login recived!" << std::endl;
		client_obj.set_login(message);
	} // setting login

	ZeroMemory(message, DEFAULT_BUFLEN);
	iResult = recv(ClientSocket, message, sizeof(message), 0);
	if (iResult == SOCKET_ERROR) {
		printf("recv faild whit error: %d\n", WSAGetLastError());
	}
	else {
		std::cout << "client hash recived!" << std::endl;
		client_obj.set_hash(message);
	} // setting hash

	bool check_result = false;
	check_result = db_manager_obj.check_login(client_obj.get_login());
	// checking received login 
	if (check_result == false) {
		ZeroMemory(flag, sizeof(flag));
		flag[0] = '1'; // login faild
		iResult = send(ClientSocket, flag, sizeof(flag), 0);
		if (iResult == SOCKET_ERROR) {
			printf("send faild whit error: %d\n", WSAGetLastError());
		}
		return;
	}

	check_result = db_manager_obj.check_client_hash(client_obj.get_login(), client_obj.get_hash());
	// checking received hash whit data base hash
	if (check_result == false) {
		ZeroMemory(flag, sizeof(flag));
		flag[0] = '2'; // password is incorrect
		iResult = send(ClientSocket, flag, sizeof(flag), 0);
		if (iResult == SOCKET_ERROR) {
			printf("send faild whit error: %d\n", WSAGetLastError());
		}
		return;
	}
	else {
		ZeroMemory(flag, sizeof(flag));
		flag[0] = '3'; // password is correct all checks are passed successfully at this point
		iResult = send(ClientSocket, flag, sizeof(flag), 0);
		if (iResult == SOCKET_ERROR) {
			printf("send faild whit error: %d\n", WSAGetLastError());
			return;
		}
	}
	
	std::string extracted_data;
	enum get_data_type { GET_NAME = 1, GET_SURNAME, GET_EMAIL };
	extracted_data = db_manager_obj.get_data_from_db(client_obj.get_login(), GET_NAME);
	client_obj.set_name(extracted_data);
	// setting client name in server, so server knows who he gonna be serving
	extracted_data = db_manager_obj.get_data_from_db(client_obj.get_login(), GET_SURNAME);

	client_obj.set_surname(extracted_data);
	// setting surname in server
	extracted_data = db_manager_obj.get_data_from_db(client_obj.get_login(), GET_EMAIL);
	client_obj.set_email(extracted_data);
	// and email
	client_obj.set_sign_in_status(sign_in_status::ONLINE);
	// setting status online at this point

	extracted_data = client_obj.get_name();
	// extracting client name from data base on successfull sign in
	// this name will be send back to client and stored in client side of program
	ZeroMemory(message, sizeof(message));
	for (auto i = 0; i < extracted_data.size(); i++) {
		message[i] = extracted_data[i];
	} // preparing message buffer 
	iResult = send(ClientSocket, message, sizeof(message), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send faild whit error: %d\n", WSAGetLastError());
		return;
	}

	extracted_data = client_obj.get_surname();
	// extracting login data
	ZeroMemory(message, sizeof(message));
	for (auto i = 0; i < extracted_data.size(); i++) {
		message[i] = extracted_data[i];
	} // preparing message buffer 
	iResult = send(ClientSocket, message, sizeof(message), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send faild whit error: %d\n", WSAGetLastError());
		return;
	}

	extracted_data = client_obj.get_email();
	// extracting email
	ZeroMemory(message, sizeof(message));
	for (auto i = 0; i < extracted_data.size(); i++) {
		message[i] = extracted_data[i];
	} // preparing message buffer 
	iResult = send(ClientSocket, message, sizeof(message), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send faild whit error: %d\n", WSAGetLastError());
		return;
	}

	db_manager_obj.extract_all_receiver_msgs_from_db(client_obj.get_login());
	// extracting all receivers messages from data base and preparing them for sending it to client
	// receiver at this point is a client that signed in successfully
	client_obj.set_mail_box(db_manager_obj.get_reciver_mail_obj());
	// obtaining reference from db_manager_obj after extraction of data from chat data base

	int send_mail_size = 0;
	// this size will be used in client and server send/receive loop
	send_mail_size = client_obj.get_mail_box_size();
	// obtaining mail box size from client mail_box
	ZeroMemory(message, DEFAULT_BUFLEN);
	std::string string_value;
	string_value = std::to_string(send_mail_size);
	//string_value = std::to_string(client_obj.get_mail_box().size());
	// converting int value to char 
	for (auto i = 0; i < string_value.size(); i++) {
		message[i] = string_value[i];
	} // filling up message buffer
	iResult = send(ClientSocket, message, sizeof(message), 0);
	// sending size to a client so client program will know the loop size as well
	if (iResult == SOCKET_ERROR) {
		printf("send faild whit error: %d\n", WSAGetLastError());
		return;
	}
	int i = 0; // used in loops that prepares data for sending back to client
	int j = 0;
	std::vector<struct envelope> tmp_mail_box = client_obj.get_mail_box();
	// obtaining reference of mail_box from client
	ZeroMemory(message, DEFAULT_BUFLEN);
	for (i; i < client_obj.get_mail_box_size(); i++) {

		for (j = 0; j < tmp_mail_box[i].get_from_email().size(); j++) {
			message[j] = tmp_mail_box[i].get_from_email()[j];
		} // preparing email data

		iResult = send(ClientSocket, message, sizeof(message), 0);
		// sending sender email data to client
		if (iResult == SOCKET_ERROR) {
			printf("send faild whit error: %d\n", WSAGetLastError());
			return;
		}

		ZeroMemory(message, DEFAULT_BUFLEN);

		for (j = 0; j < tmp_mail_box[i].get_message().size(); j++) {
			message[j] = tmp_mail_box[i].get_message()[j];
		} // preparing message data 
		iResult = send(ClientSocket, message, sizeof(message), 0);
		// sending message data to client
		if (iResult == SOCKET_ERROR) {
			printf("send faild whit error: %d\n", WSAGetLastError());
			return;
		}

		ZeroMemory(message, DEFAULT_BUFLEN);

		for (j = 0; j < tmp_mail_box[i].get_date_time().size(); j++) {
			message[j] = tmp_mail_box[i].get_date_time()[j];
		} // preparing date and time data 
		iResult = send(ClientSocket, message, sizeof(message), 0);
		// sending date time data to client
		if (iResult == SOCKET_ERROR) {
			printf("send faild whit error: %d\n", WSAGetLastError());
			return;
		}

		ZeroMemory(message, DEFAULT_BUFLEN);
	}
	db_manager_obj.set_message_flags_to_sent(client_obj.get_login());
	// all messages that was extracted from chat data base status will be marked as 'sent' in message_table
	client_obj.clean_mail_box();
	// cleaning up mail box 
	// couse after sign in all not read messages was send automaticaly
}

void ServerManager::when_client_signed_in(DataBaseManager& db_manager_obj, Client& client_obj) {
	std::string received_data;
	std::string recipient_email;
	char sb_flag; // send back flag
	bool check = false; // checks results of db_manager_functions
	int in_flag = 0; // used as received flag that was converted into ints
	bool on = true;
	while (on) {
		ServerManager::send_client_mail(db_manager_obj, client_obj);
		
		// auto send existing unread mail from chat data base to signed in client
		ZeroMemory(flag, sizeof(flag));
		iResult = recv(ClientSocket, flag, sizeof(flag), 0);
		if (iResult == SOCKET_ERROR) {
			printf("recv faild whit error: %d\n", WSAGetLastError());
			break;
		}
		in_flag = std::atoi(flag);
		switch (in_flag) {
		case 1:
			db_manager_obj.set_message_flags_to_read(client_obj.get_login());
			// after client read message the flag in data base will be set to read
			break;
		case 2:
			ZeroMemory(flag, sizeof(flag));
			flag[0] = '1';
			ZeroMemory(receiver_email, sizeof(receiver_email));
			iResult = recv(ClientSocket, receiver_email, sizeof(receiver_email), 0);
			// receiving client email
			if (iResult == SOCKET_ERROR) {
				printf("recv faild whit error: %d\n", WSAGetLastError());
				continue;
			}
			check = db_manager_obj.check_email(receiver_email);
			// checking email if it exists in data base, if false email is not present or typed in incorrecly
			if (check == false) {
				flag[0] = '2';
			}
			ZeroMemory(message, sizeof(message));
			iResult = recv(ClientSocket, message, sizeof(message), 0);
			// receiving incoming message from client
			if (iResult == SOCKET_ERROR) {
				printf("recv faild whit error: %d\n", WSAGetLastError());
				continue;
			}
			if (flag[0] != '2') { // if flag is not '2' means everything is fine message can be inserted into chat data base
				check = db_manager_obj.insert_message_into_db(client_obj.get_email(), receiver_email, message);
				if (check == false) {
					flag[0] = '3';
				}
			}
			sb_flag = flag[0];
			ServerManager::send_flag(sb_flag);
			// sending reply flag back to client
			break;
		case 3:
			client_obj.client_clean_up();
			client_obj.clean_mail_box();
			client_obj.set_sign_in_status(sign_in_status::OFFLINE);
			on = false;
			// client whishes to sign out
			// cleaning client object in server_manager
			break;
		}
	}
}

void ServerManager::send_flag(const char& input_flag) {
	ZeroMemory(flag, sizeof(flag));
	flag[0] = input_flag;
	std::cout << "send flag : " << flag[0] << std::endl;
	iResult = send(ClientSocket, flag, sizeof(flag), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send faild whit error: %d\n", WSAGetLastError());
	}
}

int ServerManager::receive_flag() {
	int return_flag = 0;
	ZeroMemory(flag, sizeof(flag));
	iResult = recv(ClientSocket, flag, sizeof(flag), 0);
	if (iResult == SOCKET_ERROR) {
		printf("recv faild whit error: %d\n", WSAGetLastError());
		return -1;
	}
	else {
		return_flag = std::atoi(flag);
		return return_flag;
	}
}

void ServerManager::shut_down(DataBaseManager& db_manager_obj) {
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return;
	}
	else {
		std::cout << "server shut down successfull" << std::endl;
	}
	// cleanup
	closesocket(ClientSocket);
	WSACleanup();
	db_manager_obj.desconnect_data_base();
}
