#include "data_base_manager.h"
#include <iostream>
#include <mysql.h>
#include <string>
#include <string.h>
#include "client_obj.h"
#include <vector>
#define QUERY_LENGTH 256
char sql_query[QUERY_LENGTH];

MYSQL mysql;
MYSQL_RES* res;
MYSQL_ROW row;

void DataBaseManager::perpare_data_base_for_connection(const char* db_pass, const char* db_name) {
	mysql_init(&mysql);
	if (&mysql == nullptr) {
		std::cout << "Error: can't create MySQL-descriptor" << std::endl;
		return;
	}
	else {
		std::cout << "descriptor created successfully!" << std::endl;
	}

	if (!mysql_real_connect(&mysql, "localhost", "root", db_pass, db_name, NULL, NULL, 0)) {
		std::cout << "Error: can't connect to database " << mysql_error(&mysql) << std::endl;
		return;
	}
	else {
		std::cout << "data base connected successfully!" << std::endl;
	}
}

void DataBaseManager::prepare_data_base_for_use() {
	int query_check = 0;
	query_check = mysql_query(&mysql, "CREATE DATABASE chat_db");
	if (query_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}

	query_check = mysql_query(&mysql, "USE chat_db");
	if (query_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}

	query_check = mysql_query(&mysql, "CREATE TABLE IF NOT EXISTS client_table (id INT NOT NULL AUTO_INCREMENT PRIMARY KEY)");
	if (query_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}

	query_check = mysql_query(&mysql, "ALTER TABLE client_table ADD COLUMN name varchar(20)");
	if (query_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}

	query_check = mysql_query(&mysql, "ALTER TABLE client_table ADD COLUMN surname varchar(20)");
	if (query_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}

	query_check = mysql_query(&mysql, "ALTER TABLE client_table ADD COLUMN email varchar(20)");
	if (query_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}

	query_check = mysql_query(&mysql, "ALTER TABLE client_table ADD CONSTRAINT unique_email UNIQUE (email)");
	// adding constraint for email
	// email will be used as an address of reciver
	if (query_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}

	query_check = mysql_query(&mysql, "ALTER TABLE client_table ADD COLUMN login varchar(20)");
	if (query_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}

	query_check = mysql_query(&mysql, "ALTER TABLE client_table ADD CONSTRAINT unique_login UNIQUE (login)");
	if (query_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}
	// adding constraint for login

	query_check = mysql_query(&mysql, "CREATE TABLE IF NOT EXISTS hash_table (id INT NOT NULL AUTO_INCREMENT PRIMARY KEY)");
	if (query_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}
	// creating table for hashes
	query_check = mysql_query(&mysql, "ALTER TABLE hash_table ADD COLUMN client_id INT REFERENCES client_table (id)");
	if (query_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}
	// adding references 
	query_check = mysql_query(&mysql, "ALTER TABLE hash_table ADD COLUMN hash varchar(100)");
	if (query_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}
	// adding column for hash
	query_check = mysql_query(&mysql, "CREATE TABLE IF NOT EXISTS message_table (id INT NOT NULL AUTO_INCREMENT PRIMARY KEY)");
	if (query_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}
	// creating table for messages
	query_check = mysql_query(&mysql, "ALTER TABLE message_table ADD COLUMN sender_id INT NOT NULL REFERENCES client_table (id)");
	if (query_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}
	query_check = mysql_query(&mysql, "ALTER TABLE message_table ADD COLUMN receiver_id INT NOT NULL REFERENCES client_table (id)");
	if (query_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}
	query_check = mysql_query(&mysql, "ALTER TABLE message_table ADD COLUMN message_ varchar(50)");
	if (query_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}
	query_check = mysql_query(&mysql, "ALTER TABLE message_table ADD COLUMN status_ varchar(50) NOT NULL CHECK ( status_ IN ('delivered', 'sent', 'read'))");
	if (query_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}
	query_check = mysql_query(&mysql, "ALTER TABLE message_table ADD COLUMN time_received TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP");
	if (query_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}
}

