#include "client_manager.h"
#include <iostream>
#include <string>

int main() {

	Client client_obj;
	ClientManager client_manager;
	char pc_name[50];
	std::cout << "please type in your computer name : ";
	std::cin >> pc_name;

	std::cout << "Welcome to chat 4.0" << std::endl;
	std::cout << "client is establishing connection to a server, please wait!" << std::endl;

	client_manager.prepare_client_for_connection(pc_name); 
	//                       name       surname    email          login password
	std::string data[5] = { "Testerov", "Tester", "test@test.ru", "t", "t" };
	std::string data2[5] = { "Viking", "Eric", "Eric@north.se", "e", "e" };
	std::string data3[5] = { "Viking", "Olaf", "Olaf@north.se", "o", "o" };
	std::string data4[5] = { "Viking", "Baleog", "Baleog@north.se", "b", "b" };
	// auto registering on start up
	client_manager.send_client_data(data[0]);
	client_manager.send_client_data(data[1]);
	client_manager.send_client_data(data[2]);
	client_manager.send_client_data(data[3]);
	client_obj.set_hash(data[4]);
	client_manager.send_client_data(client_obj.get_hash());
	// auto register Tester
	client_manager.send_client_data(data2[0]);
	client_manager.send_client_data(data2[1]);
	client_manager.send_client_data(data2[2]);
	client_manager.send_client_data(data2[3]);
	client_obj.set_hash(data2[4]);
	client_manager.send_client_data(client_obj.get_hash());
	// auto register Eric
	client_manager.send_client_data(data3[0]);
	client_manager.send_client_data(data3[1]);
	client_manager.send_client_data(data3[2]);
	client_manager.send_client_data(data3[3]);
	client_obj.set_hash(data3[4]);
	client_manager.send_client_data(client_obj.get_hash());
	// auto register Olaf
	client_manager.send_client_data(data4[0]);
	client_manager.send_client_data(data4[1]);
	client_manager.send_client_data(data4[2]);
	client_manager.send_client_data(data4[3]);
	client_obj.set_hash(data4[4]);
	client_manager.send_client_data(client_obj.get_hash());
	client_obj.client_clean_up();
	// auto register Baleog
	int input = 0;
	bool on = true;
	while (on) {
		if (client_obj.get_status() == sign_in_status::ONLINE) {
			client_manager.when_client_signed_in(client_obj);
		}
		std::cout << "\n";
		std::cout << "to register 1" << std::endl;
		std::cout << "to sign in  2" << std::endl;
		std::cout << "to quit     3" << std::endl;
		std::cout << "input : ";
		std::cin >> input;

		switch (input) {
		case 1:
			client_manager.send_flag('1');
			client_manager.client_registration(client_obj);
			break;
		case 2:
			client_manager.send_flag('2');
			client_manager.client_sign_in(client_obj);
			break;
		case 3:
			client_manager.send_flag('3');
			client_manager.client_shut_down();
			on = false;
			break;
		default:
			std::cout << "bad input, please try again" << std::endl;
			break;
		}
	}
	std::cout << "bye, see u next time:)" << std::endl;

	return 0;
}