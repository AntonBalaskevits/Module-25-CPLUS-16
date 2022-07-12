#pragma once
#include "sha1.h"
#include <string>
#include <vector>

struct envelope {
	std::string message_;
	std::string to_email_;
	std::string from_email_;
	std::string date_time_;
	envelope() = default;
	envelope(const std::string& sender_email, const std::string& message, const std::string& date_time)
		: from_email_(sender_email), message_(message), date_time_(date_time) {};

	~envelope() {};

	void set_message(const std::string& input_message);
	const std::string& get_message();

	void set_to_email(const std::string& input_receipient_email);
	const std::string& get_to_email();

	void set_from_email(const std::string& input_sender_email);
	const std::string& get_from_email();

	void set_date_time(const std::string& input_date_time);
	const std::string& get_date_time();

	void display_envelope(const int& message_number);
};

enum sign_in_status { OFFLINE, ONLINE };

class Client {
	std::string name_;
	std::string surname_;
	std::string email_;
	std::string login_;
	std::string hash_;
	std::vector<struct envelope> mail_box_;
	sign_in_status sign_in_status_ = OFFLINE;
	// sign in status is set to default OFFLINE
public:
	Client() = default;
	~Client() {};
	void set_name(const std::string& input_name);
	std::string& get_name();

	void set_surname(const std::string& input_surname);
	std::string& get_surname();

	void set_email(const std::string& input_email);
	std::string& get_email();

	void set_login(const std::string& input_login);
	std::string& get_login();

	void set_hash(const std::string& input_password);
	std::string& get_hash();

	void set_sign_in_status(sign_in_status status);
	enum sign_in_status& get_status();

	void insert_envelope(struct envelope& input_envelope);
	int get_mail_box_size();

	const Client& get_client_obj();

	void client_clean_up();
	void clean_mail_box();

	void display_all_envelopes();

	struct envelope& get_last_received_envelope();
};