void DataBaseManager::fill_up_data_base_whit_some_msg() {
	int query_check = 0;
	query_check = mysql_query(&mysql, "INSERT INTO message_table (sender_id, receiver_id, message_, status_) VALUES (1, 1, 'hello from Tester to Tester', 'delivered')");
	if (query_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}
	query_check = mysql_query(&mysql, "INSERT INTO message_table (sender_id, receiver_id, message_, status_) VALUES (2, 1, 'hello from Eric to Tester', 'delivered')");
	if (query_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}
	query_check = mysql_query(&mysql, "INSERT INTO message_table (sender_id, receiver_id, message_, status_) VALUES (3, 1, 'hello from Olaf to Tester', 'delivered')");
	if (query_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}
	query_check = mysql_query(&mysql, "INSERT INTO message_table (sender_id, receiver_id, message_, status_) VALUES (4, 1, 'hello from Baleog to Tester', 'delivered')");
	if (query_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}
	query_check = mysql_query(&mysql, "INSERT INTO message_table (sender_id, receiver_id, message_, status_) VALUES (1, 2, 'hello from Tester to Eric', 'delivered')");
	if (query_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}
	query_check = mysql_query(&mysql, "INSERT INTO message_table (sender_id, receiver_id, message_, status_) VALUES (2, 2, 'hello from Eric to Eric', 'delivered')");
	if (query_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}
	query_check = mysql_query(&mysql, "INSERT INTO message_table (sender_id, receiver_id, message_, status_) VALUES (3, 2, 'hello from Olaf to Eric', 'delivered')");
	if (query_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}
	query_check = mysql_query(&mysql, "INSERT INTO message_table (sender_id, receiver_id, message_, status_) VALUES (4, 2, 'hello from Baleog to Eric', 'delivered')");
	if (query_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}
	query_check = mysql_query(&mysql, "INSERT INTO message_table (sender_id, receiver_id, message_, status_) VALUES (1, 3, 'hello from Tester to Olaf', 'delivered')");
	if (query_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}
	query_check = mysql_query(&mysql, "INSERT INTO message_table (sender_id, receiver_id, message_, status_) VALUES (2, 3, 'hello from Eric to Olaf', 'delivered')");
	if (query_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}
	query_check = mysql_query(&mysql, "INSERT INTO message_table (sender_id, receiver_id, message_, status_) VALUES (3, 3, 'hello from Olaf to Olaf', 'delivered')");
	if (query_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}
	query_check = mysql_query(&mysql, "INSERT INTO message_table (sender_id, receiver_id, message_, status_) VALUES (4, 3, 'hello from Baleog to Olaf', 'delivered')");
	if (query_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}
	query_check = mysql_query(&mysql, "INSERT INTO message_table (sender_id, receiver_id, message_, status_) VALUES (1, 4, 'hello from Tester to Baleog', 'delivered')");
	if (query_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}
	query_check = mysql_query(&mysql, "INSERT INTO message_table (sender_id, receiver_id, message_, status_) VALUES (2, 4, 'hello from Eric to Baleog', 'delivered')");
	if (query_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}
	query_check = mysql_query(&mysql, "INSERT INTO message_table (sender_id, receiver_id, message_, status_) VALUES (3, 4, 'hello from Olaf to Baleog', 'delivered')");
	if (query_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}
	query_check = mysql_query(&mysql, "INSERT INTO message_table (sender_id, receiver_id, message_, status_) VALUES (4, 4, 'hello from Baleog to Baleog', 'delivered')");
	if (query_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}
}

