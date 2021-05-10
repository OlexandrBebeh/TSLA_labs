#pragma once
#include <iostream>
#include <vector>
#include <regex>
#include <string>
#include <fstream>

#define BUFFER_SIZE 512

enum class TokenType{
    id, 
    oper,
	if_t,
	then,
	else_t,
	number,
	relop,
	colon,
	coma,
	appropriation,
	semicolon,
	var,
	begin,
	end,
	data_type,
	open_bracket,
	close_bracket
};
const char* printType(TokenType t) {
	if (t == TokenType::id)
		return "id";
	if (t == TokenType::oper)
		return "oper";
	if (t == TokenType::if_t)
		return "if";
	if (t == TokenType::then)
		return "then";
	if (t == TokenType::else_t)
		return "else";
	if (t == TokenType::number)
		return "number";
	if (t == TokenType::relop)
		return "relop";
	if (t == TokenType::coma)
		return "coma";
	if (t == TokenType::colon)
		return "colon";
	if (t == TokenType::appropriation)
		return "appropriation";
	if (t == TokenType::semicolon)
		return "semicolon";
	if (t == TokenType::var)
		return "var";
	if (t == TokenType::begin)
		return "begin";
	if (t == TokenType::end)
		return "end";
	if (t == TokenType::data_type)
		return "data_type";
	if (t == TokenType::open_bracket)
		return "open_bracket";
	if (t == TokenType::close_bracket)
		return "close_bracket";

}

class Token {
public:
	std::string name;
	TokenType type;
	int value_ref;
	int row, symb;
	Token(std::string name, TokenType type, int value, int row, int symb) :
		name(name), type(type), value_ref(value), row(row), symb(symb) {};

	Token() {};
};

class VarTable {
public:
	std::vector<std::string> refs;

