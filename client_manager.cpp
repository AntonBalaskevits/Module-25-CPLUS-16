#include "client_manager.h"
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include "client_obj.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define ONE_BYTE 1
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
char flag[ONE_BYTE];
char message[DEFAULT_BUFLEN];

WSADATA wsaData;
SOCKET ConnectSocket = INVALID_SOCKET;
struct addrinfo* result = NULL,
    * ptr = NULL,
    hints;
int iResult;

int ClientManager::prepare_client_for_connection(const char* pc_name) {
	
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(pc_name, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
            continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}
	std::cout << "connection successfull" << std::endl;
}

void ClientManager::send_client_data(const std::string& input_data) {
	ZeroMemory(message, sizeof(message));
	for (auto i = 0; i < input_data.size(); i++) {
		message[i] = input_data[i];
	}

	iResult = send(ConnectSocket, message, sizeof(message), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send faild whit error: %d\n", WSAGetLastError());
	}
}

void ClientManager::send_flag(const char& input_flag) {
	ZeroMemory(flag, sizeof(flag));
	flag[0] = input_flag;
	iResult = send(ConnectSocket, flag, sizeof(flag), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send faild whit error: %d\n", WSAGetLastError());
	}
}
int ClientManager::receive_flag() {
	int r_flag = 0; // return flag
	ZeroMemory(flag, sizeof(flag));
	iResult = recv(ConnectSocket, flag, sizeof(flag), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send faild whit error: %d\n", WSAGetLastError());
		return -1;
	}
	r_flag = std::atoi(flag);
	return r_flag;
}

void ClientManager::when_client_signed_in(Client& client_obj) {
	std::string input_data;
	int check_incoming_mail_size = 0;
	int input = 0;
	int returned_flag = 0;
	bool on = true;
	while (on) {
		check_incoming_mail_size = ClientManager::get_incoming_mail_box_size();
		if (check_incoming_mail_size != -1 && check_incoming_mail_size != 0) {
			ClientManager::check_for_incoming_mail(client_obj, check_incoming_mail_size);
		} // this will be auto check for incoming mail on each new loop
		std::cout << "\n";
		std::cout << "hello " << client_obj.get_name() << " " << client_obj.get_surname() << std::endl;
		if (client_obj.get_mail_box_size() == 1) {
			std::cout << "you have " << client_obj.get_mail_box_size() << " message in your mail box!" << std::endl;
		}
		else {
			std::cout << "you have " << client_obj.get_mail_box_size() << " messages in your mail box!" << std::endl;
		}
		std::cout << "read messages enter 1" << std::endl;
		std::cout << "send messages enter 2" << std::endl;
		std::cout << "sign out            3" << std::endl;
		std::cout << "input : ";
		std::cin >> input;

		switch (input) {
		case 1:
			if (client_obj.get_mail_box_size() == 0) {
				std::cout << "you currently don't have any new messages!" << std::endl;
				continue;
			}
			ClientManager::send_flag('1');
			client_obj.display_all_envelopes();
			client_obj.clean_mail_box();
			// cleaning mail box after display
			break;
		case 2:
			ClientManager::send_flag('2');
			std::cout << "receipient email : ";
			std::cin >> input_data;
			ClientManager::send_client_data(input_data);

			std::cout << "message : ";
			std::cin >> input_data;
			ClientManager::send_client_data(input_data);

			returned_flag = ClientManager::receive_flag();
			if (returned_flag == 1) {
				std::cout << "message send successfully" << std::endl;
			}
			else if (returned_flag == 2) {
				std::cout << "ERROR: recipient email does not exists, or it was typed in incorrectly" << std::endl;
				std::cout << "please try again" << std::endl;
			}
			else if (returned_flag == 3) {
				std::cout << "ERROR: something went wrong while sending message" << std::endl;
				std::cout << "please try agian" << std::endl;
            }

			break;
		case 3:
			ClientManager::send_flag('3');
			client_obj.set_sign_in_status(sign_in_status::OFFLINE);
			client_obj.client_clean_up();
			client_obj.clean_mail_box();
			// cleaning fields after sign out
			on = false;
			break;
		default:
			std::cout << "bad input, please try again" << std::endl;
			break;
		}
	}
}