bool DataBaseManager::insert_client_data(const std::string& input_name, const std::string& input_surname,
	const std::string& input_email, const std::string& input_login) {
	std::string string_buffer = { "INSERT INTO client_table (name, surname, email, login) VALUES ('" };
	for (auto i = 0; i < input_name.size(); i++) {
		string_buffer.push_back(input_name[i]);
	}

	std::string sql_text = { "', '" };
	for (auto i = 0; i < sql_text.size(); i++) {
		string_buffer.push_back(sql_text[i]);
	}

	for (auto i = 0; i < input_surname.size(); i++) {
		string_buffer.push_back(input_surname[i]);
	}

	for (auto i = 0; i < sql_text.size(); i++) {
		string_buffer.push_back(sql_text[i]);
	}

	for (auto i = 0; i < input_email.size(); i++) {
		string_buffer.push_back(input_email[i]);
	}

	for (auto i = 0; i < sql_text.size(); i++) {
		string_buffer.push_back(sql_text[i]);
	}

	for (auto i = 0; i < input_login.size(); i++) {
		string_buffer.push_back(input_login[i]);
	}

	sql_text = { "')" };
	for (auto i = 0; i < sql_text.size(); i++) {
		string_buffer.push_back(sql_text[i]);
	}

	ZeroMemory(sql_query, sizeof(sql_query));
	for (auto i = 0; i < string_buffer.size(); i++) {
		sql_query[i] = string_buffer[i];
	}

	int query_error_check = 0;
	query_error_check = mysql_query(&mysql, sql_query);
	if (query_error_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return false;
	}
	else {
		return true;
	}
}

bool DataBaseManager::insert_client_hash(const std::string& input_email, const std::string& input_hash) {
	std::string string_buffer = { "SELECT id FROM client_table WHERE email = ('" };
	// constructing first sql query
	for (auto i = 0; i < input_email.size(); i++) {
		string_buffer.push_back(input_email[i]);
	}
	std::string p_marks = { "')" };
	for (auto i = 0; i < p_marks.size(); i++) {
		string_buffer.push_back(p_marks[i]);
	}
	ZeroMemory(sql_query, sizeof(sql_query));
	for (auto i = 0; i < string_buffer.size(); i++) {
		sql_query[i] = string_buffer[i];
	}

	int query_error_check = 0;
	query_error_check = mysql_query(&mysql, sql_query);
	if (query_error_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return false;
	}
	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);
	if (row == nullptr) {
		return false;
	}
	std::string client_id = row[0];

	string_buffer.clear();
	string_buffer = { "INSERT INTO hash_table (client_id, hash) VALUES (" };
	// constructing second sql query
	for (auto i = 0; i < client_id.size(); i++) {
		string_buffer.push_back(client_id[i]);
	}
	p_marks = { ", '" };
	for (auto i = 0; i < p_marks.size(); i++) {
		string_buffer.push_back(p_marks[i]);
	}
	for (auto i = 0; i < input_hash.size(); i++) {
		string_buffer.push_back(input_hash[i]);
	} // writing hash to sql query buffer
	p_marks = { "')" };
	for (auto i = 0; i < p_marks.size(); i++) {
		string_buffer.push_back(p_marks[i]);
	} // closing sql query buffer 

	ZeroMemory(sql_query, sizeof(sql_query));
	for (auto i = 0; i < string_buffer.size(); i++) {
		sql_query[i] = string_buffer[i];
	} // finalizing sql query

	query_error_check = mysql_query(&mysql, sql_query);
	if (query_error_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return false;
	}
	else {
		return true;
	}
}

