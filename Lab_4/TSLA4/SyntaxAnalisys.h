#pragma once
#include <map>
#include <functional>
#include "LexicAnalisys.h"

enum class NodeT {
	Expression,
	Statement,
	T,
	P,
	L,
	MultiStatement,
	Program
};
class NodeTWork {
public:
	static NodeT getNodeT(std::string str) {
		if(str == "Expression")
		return NodeT::Expression;
		if (str == "Statement")
			return NodeT::Statement;
		if (str == "T")
			return NodeT::T;
		if (str == "P")
			return NodeT::P;
		if (str == "L")
			return NodeT::L;
		if (str == "MultiStatement")
			return NodeT::MultiStatement;
		if (str == "Program")
			return NodeT::Program;
	}
	static std::string getNodeT(NodeT node) {
		if (node == NodeT::Expression)
			return "Expression";
		if (node == NodeT::Statement)
			return "Statement";
		if (node == NodeT::T)
			return "T";
		if (node == NodeT::P)
			return "P";
		if (node == NodeT::L)
			return "L";
		if (node == NodeT::MultiStatement)
			return "MultiStatement";
		if (node == NodeT::Program)
			return "Program";
	}
};

struct Node {
	Token t;
	std::string name;
	NodeT type;
	std::vector<Node> childs;
	Node(std::string name, Token t, NodeT type, std::vector<Node> childs) :name(name), t(t), type(type), childs(childs) {};
	void addChilds(std::vector<Node> c) {
		for (Node n : c) {
			childs.push_back(n);
		}
	}
};
class SyntaxisAnalisys {
	std::vector<Token> tokens;
	std::map<std::vector<std::string>, NodeT> rules;
	std::map<std::vector<std::string>, std::function<Node(std::vector<Node>)>> reduction_f;
	std::vector<std::string> nonterm;
public:
	SyntaxisAnalisys() {
		nonterm.push_back("Program");
		nonterm.push_back("MultiStatement");
		nonterm.push_back("Expression");
		nonterm.push_back("Statement");
		nonterm.push_back("T");
		nonterm.push_back("P");
		nonterm.push_back("R");
		nonterm.push_back("dit");

		rules.emplace(std::vector<std::string>{"TokenName"}, NodeT::L);
		rules.emplace(std::vector<std::string>{"L"}, NodeT::T);
		rules.emplace(std::vector<std::string>{"T", "*", "T"}, NodeT::T);
		rules.emplace(std::vector<std::string>{"T"}, NodeT::P);
		rules.emplace(std::vector<std::string>{"P","+","P"}, NodeT::P);
		rules.emplace(std::vector<std::string>{"P", "-", "P"}, NodeT::P);
		rules.emplace(std::vector<std::string>{"-", "P"}, NodeT::P);
		rules.emplace(std::vector<std::string>{"P"}, NodeT::Expression);
		rules.emplace(std::vector<std::string>{"Expression", "==", "Expression"}, NodeT::Expression);
		rules.emplace(std::vector<std::string>{"Expression", "!=", "Expression"}, NodeT::Expression);
		rules.emplace(std::vector<std::string>{"Expression", "<", "Expression"}, NodeT::Expression);
		rules.emplace(std::vector<std::string>{"Expression", "==", "Expression"}, NodeT::Expression);
		rules.emplace(std::vector<std::string>{"TokenName", ":=", "Expression",";"}, NodeT::Statement);
		rules.emplace(std::vector<std::string>{"TokenName", ":","integer",":=", "Expression", ";"}, NodeT::Statement);
		rules.emplace(std::vector<std::string>{"TokenName", ":", "integer", ";"}, NodeT::Statement);
		rules.emplace(std::vector<std::string>{"if_t","(","Expression",")","Statement","else_t", "Statement"}, NodeT::Statement);
		rules.emplace(std::vector<std::string>{"if_t", "(", "Expression", ")", "Statement"}, NodeT::Statement);
		rules.emplace(std::vector<std::string>{"Statement"}, NodeT::MultiStatement);
		rules.emplace(std::vector<std::string>{"MultiStatement", "MultiStatement"}, NodeT::MultiStatement);
		rules.emplace(std::vector<std::string>{"var","MultiStatement","begin", "MultiStatement","end"}, NodeT::Program);
		auto temp = [](NodeT type) {
			return [=](std::vector<Node> c) mutable {
				return Node(NodeTWork::getNodeT(type), Token(), type, c);
			};
			//return Node(NodeTWork::getNodeT(type), Token(), type, ch);
		};
		reduction_f.emplace(std::vector<std::string>{"TokenName"}, temp(NodeT::T));
		reduction_f.emplace(std::vector<std::string>{"L"}, temp(NodeT::T));
		reduction_f.emplace(std::vector<std::string>{"T", "*", "T"}, temp(NodeT::T));
		reduction_f.emplace(std::vector<std::string>{"T"}, temp(NodeT::P));
		reduction_f.emplace(std::vector<std::string>{"P", "+", "P"}, temp(NodeT::P));
		reduction_f.emplace(std::vector<std::string>{"P", "-", "P"}, temp(NodeT::P));
		reduction_f.emplace(std::vector<std::string>{"-", "P"}, temp(NodeT::P));
		reduction_f.emplace(std::vector<std::string>{"P"}, temp(NodeT::Expression));
		reduction_f.emplace(std::vector<std::string>{"Expression", "==", "Expression"}, temp(NodeT::Expression));
		reduction_f.emplace(std::vector<std::string>{"Expression", "!=", "Expression"}, temp(NodeT::Expression));
		reduction_f.emplace(std::vector<std::string>{"Expression", "<", "Expression"}, temp(NodeT::Expression));
		reduction_f.emplace(std::vector<std::string>{"Expression", "==", "Expression"}, temp(NodeT::Expression));
		reduction_f.emplace(std::vector<std::string>{"TokenName", ":=", "Expression", ";"}, temp(NodeT::Statement));
		reduction_f.emplace(std::vector<std::string>{"TokenName", ":", "integer", ":=", "Expression", ";"}, temp(NodeT::Statement));
		reduction_f.emplace(std::vector<std::string>{"TokenName", ":", "integer", ";"}, temp(NodeT::Statement));
		reduction_f.emplace(std::vector<std::string>{"if_t", "(", "Expression", ")", "Statement", "else_t", "Statement"}, temp(NodeT::Statement));
		reduction_f.emplace(std::vector<std::string>{"if_t", "(", "Expression", ")", "Statement"}, temp(NodeT::Statement));
		reduction_f.emplace(std::vector<std::string>{"Statement"}, temp(NodeT::MultiStatement));
		reduction_f.emplace(std::vector<std::string>{"MultiStatement", "MultiStatement"}, temp(NodeT::MultiStatement));
		reduction_f.emplace(std::vector<std::string>{"var", "MultiStatement", "begin", "MultiStatement", "end"}, temp(NodeT::Program));

	};
	SyntaxisAnalisys(std::vector<Token> t) {
		*this = SyntaxisAnalisys();
		this->setTokens(t);
	};

	void setTokens(std::vector<Token> t) {
		tokens = t;
	}
	void startAnalisys() {

	}
};