	std::string getByIndex(int i) {
		return refs[i];
	}
	int contains(std::string str) {
		for (int i = 0; i < refs.size(); i++) {
			if (refs[i] == str) {
				return i;
			}
		}
		refs.push_back(str);
		return refs.size() - 1;
	}
	void print() {
		std::cout <<  std::endl;
		for (int i = 0; i < refs.size(); i++) {
			std::cout << i << " -> " + refs[i] << std::endl;
		}
	}
};
class LexicAnalys {
	std::vector<Token> tokens;
	VarTable tab;

public:
	void getToken(std::string line, int row, int& index) { 
		std::regex int_reg("^[0-9]+");
		std::regex name_reg("^[a-zA-Z][a-zA-Z0-9]*");
		std::regex oper_reg("^(\\+|-|\\*)");
		std::regex relop_reg("^((==)|(!=)|<)");
		std::smatch m;

		if (line[0] == ')') {
			tokens.push_back(Token(")", TokenType::close_bracket, -1, row, index));
			index++;
		}
		else
		if (line[0] == '(') {
			tokens.push_back(Token("(", TokenType::open_bracket, -1, row, index));
			index++;
		} else
		if (line[0] == ',') {
			tokens.push_back(Token(",", TokenType::coma, -1, row, index));
			index++;
		} else
		if (line[0] == ';') {
			tokens.push_back(Token(";", TokenType::semicolon, -1, row, index));
			index++;
		}
		else if (line[0] == ':' && line[1] == '=') {
			tokens.push_back(Token(":=", TokenType::appropriation, -1, row, index));
			index += 2;
		}
		else if (line[0] == ':') {
			tokens.push_back(Token(":", TokenType::colon, -1, row, index));
			index ++;
		}
		else if (line.substr(0, 3) == "var") {
			tokens.push_back(Token("var", TokenType::var, -1, row, index));
			index += 3;
		}
		else if (line.substr(0, 5) == "begin") {
			tokens.push_back(Token("begin", TokenType::begin, -1, row, index));
			index += 5;
		}
		else if (line.substr(0, 3) == "end") {
			tokens.push_back(Token("end", TokenType::end, -1, row, index));
			index += 3;
		}
		else if (line.substr(0, 2) == "if") {
			tokens.push_back(Token("if", TokenType::if_t, -1, row, index));
			index += 2;
		}
		else if (line.substr(0, 4) == "then") {
			tokens.push_back(Token("then", TokenType::then, -1, row, index));
			index += 4;
		}
		else if (line.substr(0, 4) == "else") {
			tokens.push_back(Token("else", TokenType::else_t, -1, row, index));
			index += 4;
		}
		else if (line.substr(0, 7) == "integer") {
			tokens.push_back(Token("integer", TokenType::data_type, -1, row, index));
			index += 7;
		}
		else if (std::regex_search(line, m,int_reg)) {
			std::string s = m.str();
			tokens.push_back(Token(s, TokenType::number, tab.contains(s), row, index));
			index += s.length();
		} else if (std::regex_search(line, m, name_reg)) {
			std::string s = m.str();
			tokens.push_back(Token(s, TokenType::id, tab.contains(s), row, index));
			index += s.length();
		} else if (std::regex_search(line, m, oper_reg)) {
			std::string s = m.str();
			tokens.push_back(Token(s, TokenType::oper, - 1, row, index));
			index += s.length();
		} else if (std::regex_search(line, m, relop_reg)) {
			std::string s = m.str();
			tokens.push_back(Token(s, TokenType::relop, -1, row, index));
			index += s.length();
		}
		else {
			if (line[0] != ' ' && line[0] != '\t' && line[0] != '\n') {
				std::cout << "Unknown token at line: " << row << " pos: " << index+1 << " Symbol:" << line[0] <<  std::endl;
				exit(1);
			}
			index++;
		}
	}
	void analysis(std::string name) {

		std::ifstream file(name);
		std::string buffer;
		int row = 1, index = 0;
		int pBegin = 0, pCur = 0;
		while (!file.eof()) {
			getline(file,buffer);
			while (index < buffer.length()) {
				getToken(buffer.substr(index), row, index);
			}
			index = 0;
			row++;
		}
		print();
		write();

	}
	void print() {
		std::cout << "|  Token type  |   Token value  | Row | Pos | Name\n";
		for (int i = 0; i < tokens.size(); i++) {
			std::cout << "|";
			printf("%14s", printType(tokens[i].type));
			std::cout << "| ";
			if (tokens[i].value_ref >= 0) {
				printf("%15d", tokens[i].value_ref);
				std::cout << "|";
			}
			else {
				printf("%15s", "");
				std::cout << "|";
			}
			printf("%5d", tokens[i].row); 
			std::cout << "|";
			printf("%5d", tokens[i].symb);
			std::cout << "| ";
			std::cout << tokens[i].name;
			std::cout << "\n";
		}
		tab.print();
	}
	void write() {
		std::ofstream file("listing.txt");

		for (int i = 0; i < tokens.size(); i++) {
			file << "Name: " + tokens[i].name + " | ";
			file << "Token type: ";
			file << printType(tokens[i].type);
			file << " | ";
			if (tokens[i].value_ref >= 0) {
				file << "Token value: " << tokens[i].value_ref << "(" << tab.getByIndex(tokens[i].value_ref) + ") | ";
			}
			file << "Row: " << tokens[i].row << " | ";
			file << "Pos: " << tokens[i].symb + 1 << " " << std::endl;
		}
		file << std::endl;
		for (int i = 0; i < tab.refs.size(); i++) {
			file << i << " -> " + tab.refs[i] << std::endl;
		}
		file.close();
	}
};




/*
class GeneralValue {
public:
	virtual void* getValue() const = 0;
};

class ValueInt : public GeneralValue {
public:
	std::string id;
	std::unique_ptr<int> value;
	ValueInt(int* value):value(std::move(value)){}
	void* getValue() const {
		return value.get();
	}
};

class ValueStr : public GeneralValue {
public:
	std::string id;
	std::unique_ptr<std::string> value;
	ValueStr(std::string* value) :value(value) {}
	void* getValue() const {
		return value.get();
	}
};
*/