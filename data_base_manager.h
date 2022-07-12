#pragma once
#include <string>
#include <vector>
#include "client_obj.h"


struct DataBaseManager {
private:
	std::vector<struct envelope> client_mail;
	// client_mail used as temporar storage container
	// for envelopes that was extracted from chats data base
	std::string client_name_;
	std::string client_surname_;
	std::string client_email_;
	// this 3 strings are used to build the envelope
public:
	DataBaseManager() = default;
	void perpare_data_base_for_connection(const char* db_pass, const char* db_name);
	// this function is used to create connection whit data base 

	void prepare_data_base_for_use();
	// this function runs a commands to create data base and tables
	void fill_up_data_base_whit_some_msg();

	bool insert_client_data(const std::string& input_name, const std::string& input_surname, 
		const std::string& input_email, const std::string& input_login);
	bool insert_client_hash(const std::string& input_email, const std::string& input_hash);
	// inserted hash will be linked to input email 
	bool insert_message_into_db(const std::string& sender_email, const std::string& receiver_email, const std::string& input_message);
	// inserts message into data base using sender_email, receiver_email and input_message from them sql query is constructed

	std::string get_data_from_db(const std::string& requesting_client_login, int data_type);
	// obtaining client data from data base using preset sql_queries
	// client data types (name of the receiver, surname of the receiver, email of the receiver,
	// email of the sender, message time_date stamp, and first message recived

	void extract_all_receiver_msgs_from_db(const std::string& r_login);
	// on a first client sign in it will extract all messages where flag is set to delivered

	void extract_only_delivered_msgs_from_db(const std::string& requesting_login);
	// extracts message where flag was set to delivered

	void extract_not_read_msgs_from_db(const std::string& requesting_login);
	// testing second version
	void set_message_flags_to_read(const std::string& requesting_login);
	void set_message_flags_to_sent(const std::string& requesting_login);

	void set_receiver_mail_obj(const std::vector<struct envelope>& input_mail_obj);
	std::vector<struct envelope>& get_reciver_mail_obj();
	void clean_client_mail();


	bool check_email(const std::string& input_email);
	bool check_login(const std::string& input_login);
	// this two methods are used to check if data base already have email and login
	// if database will have email or login function will return true
	// which will mean that email or login is already taken
	bool check_client_hash(const std::string& input_login, const std::string& input_hash);
	// this function will check if hash from data base matches the one that was received from client 
	// during sign in process. if hash will match data base hash function will return true
	void desconnect_data_base();
};