bool DataBaseManager::insert_message_into_db(const std::string& sender_email, const std::string& receiver_email, const std::string& input_message) {
	std::string string_buffer = { "SELECT id FROM client_table WHERE email = ('" };
	// constructing first sql query
	for (auto i = 0; i < sender_email.size(); i++) {
		string_buffer.push_back(sender_email[i]);
	}
	std::string p_marks = { "')" };
	for (auto i = 0; i < p_marks.size(); i++) {
		string_buffer.push_back(p_marks[i]);
	}
	ZeroMemory(sql_query, sizeof(sql_query));
	for (auto i = 0; i < string_buffer.size(); i++) {
		sql_query[i] = string_buffer[i];
	}

	int query_error_check = 0;
	query_error_check = mysql_query(&mysql, sql_query);
	if (query_error_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return false;
	}
	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);
	if (row == nullptr) {
		return false;
	}
	std::string sender_id = row[0];
	// storing sender id from client_table

	string_buffer = { "SELECT id FROM client_table WHERE email = ('" };
	// constructing second
	for (auto i = 0; i < receiver_email.size(); i++) {
		string_buffer.push_back(receiver_email[i]);
	}
	p_marks = { "')" };
	for (auto i = 0; i < p_marks.size(); i++) {
		string_buffer.push_back(p_marks[i]);
	}
	ZeroMemory(sql_query, sizeof(sql_query));
	for (auto i = 0; i < string_buffer.size(); i++) {
		sql_query[i] = string_buffer[i];
	}

	query_error_check = mysql_query(&mysql, sql_query);
	if (query_error_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return false;
	}
	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);
	if (row == nullptr) {
		return false;
	}
	std::string receiver_id = row[0];

	string_buffer = { "INSERT INTO message_table (sender_id, receiver_id, message_, status_ ) VALUES (" };
	// constructing final sql query for inserting message
	string_buffer.push_back(sender_id[0]);

	p_marks = { ", " };
	for (auto i = 0; i < p_marks.size(); i++) {
		string_buffer.push_back(p_marks[i]);
	}
	string_buffer.push_back(receiver_id[0]);

	p_marks = { ", '" };
	for (auto i = 0; i < p_marks.size(); i++) {
		string_buffer.push_back(p_marks[i]);
	}
	for (auto i = 0; i < input_message.size(); i++) {
		string_buffer.push_back(input_message[i]);
	} // writing input message to a string buffer
	p_marks = { "', '" };
	for (auto i = 0; i < p_marks.size(); i++) {
		string_buffer.push_back(p_marks[i]);
	}
	p_marks = { "delivered')" };
	for (auto i = 0; i < p_marks.size(); i++) {
		string_buffer.push_back(p_marks[i]);
	}

	ZeroMemory(sql_query, sizeof(sql_query));
	for (auto i = 0; i < string_buffer.size(); i++) {
		sql_query[i] = string_buffer[i];
	} // finalizing sql query to insert new message into message_table

	std::cout << sender_email << receiver_email << std::endl; 
	std::cout << "sql 1 " << sql_query << std::endl;
	query_error_check = mysql_query(&mysql, sql_query);
	if (query_error_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return false;
	}
	return true;
}

