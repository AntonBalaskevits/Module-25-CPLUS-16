#pragma once
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
};

enum sign_in_status { OFFLINE, ONLINE };

class Client {
	std::string name_;
	std::string surname_;
	std::string email_;
	std::string receiver_email_;
	std::string login_;
	std::string message_;
	std::string hash_;
	std::vector<struct envelope> mail_box_;
	sign_in_status sign_in_status_ = OFFLINE;
public:
	Client() = default;
	~Client() {};

	void set_name(const std::string& input_name);
	std::string& get_name();

	void set_surname(const std::string& input_surname);
	std::string& get_surname();

	void set_email(const std::string& input_email);
	const std::string& get_email();

	void set_receiver_email(const std::string& receiver_email);
	std::string& get_receiver_email();
	void clean_receiver_email();

	void set_login(const std::string& input_login);
	std::string& get_login();

	void set_message(const std::string& input_text);
	const std::string& get_message();

	void set_hash(const std::string& input_hash);
	std::string& get_hash();

	void set_sign_in_status(sign_in_status status);
	enum sign_in_status& get_status();

	void set_envelope(struct envelope& input_envelope);

	void set_mail_box(const std::vector<struct envelope>& mail_obj);
	const std::vector<struct envelope>& get_mail_box();
	void clean_mail_box();

	int get_mail_box_size();

	void client_clean_up();
};