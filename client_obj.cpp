#include "client_obj.h"
#include <string>
#include <iostream>
#include <vector>

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

void Client::set_name(const std::string& input_name) {
	name_ = input_name;
}
std::string& Client::get_name() {
	return name_;
}

void Client::set_surname(const std::string& input_surname) {
	surname_ = input_surname;
}
std::string& Client::get_surname() {
	return surname_;
}

void Client::set_email(const std::string& input_email) {
	email_ = input_email;
}
const std::string& Client::get_email() {
	return email_;
}

void Client::set_receiver_email(const std::string& receiver_email) {
	receiver_email_ = receiver_email;
}

void Client::clean_receiver_email() {
	receiver_email_.clear();
}

std::string& Client::get_receiver_email() {
	return receiver_email_;
}

void Client::set_login(const std::string& input_login) {
	login_ = input_login;
}
std::string& Client::get_login() {
	return login_;
}

void Client::set_message(const std::string& input_text) {
	message_.clear();
	for (auto i = 0; i < sizeof(input_text); i++) {
		message_.push_back(input_text[i]);
	}
}
const std::string& Client::get_message() {
	return message_;
}

void Client::set_hash(const std::string& input_password) {
	hash_ = input_password;
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

void Client::set_envelope(struct envelope& input_envelope) {
	mail_box_.push_back(input_envelope);
}

void Client::set_mail_box(const std::vector<struct envelope>& mail_obj) {
	mail_box_ = mail_obj;
}

const std::vector<struct envelope>& Client::get_mail_box() {	
	return mail_box_;
}

int Client::get_mail_box_size() {
	return mail_box_.size();
}

void Client::clean_mail_box() {
	mail_box_.clear();
}

void Client::client_clean_up() {
	name_.clear();
	surname_.clear();
	email_.clear();
	login_.clear();
	message_.clear();
	hash_.clear();
	mail_box_.clear();
}