std::string DataBaseManager::get_data_from_db(const std::string& requesting_client_login, int data_type) {
	int i = 0; // used in for loops trough-out this code body
	int sql_q_error_check = 0; // used for all sql query checks in current code body
	std::string string_buffer; // used as buffer to prepare and concatenate all sql query commands
	std::string s_buf_for_sub_query; // used for sub-queries
	std::string p_marks; // used for punctuation marks and parenthesis for preparation and finalizing sql_query request
	switch (data_type) {
	case 1: // fisrt block: obtaining name data from chat_db
		string_buffer = { "SELECT name FROM client_table WHERE login = ('" };
		for (i = 0; i < requesting_client_login.size(); i++) {
			string_buffer.push_back(requesting_client_login[i]);
		} 
		p_marks = { "')" };
		for (i = 0; i < p_marks.size(); i++) {
			string_buffer.push_back(p_marks[i]);
		}
		ZeroMemory(sql_query, sizeof(sql_query));
		for (auto i = 0; i < string_buffer.size(); i++) {
			sql_query[i] = string_buffer[i];
		} // finalizing sql query
		sql_q_error_check = mysql_query(&mysql, sql_query);
		if (sql_q_error_check != 0) {
			std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
			return "error";
		}
		res = mysql_store_result(&mysql);
		row = mysql_fetch_row(res);
		if (row == nullptr) {
			return "error";
		}
		return row[0];
		break;
	case 2: // second block: obtaining surname data from chat_db
		string_buffer = { "SELECT surname FROM client_table WHERE login = ('" };
		for (i = 0; i < requesting_client_login.size(); i++) {
			string_buffer.push_back(requesting_client_login[i]);
		}
		p_marks = { "')" };
		for (i = 0; i < p_marks.size(); i++) {
			string_buffer.push_back(p_marks[i]);
		}
		ZeroMemory(sql_query, sizeof(sql_query));
		for (auto i = 0; i < string_buffer.size(); i++) {
			sql_query[i] = string_buffer[i];
		} // finalizing sql query

		sql_q_error_check = mysql_query(&mysql, sql_query);
		if (sql_q_error_check != 0) {
			std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
			return "error";
		}
		res = mysql_store_result(&mysql);
		row = mysql_fetch_row(res);
		if (row == nullptr) {
			return "error";
		}
		return row[0];
		break;
	case 3: // block 3: obtaining email data from chat_db
		string_buffer = { "SELECT email FROM client_table WHERE login = ('" };
		for (i = 0; i < requesting_client_login.size(); i++) {
			string_buffer.push_back(requesting_client_login[i]);
		}
		p_marks = { "')" };
		for (i = 0; i < p_marks.size(); i++) {
			string_buffer.push_back(p_marks[i]);
		}
		ZeroMemory(sql_query, sizeof(sql_query));
		for (auto i = 0; i < string_buffer.size(); i++) {
			sql_query[i] = string_buffer[i];
		} // finalizing sql query

		sql_q_error_check = mysql_query(&mysql, sql_query);
		if (sql_q_error_check != 0) {
			std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
			return "error";
		}
		res = mysql_store_result(&mysql);
		row = mysql_fetch_row(res);
		if (row == nullptr) {
			return "error";
		}
		return row[0];

		break;
	case 4: 
		// block 4: obtaining sender email from chat_db
		// sender email will be used as refernce from who the message was send
	    // server later will send this data to client, so requesting client will know from who the message was sent!
		s_buf_for_sub_query = { "SELECT email FROM client_table WHERE id = (" };
		for (i = 0; i < s_buf_for_sub_query.size(); i++) {
			string_buffer.push_back(s_buf_for_sub_query[i]);
		}
		s_buf_for_sub_query = { "SELECT DISTINCT sender_id FROM message_table WHERE receiver_id = (" };
		for (i = 0; i < s_buf_for_sub_query.size(); i++) {
			string_buffer.push_back(s_buf_for_sub_query[i]);
		}
		s_buf_for_sub_query = { "SELECT id FROM client_table WHERE login = ('" };
		for (i = 0; i < s_buf_for_sub_query.size(); i++) {
			string_buffer.push_back(s_buf_for_sub_query[i]);
		}
		for (i = 0; i < requesting_client_login.size(); i++) {
			string_buffer.push_back(requesting_client_login[i]);
		}
		p_marks = { "')))" };
		for (i = 0; i < p_marks.size(); i++) {
			string_buffer.push_back(p_marks[i]);
		}
		
		ZeroMemory(sql_query, sizeof(sql_query));
		for (auto i = 0; i < string_buffer.size(); i++) {
			sql_query[i] = string_buffer[i];
		} // finalizing sql query
		sql_q_error_check = mysql_query(&mysql, sql_query);
		if (sql_q_error_check != 0) {
			std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
			return "error";
		}
		res = mysql_store_result(&mysql);
        row = mysql_fetch_row(res);
        if (row == nullptr) {
	        return "error";
        }
		return row[0];
		break;
	case 5:
		string_buffer = 
		{ "SELECT time_received FROM message_table WHERE receiver_id = (SELECT id FROM client_table WHERE login = ('" };
		for (i = 0; i < requesting_client_login.size(); i++) {
			string_buffer.push_back(requesting_client_login[i]);
		}
		p_marks = { "'))" };
		for (i = 0; i < p_marks.size(); i++) {
			string_buffer.push_back(p_marks[i]);
		}
		ZeroMemory(sql_query, sizeof(sql_query));
		for (auto i = 0; i < string_buffer.size(); i++) {
			sql_query[i] = string_buffer[i];
		} // finalizing sql query
		sql_q_error_check = mysql_query(&mysql, sql_query);
		if (sql_q_error_check != 0) {
			std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
			return "error";
		}
		res = mysql_store_result(&mysql);
		row = mysql_fetch_row(res);
		if (row == nullptr) {
			return "error";
		}
		return row[0];
		break;
	case 6:
		string_buffer =
		{ "SELECT message_ FROM message_table WHERE receiver_id = (SELECT id FROM client_table WHERE login = ('" };
		for (i = 0; i < requesting_client_login.size(); i++) {
			string_buffer.push_back(requesting_client_login[i]);
		}
		p_marks = { "'))" };
		for (i = 0; i < p_marks.size(); i++) {
			string_buffer.push_back(p_marks[i]);
		}
		ZeroMemory(sql_query, sizeof(sql_query));
		for (auto i = 0; i < string_buffer.size(); i++) {
			sql_query[i] = string_buffer[i];
		} // finalizing sql query
		sql_q_error_check = mysql_query(&mysql, sql_query);
		if (sql_q_error_check != 0) {
			std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
			return "error";
		}
		res = mysql_store_result(&mysql);
		row = mysql_fetch_row(res);
		if (row == nullptr) {
			return "error";
		}
		return row[0];
		break;
	case 7:
		string_buffer =
		{ "SELECT id FROM client_table WHERE login = ('" };
		for (i = 0; i < requesting_client_login.size(); i++) {
			string_buffer.push_back(requesting_client_login[i]);
		}
		p_marks = { "')" };
		for (i = 0; i < p_marks.size(); i++) {
			string_buffer.push_back(p_marks[i]);
		}
		ZeroMemory(sql_query, sizeof(sql_query));
		for (auto i = 0; i < string_buffer.size(); i++) {
			sql_query[i] = string_buffer[i];
		} // finalizing sql query
		sql_q_error_check = mysql_query(&mysql, sql_query);
		if (sql_q_error_check != 0) {
			std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
			return "error";
		}
		res = mysql_store_result(&mysql);
		row = mysql_fetch_row(res);
		if (row == nullptr) {
			return "error";
		}
		return row[0];
		break;
	}	
}

