#pragma once
#include <string>
#include "client_obj.h"
#include "data_base_manager.h"

struct ServerManager {

	ServerManager() = default;
	int prepare_server_and_data_base_for_connection(DataBaseManager& db_manager_obj, const char* db_pass, const char* db_name);

	void send_client_mail(DataBaseManager& db_manager_obj, Client& client_obj);

	void client_registration(DataBaseManager& db_manager_obj, Client& client_obj);
	void auto_register(DataBaseManager& db_manager_obj);
	// the auto register will fill up chat data base whit some random users

	void client_sign_in(DataBaseManager& db_manager_obj, Client& client_obj);
	void when_client_signed_in(DataBaseManager& db_manager_obj, Client& client_obj);

	void send_flag(const char& input_flag);
	int receive_flag();

	void shut_down(DataBaseManager& db_manager_obj);
};