int ClientManager::get_incoming_mail_box_size() {
	int receive_mail_size = 0;
	ZeroMemory(message, sizeof(message));
	iResult = recv(ConnectSocket, message, sizeof(message), 0);
	if (iResult == SOCKET_ERROR) {
		printf("recv faild whit error: %d\n", WSAGetLastError());
		return -1;
	}
	receive_mail_size = std::atoi(message);
	return receive_mail_size;
}

void ClientManager::check_for_incoming_mail(Client& client_obj, int receive_mail_size) {
	envelope received_envelope;
	// converting chars back to int
	ZeroMemory(message, sizeof(message));
	for (auto i = 0; i < receive_mail_size; i++) {

		iResult = recv(ConnectSocket, message, sizeof(message), 0);
		if (iResult == SOCKET_ERROR) {
			printf("recv faild whit error: %d\n", WSAGetLastError());
			return;
		}
		else {
			received_envelope.set_from_email(message);
		}

		ZeroMemory(message, sizeof(message));

		iResult = recv(ConnectSocket, message, sizeof(message), 0);
		if (iResult == SOCKET_ERROR) {
			printf("recv faild whit error: %d\n", WSAGetLastError());
			return;
		}
		else {
			received_envelope.set_message(message);
		}

		ZeroMemory(message, sizeof(message));

		iResult = recv(ConnectSocket, message, sizeof(message), 0);
		if (iResult == SOCKET_ERROR) {
			printf("recv faild whit error: %d\n", WSAGetLastError());
			return;
		}
		else {
			received_envelope.set_date_time(message);
		}
		client_obj.insert_envelope(received_envelope);
		//ZeroMemory(message, sizeof(message));
	}
}

void ClientManager::client_registration(Client& client_obj) {
	ZeroMemory(flag, ONE_BYTE);
	flag[0] = '1'; 
	//send(ConnectSocket, flag, sizeof(flag), 0);
	// sending falg to server for registration process
	std::string input; // for client input data

	std::cout << "\n";
	std::cout << "please enter your details down below" << std::endl;
	std::cout << "name : ";
	std::cin >> input;
	client_obj.set_name(input);
	input.clear();
	ClientManager::send_client_data(client_obj.get_name());

	std::cout << "surname : ";
	std::cin >> input;
	client_obj.set_surname(input);
	input.clear();
	ClientManager::send_client_data(client_obj.get_surname());

	std::cout << "email : ";
	std::cin >> input;
	client_obj.set_email(input);
	input.clear();
	ClientManager::send_client_data(client_obj.get_email());

	std::cout << "login : ";
	std::cin >> input;
	client_obj.set_login(input);
	input.clear();
	ClientManager::send_client_data(client_obj.get_login());

	std::cout << "password : ";
	std::cin >> input;
	client_obj.set_hash(input);
	input.clear();
	ClientManager::send_client_data(client_obj.get_hash());
	client_obj.client_clean_up();
	// cleaning up, clearing name, surname, email, login and hash fields

	ZeroMemory(flag, sizeof(flag));
	iResult = recv(ConnectSocket, flag, sizeof(flag), 0);
	if (iResult == SOCKET_ERROR) {
		printf("recv faild whit error: %d\n", WSAGetLastError());
	}

	if (flag[0] == '1') {
		std::cout << "registration succsessfull" << std::endl;
	}
	else if (flag[0] == '2') {
		std::cout << "ERROR: registration faild, email is already taken!" << std::endl;
		std::cout << "please try again, later" << std::endl;
	}
	else if (flag[0] == '3') {
		std::cout << "ERROR: registration faild, login is already taken!" << std::endl;
		std::cout << "please try again, later" << std::endl;
	}
	else if (flag[0] == '4') {
		std::cout << "ERROR: registration faild, email and login is alredy taken!" << std::endl;
		std::cout << "please try again, later" << std::endl;
	}
	else if (flag[0] == '5') {
		std::cout << "ERROR: registration faild, something went wrong" << std::endl;
		std::cout << "please try again, later" << std::endl;
	}
	else if (flag[0] == '6') {
		std::cout << "ERROR: registration faild, something went wrong" << std::endl;
		std::cout << "please try again, later" << std::endl;
	}
 }