void DataBaseManager::extract_all_receiver_msgs_from_db(const std::string& r_login) {
	int i = 0;
	std::vector<struct envelope> client_mail;
	std::string p_marks;
	std::string sql_buf;
	std::string sql_query_text =
	{ "SELECT email, message_, time_received FROM client_table JOIN message_table ON client_table.id = message_table.sender_id " };
	for (; i < sql_query_text.size(); i++) {
		sql_buf.push_back(sql_query_text[i]);
	}
	sql_query_text =
	{ "WHERE message_table.receiver_id IN (SELECT id FROM client_table WHERE login = ('" };
	for (i = 0; i < sql_query_text.size(); i++) {
		sql_buf.push_back(sql_query_text[i]);
	}
	for (i = 0; i < r_login.size(); i++) {
		sql_buf.push_back(r_login[i]);
	}
	p_marks = { "'))" };
	for (i = 0; i < p_marks.size(); i++) {
		sql_buf.push_back(p_marks[i]);
	}
	ZeroMemory(sql_query, sizeof(sql_query));
	for (i = 0; i < sql_buf.size(); i++) {
		sql_query[i] = sql_buf[i];
	}

	int error_check = 0;
	error_check = mysql_query(&mysql, sql_query);
	if (error_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}
	if (res = mysql_store_result(&mysql)) {
		while (row = mysql_fetch_row(res)) {
			for (i = 0; i < mysql_num_fields(res); i++) {
				envelope new_envelope(row[i], row[i + 1], row[i + 2]);
				client_mail.push_back(new_envelope);
				break;
			}
		}
	}
	else {
		std::cout << "ERROR mysql number : " << mysql_error(&mysql);
	}
	DataBaseManager::set_receiver_mail_obj(client_mail);
}

void DataBaseManager::extract_only_delivered_msgs_from_db(const std::string& requesting_login) {
	int i = 0;
	std::vector<struct envelope> client_mail;
	std::string p_marks;
	std::string sql_buf;
	std::string sql_query_text =
	{ "SELECT email, message_, time_received FROM client_table JOIN message_table ON client_table.id = message_table.sender_id " };
	for (; i < sql_query_text.size(); i++) {
		sql_buf.push_back(sql_query_text[i]);
	}
	sql_query_text =
	{ "WHERE message_table.status_ = ('delivered') AND message_table.receiver_id IN (SELECT id FROM client_table WHERE login = ('" };
	for (i = 0; i < sql_query_text.size(); i++) {
		sql_buf.push_back(sql_query_text[i]);
	}
	for (i = 0; i < requesting_login.size(); i++) {
		sql_buf.push_back(requesting_login[i]);
	}
	p_marks = { "'))" };
	for (i = 0; i < p_marks.size(); i++) {
		sql_buf.push_back(p_marks[i]);
	}
	ZeroMemory(sql_query, sizeof(sql_query));
	for (i = 0; i < sql_buf.size(); i++) {
		sql_query[i] = sql_buf[i];
	}

	int error_check = 0;
	error_check = mysql_query(&mysql, sql_query);
	if (error_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}
	if (res = mysql_store_result(&mysql)) {
		while (row = mysql_fetch_row(res)) {
			for (i = 0; i < mysql_num_fields(res); i++) {
				envelope new_envelope(row[i], row[i + 1], row[i + 2]);
				client_mail.push_back(new_envelope);
				break;
			}
		}
	}
	else {
		std::cout << "ERROR mysql number : " << mysql_error(&mysql);
	}
	DataBaseManager::set_receiver_mail_obj(client_mail);
}

