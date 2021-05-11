#pragma once
#include <map>
#include <functional>
#include <algorithm>
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
	int maxRuleSize = 8;
public:
	SyntaxisAnalisys() {
		nonterm.push_back("Program");
		nonterm.push_back("MultiStatement");
		nonterm.push_back("Expression");
		nonterm.push_back("Statement");
		nonterm.push_back("T");
		nonterm.push_back("P");
		nonterm.push_back("id");
		nonterm.push_back("number");
		nonterm.push_back("var");
		nonterm.push_back("if");
		nonterm.push_back("else");

		rules.emplace(std::vector<std::string>{"id"}, NodeT::L);
		rules.emplace(std::vector<std::string>{"number"}, NodeT::L);
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
		rules.emplace(std::vector<std::string>{"var", "id", ":=", "Expression",";"}, NodeT::Statement);
		rules.emplace(std::vector<std::string>{"id", ":=", "Expression", ";"}, NodeT::Statement);
		rules.emplace(std::vector<std::string>{"var", "id", ":","integer",":=", "Expression", ";"}, NodeT::Statement);
		rules.emplace(std::vector<std::string>{"id", ":", "integer", ";"}, NodeT::Statement);
		rules.emplace(std::vector<std::string>{"id", ":", "integer", ":=", "Expression", ";"}, NodeT::Statement);
		rules.emplace(std::vector<std::string>{"var", "id", ":", "integer", ";"}, NodeT::Statement);
		rules.emplace(std::vector<std::string>{"if","(","Expression",")","then","Statement","else", "Statement"}, NodeT::Statement);
		rules.emplace(std::vector<std::string>{"if", "(", "Expression", ")", "then", "Statement"}, NodeT::Statement);
		rules.emplace(std::vector<std::string>{"Statement", "Statement"}, NodeT::Statement);
		rules.emplace(std::vector<std::string>{"begin", "Statement", "end"}, NodeT::Program);
		rules.emplace(std::vector<std::string>{"Statement","begin", "Statement","end"}, NodeT::Program);
		auto temp = [](NodeT type) {
			return [=](std::vector<Node> c) mutable {
				return Node(NodeTWork::getNodeT(type), Token(), type, c);
			};
			//return Node(NodeTWork::getNodeT(type), Token(), type, ch);
		};
		reduction_f.emplace(std::vector<std::string>{"id"}, temp(NodeT::L));
		reduction_f.emplace(std::vector<std::string>{"number"}, temp(NodeT::L));
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
		reduction_f.emplace(std::vector<std::string>{"id", ":=", "Expression", ";"}, temp(NodeT::Statement));
		reduction_f.emplace(std::vector<std::string>{"id", ":", "integer", ":=", "Expression", ";"}, temp(NodeT::Statement));
		reduction_f.emplace(std::vector<std::string>{"id", ":", "integer", ";"}, temp(NodeT::Statement));
		reduction_f.emplace(std::vector<std::string>{"var","id", ":=", "Expression", ";"}, temp(NodeT::Statement));
		reduction_f.emplace(std::vector<std::string>{"var","id", ":", "integer", ":=", "Expression", ";"}, temp(NodeT::Statement));
		reduction_f.emplace(std::vector<std::string>{"var","id", ":", "integer", ";"}, temp(NodeT::Statement));
		reduction_f.emplace(std::vector<std::string>{"if", "(", "Expression", ")", "then", "Statement", "else", "Statement"}, temp(NodeT::Statement));
		reduction_f.emplace(std::vector<std::string>{"if", "(", "Expression", ")", "then", "Statement"}, temp(NodeT::Statement));
		reduction_f.emplace(std::vector<std::string>{"Statement", "Statement"}, temp(NodeT::Statement));
		reduction_f.emplace(std::vector<std::string>{"begin", "Statement", "end"}, temp(NodeT::Program));
		reduction_f.emplace(std::vector<std::string>{"Statement", "begin", "Statement", "end"}, temp(NodeT::Program));

	};

	std::vector<std::string> getNames(std::vector<Node> arr) {
		std::vector<std::string> temp;
		for (Node n : arr) {
			temp.push_back(n.name);
		}
		return temp;
	}
	bool startWith(std::vector<std::string> v1, std::vector<std::string> v2) {
		if (v1.size() < v2.size()) return false;
		for (int i = 0; i < v2.size(); i++) {
			if (v1[i] != v2[i]) return false;
		}
		return true;
	}
	SyntaxisAnalisys(std::vector<Token> t) {
		*this = SyntaxisAnalisys();
		this->setTokens(t);
	};

	void setTokens(std::vector<Token> t) {
		tokens = t;
	}

	bool startsWith(std::vector<Node> arr) {
		std::vector<std::string> names = getNames(arr);
		for (auto& const element : rules) {
			
			if (startWith(element.first, names)) {
				return true;
			}
		}
		return false;
	}
	bool isNonTerm(std::string str) {
		for (auto& const nt : nonterm) {

			if (nt == str) {
				return true;
			}
		}
		return false;
	}

	bool containRule(std::vector<std::string> str) {
		auto search = rules.find(str);
		if(search == rules.end()) return false;
		return true;
	}
	bool deep_reduce(std::vector<Node>& stack) {
		int max = std::min(maxRuleSize,(int) stack.size());
		for (int i = max; i > 0; i--) {

			for (int j = 0; j < (stack.size() - i) + 1; j++) {
				int maxReductSize = -1;
				auto itbegin = stack.end() - i - j;
				auto itend = stack.end() - j;
				std::vector<Node> temp(itbegin, itend);
				if (containRule(getNames(temp))) {
					std::vector<Node> childs;
					std::vector<std::string> temp;
					for (int k = i; k > 0; k--) {
						childs.insert(childs.begin(), stack[stack.size() - i - j + k -1 ]);
						temp.insert(temp.begin(), stack[stack.size() - i - j + k - 1].name);
					}
					stack.erase(itbegin, itend);
					stack.insert(stack.end() - j,reduction_f[temp](childs));
					return true;
				}
			}

		}
		return false;
	}
	void reduct(std::vector<Node>& stack,int size) {
		std::vector<Node> childs;
		std::vector<std::string> temp;
		for(int i = 0; i < size; i++){
			childs.insert(childs.begin(), stack[stack.size() - 1]);
			temp.insert(temp.begin(), stack[stack.size() - 1].name);
			stack.pop_back();
		}
		stack.push_back(reduction_f[temp](childs));
	}
	void throwExeption(Node node) {
		std::cout << "Syntaxis error at row " << node.t.row << " symb " << node.t.symb;
		exit(1);
	}
	bool addToStack(std::vector<Node>& stack, Node node) {
		int max = std::min(maxRuleSize, (int)stack.size());
		std::vector<Node> temp1;
		std::vector<Node> temp2{ node };
		int maxReductSize = -1;
		for (int i = stack.size() - 1; i >= (int)stack.size() - max; i--) {
			temp1.insert(temp1.begin(), stack[i]);
			temp2.insert(temp2.begin(), stack[i]);
			if (startsWith(temp2)) {
				stack.push_back(node);
				return false;
			}
			if (startsWith(temp1) && isNonTerm(node.name)) {
				stack.push_back(node);
				return false;
			}
			if (containRule(getNames(temp1))) {
				maxReductSize = stack.size() - i - 1;
			}
		}
		if (maxReductSize >= 0) {
			reduct(stack, maxReductSize+1);
			return true;
		}
		if (deep_reduce(stack)) {
			return true;
		}
		throwExeption(node);
		return true;
	};
	void final_reduct(std::vector<Node>& stack) {
		int max = std::min(maxRuleSize, (int)stack.size());
		std::vector<std::string> temp;
		int maxReductSize = -1;
		for (int i = (int)stack.size() - 1; i >= (int)stack.size() - max; i--) {
			temp.insert(temp.begin(), stack[i].name);
			if (containRule(temp)) {
				maxReductSize = stack.size() - i - 1;
			}
		}
		reduct(stack, maxReductSize + 1);
	}
	void pushNode(std::vector<Node>& stack, Node node) {
		if (stack.size() == 0) {
			stack.push_back(node);
		} else {
			do {

			} while (addToStack(stack, node));
		}
		if (node.name == ";") {
			final_reduct(stack);
		}		
		if (node.name == "begin") {
			while (deep_reduce(stack));
		}
	}
	void detour(std::ofstream& file, Node node, std::string way) {
		file << way;
		file << "; Node name: " << node.name;
		file << "; Node type: " << NodeTWork::getNodeT(node.type);
		if (node.type == NodeT::L && node.childs.size() == 0) {
			file << "; Token name: " << node.t.name;
			file << "; Token type: " << printType(node.t.type) << "\n";
		} else{
			file << "; Childs: [";
			for(int i = 0; i < node.childs.size();i++){
				if (i == node.childs.size() - 1){
					file << node.childs[i].name;
					
				}else {
					file << node.childs[i].name << ", ";
				}
			}
			file << "]\n";
			for (Node child : node.childs) {
				detour(file, child, "\t" + way + "->" + node.name);
			}
		}
	}
	void writeTree(Node tree) {
		std::ofstream file("tree.txt");
		detour(file, tree, "Way: root");
		file.close();
	}
	void startAnalisys() {
		std::vector<Node> stack;
		for (int i = 0; i < tokens.size();i++) {
			if (tokens[i].type == TokenType::id || tokens[i].type == TokenType::number) {
				pushNode(stack, Node(printType(tokens[i].type), tokens[i], NodeT::L, std::vector<Node>()));
			} else
			pushNode(stack, Node(tokens[i].name, tokens[i], NodeT::L, std::vector<Node>()));
		}


		while (true) {
			deep_reduce(stack);
			if (stack.size() == 1 && stack[0].type == NodeT::Program) break;
		}

		writeTree(stack[0]);
	}
};