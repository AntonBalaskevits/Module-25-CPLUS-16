#pragma once
#include <string>
#include "client_obj.h"

struct ClientManager {
private:
	Client client_obj_;
public:
	ClientManager() = default;
	int prepare_client_for_connection(const char* pc_name);

	void send_client_data(const std::string& input_data);
	void send_flag(const char& input_flag);
	int receive_flag();

	void when_client_signed_in(Client& client_obj);
	int get_incoming_mail_box_size();
	void check_for_incoming_mail(Client& client_obj, int receive_mail_size);

	void client_registration(Client& clinet_obj);

	void client_sign_in(Client& client_obj);

	void client_shut_down();
};