void DataBaseManager::set_message_flags_to_read(const std::string& requesting_login) {
	int i = 0;
	int error_check = 0;
	std::string p_marks;
	std::string sql_buf;
	std::string sql_query_text;
	sql_query_text = {
	"UPDATE message_table SET status_ = ('read') WHERE status_ = ('sent') AND receiver_id IN (SELECT id FROM client_table WHERE login = ('"
	};
	for (; i < sql_query_text.size(); i++) {
		sql_buf.push_back(sql_query_text[i]);
	}
	for (i = 0; i < requesting_login.size(); i++) {
		sql_buf.push_back(requesting_login[i]);
	}
	p_marks = { "'))" };
	for (i = 0; i < p_marks.size(); i++) {
		sql_buf.push_back(p_marks[i]);
	}
	ZeroMemory(sql_query, sizeof(sql_query));
	for (i = 0; i < sql_buf.size(); i++) {
		sql_query[i] = sql_buf[i];
	}
	error_check = mysql_query(&mysql, sql_query);
	if (error_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}
	// SELECT email, status_
    // FROM client_table
    // JOIN message_table
    // ON client_table.id = message_table.sender_id
	// WHERE message_table.receiver_id IN(
	  // SELECT id
	  // FROM client_table
	  // WHERE login = ('a') AND message_table.status_ = ('delivered'));
}

void DataBaseManager::set_message_flags_to_sent(const std::string& requesting_login) {
	int i = 0;
	int error_check = 0;
	std::string p_marks;
	std::string sql_buf;
	std::string sql_query_text;
	sql_query_text = {
	"UPDATE message_table SET status_ = ('sent') WHERE status_ NOT IN ('read', 'sent') AND receiver_id IN (SELECT id FROM client_table WHERE login = ('"
	};
	for (; i < sql_query_text.size(); i++) {
		sql_buf.push_back(sql_query_text[i]);
	}
	for (i = 0; i < requesting_login.size(); i++) {
		sql_buf.push_back(requesting_login[i]);
	}
	p_marks = { "'))" };
	for (i = 0; i < p_marks.size(); i++) {
		sql_buf.push_back(p_marks[i]);
	}
	ZeroMemory(sql_query, sizeof(sql_query));
	for (i = 0; i < sql_buf.size(); i++) {
		sql_query[i] = sql_buf[i];
	}
	error_check = mysql_query(&mysql, sql_query);
	if (error_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}
}

void DataBaseManager::extract_not_read_msgs_from_db(const std::string& requesting_login) {
	int i = 0;
	std::vector<struct envelope> client_mail;
	std::string p_marks;
	std::string sql_buf;
	std::string sql_query_text =
	{ "SELECT email, message_, time_received FROM client_table JOIN message_table ON client_table.id = message_table.sender_id " };
	for (; i < sql_query_text.size(); i++) {
		sql_buf.push_back(sql_query_text[i]);
	}
	sql_query_text =
	{ "WHERE message_table.status_ != ('read') AND message_table.receiver_id IN (SELECT id FROM client_table WHERE login = ('" };
	for (i = 0; i < sql_query_text.size(); i++) {
		sql_buf.push_back(sql_query_text[i]);
	}
	for (i = 0; i < requesting_login.size(); i++) {
		sql_buf.push_back(requesting_login[i]);
	}
	p_marks = { "'))" };
	for (i = 0; i < p_marks.size(); i++) {
		sql_buf.push_back(p_marks[i]);
	}
	ZeroMemory(sql_query, sizeof(sql_query));
	for (i = 0; i < sql_buf.size(); i++) {
		sql_query[i] = sql_buf[i];
	}

	int error_check = 0;
	error_check = mysql_query(&mysql, sql_query);
	if (error_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return;
	}
	if (res = mysql_store_result(&mysql)) {
		while (row = mysql_fetch_row(res)) {
			for (i = 0; i < mysql_num_fields(res); i++) {
				envelope new_envelope(row[i], row[i + 1], row[i + 2]);
				client_mail.push_back(new_envelope);
				break;
			}
		}
	}
	else {
		std::cout << "ERROR mysql number : " << mysql_error(&mysql);
	}
	DataBaseManager::set_receiver_mail_obj(client_mail);

}

