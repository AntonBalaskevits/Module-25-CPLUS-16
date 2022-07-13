#include <iostream>
#include "server_manager.h"
#include "data_base_manager.h"
#include <string>
#include <windows.h>
#include <stdio.h>
#include "client_obj.h"

int main() {
	const char* db_pass = { "root" };
	const char* db_name = { "testdb" };

	Client client_obj;
	DataBaseManager db_manager_obj;
	ServerManager server_obj;
	server_obj.prepare_server_and_data_base_for_connection(db_manager_obj, db_pass, db_name);
	db_manager_obj.prepare_data_base_for_use();

	server_obj.auto_register(db_manager_obj);
	server_obj.auto_register(db_manager_obj);
	server_obj.auto_register(db_manager_obj);
	server_obj.auto_register(db_manager_obj);
	// client program will send some data for registration on successfull connection
	db_manager_obj.fill_up_data_base_whit_some_msg();
	// filling data base whit some test messages

	int flag = 0;
	bool on = true;
	while (on) {
		if (client_obj.get_status() == sign_in_status::ONLINE) {
			server_obj.when_client_signed_in(db_manager_obj, client_obj);
		}
		flag = server_obj.receive_flag();
		// this is where server receives request from client
		// flag '1' registration, flag '2' sign in, flag '3' quit and shut down
		if (flag == -1) {
			std::cout << "main flag recv faild" << std::endl;
			std::cout << "main loop restarted" << std::endl;
			continue;
		}
		switch (flag) {
		case 1:
			server_obj.client_registration(db_manager_obj, client_obj);
			break;
		case 2:
			server_obj.client_sign_in(db_manager_obj, client_obj);
			break;
		case 3:
			server_obj.shut_down(db_manager_obj);
			on = false;
			break;
		}
	}
	std::cout << "server shut down" << std::endl;

	return 0;
}
