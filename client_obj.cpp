#include "client_obj.h"
#include <string>
#include "sha1.h"
#include <iostream>
void envelope::set_message(const std::string& input_message) {
	message_ = input_message;
}
const std::string& envelope::get_message() {
	return message_;
}

void envelope::set_to_email(const std::string& input_receipient_email) {
	to_email_ = input_receipient_email;
}
const std::string& envelope::get_to_email() {
	return to_email_;
}

void envelope::set_from_email(const std::string& input_sender_email) {
	from_email_ = input_sender_email;
}
const std::string& envelope::get_from_email() {
	return from_email_;
}

void envelope::set_date_time(const std::string& input_date_time) {
	date_time_ = input_date_time;
}
const std::string& envelope::get_date_time() {
	return date_time_;
}

void envelope::display_envelope(const int& message_number) {
	std::cout << "\n";
	std::cout << "message number : " << message_number + 1 << std::endl;
	std::cout << "message        : " << message_ << std::endl;
	std::cout << "from           : " << from_email_ << std::endl;
	std::cout << "received on    : " << date_time_ << std::endl;
}



void Client::set_name(const std::string& input_name) {
	name_.clear();
	// cleaning container before setting new name
	name_ = input_name;
}
std::string& Client::get_name() {
	return name_;
}

void Client::set_surname(const std::string& input_surname) {
	surname_.clear();
	surname_ = input_surname;
}
std::string& Client::get_surname() {
	return surname_;
}

void Client::set_email(const std::string& input_email) {
	email_.clear();
	email_ = input_email;
}
std::string& Client::get_email() {
	return email_;
}

void Client::set_login(const std::string& input_login) {
	login_.clear();
	login_ = input_login;
}
std::string& Client::get_login() {
	return login_;
}

void Client::set_hash(const std::string& input_password) {
	// copy of chars array from string
	char* passwordCopyFromString = new char[input_password.size() + 1];
	for (int i = 0; i < input_password.size(); i++)
	{
		// copying chars from string
		passwordCopyFromString[i] = input_password[i]; 
	}
	// obtaining hash 
	// and setting hash
	uint* tmp_int_hash = nullptr;
	tmp_int_hash = sha1(passwordCopyFromString, sizeof(input_password.size()) - 1);
	delete[] passwordCopyFromString;
	// cleaning up
	std::string tmp_string_hash[5];
	for (auto i = 0; i < 5; i++) {
		tmp_string_hash[i] = std::to_string(tmp_int_hash[i]);
		// converting from ints to chars
	}
	delete[] tmp_int_hash;
	//cleaning up 
	hash_.clear();
	for (auto i = 0; i < 5; i++) {
		for (auto j = 0; j < tmp_string_hash[i].size(); j++) {
			hash_.push_back(tmp_string_hash[i].at(j));
			// copying from string array in to single string
			// at this point hash will be prepared in a single string buffer
			// for sending it to a server
		}
	}
}
std::string& Client::get_hash() {
	return hash_;
}

void Client::set_sign_in_status(sign_in_status status) {
	sign_in_status_ = status;
}
enum sign_in_status& Client::get_status() {
	return sign_in_status_;
}

void Client::insert_envelope(struct envelope& input_envelope) {
	mail_box_.push_back(input_envelope);
}

int Client::get_mail_box_size() {
	return mail_box_.size();
}

const Client& Client::get_client_obj() {
	return *this;
}

void Client::client_clean_up() {
	name_.clear();
	surname_.clear();
	email_.clear();
	login_.clear();
	hash_.clear();
}

void Client::clean_mail_box() {
	mail_box_.clear();
}

void Client::display_all_envelopes() {
	for (auto i = 0; i < mail_box_.size(); i++) {
		mail_box_[i].display_envelope(i);
	}
}

struct envelope& Client::get_last_received_envelope() {
	int last_envelope_index = 0;
	last_envelope_index = mail_box_.size();
	return mail_box_[last_envelope_index];
}