void DataBaseManager::set_receiver_mail_obj(const std::vector<struct envelope>& input_mail_obj) {
	client_mail = input_mail_obj;
}

std::vector<struct envelope>& DataBaseManager::get_reciver_mail_obj() {
	return client_mail;
}

void DataBaseManager::clean_client_mail() {
	client_mail.clear();
}

bool DataBaseManager::check_email(const std::string& input_email) {
	std::string sql_text = { "SELECT email FROM client_table WHERE email = ('" };
	for (auto i = 0; i < input_email.size(); i++) {
		sql_text.push_back(input_email[i]);
	}
	std::string closing = { "')" };
	for (auto i = 0; i < closing.size(); i++) {
		sql_text.push_back(closing[i]);
	}

	ZeroMemory(sql_query, sizeof(sql_query));

	for (auto i = 0; i < sql_text.size(); i++) {
		sql_query[i] = sql_text[i];
	}

	int query_error_check = 0;
	query_error_check = mysql_query(&mysql, sql_query);
	if (query_error_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
	}
	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);

	if (row == nullptr) {
		return false;
	}
	if (row[0] == input_email) {
		return true;
	}
}

bool DataBaseManager::check_login(const std::string& input_login) {
	std::string sql_text = { "SELECT login FROM client_table WHERE login = ('" };
	for (auto i = 0; i < input_login.size(); i++) {
		sql_text.push_back(input_login[i]);
	}
	std::string closing = { "')" };
	for (auto i = 0; i < closing.size(); i++) {
		sql_text.push_back(closing[i]);
	}
	ZeroMemory(sql_query, sizeof(sql_query));

	for (auto i = 0; i < sql_text.size(); i++) {
		sql_query[i] = sql_text[i];
	}
	int query_error_check = 0;
	query_error_check = mysql_query(&mysql, sql_query);
	if (query_error_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
	}

	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);
	if (row == nullptr) {
		return false;
	}
	else {
		return true;
	}
	if (row[0] == input_login) {
		return false;
	}
}

bool DataBaseManager::check_client_hash(const std::string& input_login, const std::string& input_hash) {
	int i = 0; // used in for loops trough-out this code body
	std::string string_buffer
	{ "SELECT hash FROM hash_table WHERE client_id IN (SELECT id FROM client_table WHERE login = ('" };

	for (i = 0; i < input_login.size(); i++) {
		string_buffer.push_back(input_login[i]);
	}
	std::string p_marks{ "'))" };
	for (i = 0; i < p_marks.size(); i++) {
		string_buffer.push_back(p_marks[i]);
	}
	ZeroMemory(sql_query, sizeof(sql_query));
	for (i = 0; i < string_buffer.size(); i++) {
		sql_query[i] = string_buffer[i];
	}
	int query_error_check = 0;
	query_error_check = mysql_query(&mysql, sql_query);
	if (query_error_check != 0) {
		std::cout << "mqsql query faild whit error: " << mysql_error(&mysql) << std::endl;
		return false;
	}
	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);
	if (row == nullptr) {
		return false;
	}
	std::string client_hash = row[0];
	// obtaining client hash from data base and comparing to input_hash
	if (client_hash == input_hash) {
		return true;
	}
	else {
		return false;
	}
}

void DataBaseManager::desconnect_data_base() {
	std::cout << "data base is desconnected" << std::endl;
	mysql_close(&mysql);
}