void ClientManager::client_sign_in(Client& client_obj) {
	client_obj.client_clean_up();
	// cleaning client object 
	ZeroMemory(flag, ONE_BYTE);
	flag[0] = '2';
	//send(ConnectSocket, flag, sizeof(flag), 0);
	// sending falg to server for sign in process
	std::string input; // for client input data

	std::cout << "\n";
	std::cout << "please enter your details down below!" << std::endl;
	std::cout << "login    : ";
	std::cin >> input;
	client_obj.set_login(input);
	ClientManager::send_client_data(client_obj.get_login());

	std::cout << "password : ";
	std::cin >> input;
	client_obj.set_hash(input);
	ClientManager::send_client_data(client_obj.get_hash());

	ZeroMemory(flag, sizeof(flag));
	iResult = recv(ConnectSocket, flag, sizeof(flag), 0);
	// waiting for answer from server
	if (iResult == SOCKET_ERROR) {
		printf("recv faild whit error: %d\n", WSAGetLastError());
	}

	if (flag[0] == '1') {
		std::cout << "ERROR: login is incorrect please try again" << std::endl;
	}
	else if (flag[0] == '2') {
		std::cout << "ERROR: password is incorrect please try again" << std::endl;
	}
	else if (flag[0] == '3') {	
		ZeroMemory(message, sizeof(message));
		iResult = recv(ConnectSocket, message, sizeof(message), 0);
		if (iResult == SOCKET_ERROR) {
			printf("recv faild whit error: %d\n", WSAGetLastError());
			return;
		}
		else {
			client_obj.set_name(message);
		}
		iResult = recv(ConnectSocket, message, sizeof(message), 0);
		if (iResult == SOCKET_ERROR) {
			printf("recv faild whit error: %d\n", WSAGetLastError());
			return;
		}
		else {
			client_obj.set_surname(message);
		}
		iResult = recv(ConnectSocket, message, sizeof(message), 0);
		if (iResult == SOCKET_ERROR) {
			printf("recv faild whit error: %d\n", WSAGetLastError());
			return;
		}
		else {
			client_obj.set_email(message);
		}

		client_obj.set_sign_in_status(sign_in_status::ONLINE);
		// setting sign in status to ONLINE on successfull sign in
		std::cout << "sign in successfull" << std::endl;

		int receive_mail_size = 0;
		envelope received_envelope;
		ZeroMemory(message, sizeof(message));
		iResult = recv(ConnectSocket, message, sizeof(message), 0);
		receive_mail_size = std::atoi(message);
		// converting chars back to int

		ZeroMemory(message, sizeof(message));
		for (auto i = 0; i < receive_mail_size; i++) {

			iResult = recv(ConnectSocket, message, sizeof(message), 0);
			if (iResult == SOCKET_ERROR) {
				printf("recv faild whit error: %d\n", WSAGetLastError());
				return;
			}
			else {
				received_envelope.set_from_email(message);
			} 

			ZeroMemory(message, sizeof(message));

			iResult = recv(ConnectSocket, message, sizeof(message), 0);
			if (iResult == SOCKET_ERROR) {
				printf("recv faild whit error: %d\n", WSAGetLastError());
				return;
			}
			else {
				received_envelope.set_message(message);
			}

			ZeroMemory(message, sizeof(message));

			iResult = recv(ConnectSocket, message, sizeof(message), 0);
			if (iResult == SOCKET_ERROR) {
				printf("recv faild whit error: %d\n", WSAGetLastError());
				return;
			}
			else {
				received_envelope.set_date_time(message);
			}
			client_obj.insert_envelope(received_envelope);
			//ZeroMemory(message, sizeof(message));
		}
	}
}

void ClientManager::client_shut_down() {
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return;
	}
	else {
		std::cout << "shut down successfull" << std::endl;
	}

	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();
}



