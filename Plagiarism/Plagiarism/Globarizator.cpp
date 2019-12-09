#include "common.h"
#include "pch.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <iostream>
#include <cctype>
class Globalizator {
	std::unordered_set <std::string> intTypes = {
	"int",
	"long",
	"short",
	"unsigned",
	"signed",
	};
	std::unordered_set <std::string> doubleTypes = {
		"float",
		"double",
		"long double",
	};
	std::unordered_set <std::string> charTypes = {
		"char",
		"string"
	};
	std::unordered_set <std::string> anotherTypes = {
		"bool",
		"mutex",
		"conditional_variable",
		"istringstram",
		"ostringstream",
		"ifstream",
		"ofstream"
	};
	std::unordered_set <std::string> stlArrays = {
		"array",
		"vector",
		"deque",
		"forward_list",
		"list"
	};
	std::unordered_set <std::string> stlMap = {
		"map",
		"multimap",
		"unordered_map",
		"unordered_multimap"
	};
	std::unordered_set <std::string> stlSet = {
		"set",
		"multiset",
		"unordered_set",
		"unordered_multiset"
	};
	std::unordered_set <std::string> stlAdap = {
		"stack",
		"queue",
		"priority_queue"
	};
	std::unordered_set <std::string> namespaceUsing{
		"std"
	};
	std::unordered_map<std::string, std::string> variables;
	std::unordered_set<std::string> functions;
	std::string inputText;
	std::string outputText;
	

	// ������� �����������, � ����� �������� ��� �������, ���������� � ��������
	auto makeTokenJob() -> std::vector<std::string> {
		std::istringstream buf(inputText);
		std::vector<std::string> outBuf;
		std::string line("");
		std::string word("");
		bool isLongComment = false;
		while (std::getline(buf, line, '\n')) {
			if (line.size() == 0)
				continue;
			if (isLongComment && line.size() == 1)
				continue;
			if (!isLongComment && line.size() == 1 && line != " " && line != "	") {
				workWithWord(line, outBuf);
				continue;
			}
			for (int i = 0; i < line.size() - 1; i++) {
				if (isLongComment && line[i] == '*' && line[i + 1] == '/') {
					isLongComment = false;
					i++;
					continue;
				}
				if (isLongComment)
					continue;
				if (line[i] == '/' && line[i + 1] == '*') {
					isLongComment = true;
					if (word != "")
						workWithWord(word, outBuf);
					word = "";
					continue;
				}
				if (line[i] == '/' && line[i + 1] == '/') {
					if (word != "")
						workWithWord(word, outBuf);
					word = "";
					break;
				}
				if (i == line.size() - 2) {
					word += line[line.size() - 2];
					if (line[line.size() - 1] != ' ' && line[line.size() - 1] != '	')
						word += line[line.size() - 1];
					workWithWord(word, outBuf);
					word = "";
					continue;
				}
				if (line[i] == ' ' || line[i] == '	') {
					if (word == "")
						continue;
					workWithWord(word, outBuf);
					word = "";
					continue;
				}
				word += line[i];
			}
		}

		makeTypeConvertJob(outBuf);
		makeDeletingJob(outBuf);
		makeReplacing(outBuf);
		return outBuf;
	}



	// ��������� ������ string ����� �������� ������������, ����� ������ ��� ������ ������� �
	// �������� ������ ������ ��������� ����
	auto workWithWord(std::string word, std::vector<std::string>& outBuf) -> void {
		std::string result_word("");
		for (int i = 0; i < word.size(); i++) {
			if (std::isalpha(static_cast<char>(word[i])) || word[i] == '_' || std::isdigit(static_cast<char>(word[i]))) {
				result_word += word[i];
				if (i == word.size() - 1)
					outBuf.push_back(result_word);
				continue;
			}
			if (result_word != "")
				outBuf.push_back(result_word);
			result_word = "";
			result_word += word[i];
			if (result_word != " " && result_word != "	")
				outBuf.push_back(result_word);
			result_word = "";
		}
	}



	//������ ���� �� �����, ������� namespace, �������� ��������� � ���������� ���������, ����������� � ��������
	auto makeTypeConvertJob(std::vector<std::string>& outBuf) -> void {
		int i = 0;
		while (i < outBuf.size()) {
			// ������� namespace
			if (outBuf[i] == "namespace") { // ������� ������� ��� namespace
				if (outBuf[i + 1] != "std")
					namespaceUsing.insert(outBuf[i + 1]);
				outBuf[i + 1] = "";
				outBuf[i] = "";
				i = i + 2;
				continue;
			}

			if (namespaceUsing.find(outBuf[i]) != namespaceUsing.end()) { // � ������ ������� ��, ���� ���������
				if (outBuf[i + 1] == ":" && outBuf[i + 2] == ":") {
					outBuf[i] = "";
					outBuf[i + 1] = "";
					outBuf[i + 2] = "";
					i = i + 3;
					continue;
				}
				i++;
				continue;
			}

			//������� �������
			if (outBuf[i] == "#" && outBuf[i + 1] == "include") {
				while (outBuf[i] != ">")
					i++;
				i++;
				continue;
			}

			//������ ������ ���������� � ������ ������� ����� ������ type
			if (outBuf[i] == "class" || outBuf[i] == "struct") {
				if (i + 1 < outBuf.size()) {
					anotherTypes.insert(outBuf[i + 1]);
					outBuf[i + 1].clear();
				}
				i = i + 2;
				continue;
			}

			if (anotherTypes.find(outBuf[i]) != anotherTypes.end()) {
				if (i + 1 < outBuf.size() && i + 2 < outBuf.size() && i + 3 < outBuf.size()) {
					if (outBuf[i + 1] == ":" && outBuf[i + 2] == ":") {
						outBuf[i] = "";
						outBuf[i + 1] = "";
						outBuf[i + 2] = "";
						i = i + 3;
						continue;
					}
				}
				outBuf[i] = "type";
				i = i + 1;
				continue;
			}

			if (outBuf[i] == "void" && i + 1 < outBuf.size()) {
				functions.insert(outBuf[i + 1]);
				outBuf[i + 1] = "func";
				i = i + 2;
				continue;
			}
			// ������� ��� int � �������� ��� 
			if (intTypes.find(outBuf[i]) != intTypes.end()) {
				while (intTypes.find(outBuf[i]) != intTypes.end()) {
					outBuf[i] = "";
					if (i + 1 < outBuf.size()) {
						i++;
					}
					else
						break;
				}
				if (doubleTypes.find(outBuf[i]) != doubleTypes.end()) {
					outBuf[i] = "double";
					i++;
					continue;
				}
				if (charTypes.find(outBuf[i]) != charTypes.end()) {
					outBuf[i] = "char";
					i++;
					continue;
				}
				outBuf[i - 1] = "int";
				i++;
				continue;
			}
			// ������ - ��������� double
			if (doubleTypes.find(outBuf[i]) != doubleTypes.end()) {
				outBuf[i] = "double";
				i++;
				continue;
			}
			// ������ - ��������� char
			if (charTypes.find(outBuf[i]) != charTypes.end()) {
				outBuf[i] = "char";
				i++;
				continue;
			}
			//�������� �������� �� ����������� ������ (��, ��� ����� ���, ���� ��� ���������, ����� ������� � ������ �����!!!)

			// ��������� ����������� stl
			// ���������� � ���������������� �������� � ���������
			if (stlArrays.find(outBuf[i]) != stlArrays.end()) {
				outBuf[i] = "array";
				i++;
				continue;
			}
			// ���������� map
			if (stlMap.find(outBuf[i]) != stlMap.end()) {
				outBuf[i] = "map";
				i++;
				continue;
			}

			//���������� set
			if (stlSet.find(outBuf[i]) != stlSet.end()) {
				outBuf[i] = "set";
				i++;
				continue;
			}

			//�������� �����������
			if (stlAdap.find(outBuf[i]) != stlAdap.end()) {
				outBuf[i] = "queue";
				i++;
				continue;
			}

			if (outBuf[i] == "\'") { // ������ ���������� ��������
				outBuf[i] = "chr";
				i++;
				while (i + 1 < outBuf.size()) {
					if (outBuf[i] == "\'") {
						outBuf[i].clear();
						break;
					}
					outBuf[i] = "";
					i++;
				}
				i++;
				continue;
			}

			if (outBuf[i] == "\"") { // ������ ��������� ��������
				outBuf[i] = "str";
				i++;
				while (i + 1 < outBuf.size()) {
					if (outBuf[i] == "\"") {
						outBuf[i].clear();

						break;
					}
					outBuf[i] = "";
					i++;
				}
				i++;
				continue;
			}
			i++;
		}
	}


	// ��������������� �������, ����������� ����������, ��� ����� ����: ���������� ������� ��� ����������
	auto isFunctionSimple(std::vector<std::string>& outBuf, int i) -> int { // ��� i - ������, �� �������� ����� ���
		
		//�������� �� �����������
		if (outBuf[i + 1] == "(") {
			i = i + 2;
			return i;
		}
		// �������� �� ������ ���������� � ���������� �������
		if (outBuf[i + 1] == "," || outBuf[i + 1] == ")" || outBuf[i+1] == ">") {
			i = i + 2;
			return i;
		}
		if (i + 1 < outBuf.size() && i + 2 < outBuf.size()) {
			if (outBuf[i + 2] == "=" || outBuf[i + 2] == ";" || outBuf[i + 2] == ","
				|| outBuf[i + 2] == ")" || outBuf[i + 2] == "}" ||
				outBuf[i + 2] == "[") { // ������ int a = b; int a; int a[23]; 
				variables.insert({ outBuf[i + 1], outBuf[i] });
				outBuf[i + 1] = "";
				return i + 3;
			}
			if (outBuf[i + 2] == "(" || outBuf[i + 2] == "{") { // C����� int a(5); int a{5}; int a(b); int a(c.get()); 
				if (i + 3 < outBuf.size()) {
					if (std::isdigit(outBuf[i + 3][0]) || outBuf[i + 3] == "chr" || outBuf[i + 3] == "str" || variables.find(outBuf[i + 3]) !=
						variables.end()) {
						variables.insert({ outBuf[i + 1], outBuf[i] });
						outBuf[i + 1] = "";
						i = i + 2;
						return i;
					}
				}
			}
			functions.insert(outBuf[i + 1]);
			outBuf[i + 1] = "func";
			return i + 2;
		}
		return i + 1;
	}


	// ���������������� �������, ����������� ����������, ��� ����� ����- ���������� ������� ��� ����������- � ������ ����������� STl
	auto isFunctionStl(std::vector<std::string>& outBuf, int i) -> int { // ��� i - ������, �� �������� ����� ���
		// ���� ����� ����
		int k = i;
		while (outBuf[k] != ">")
			k++;

		// �������� �� ������ ���������� � ���������� �������
		if (outBuf[k + 1] == "," || outBuf[k + 1] == ")" || outBuf[k+1] == ">") {
			k = k + 2;
			return k;
		}

		if (k + 1 < outBuf.size() && k + 2 < outBuf.size()) {
			if (outBuf[k + 2] == "=" || outBuf[k + 2] == ";" || outBuf[k + 2] == ","
				|| outBuf[k + 2] == ")" || outBuf[k + 2] == "}" ||
				outBuf[k + 2] == "[") { // ������ int a = b; int a; int a[23];
				std::string type;
				for (int j = i; j < k + 1; j++) {
					type += outBuf[j];
				}
				variables.insert({ outBuf[k + 1], type });
				outBuf[k + 1] = "";
				return k + 3;
			}
			if (outBuf[k + 2] == "(" || outBuf[k + 2] == "{") { // C����� int a(5); int a{5}; int a(b); int a(c.get()); 
				if (k + 3 < outBuf.size()) {
					if (std::isdigit(outBuf[k + 3][0]) || outBuf[k + 3] == "chr" || outBuf[k + 3] == "str" || variables.find(outBuf[k + 3]) !=
						variables.end()) {
						std::string type;
						for (int j = i; j < k + 1; j++) {
							type += outBuf[j];
						}
						variables.insert({ outBuf[k + 1], type });
						outBuf[k + 1] = "";
						k = k + 2;
						return k;
					}
				}
			}
			functions.insert(outBuf[k + 1]);
			outBuf[k + 1] = "func";
			return k + 2;
		}
		return k + 1;
	}

	//�������� ������ � ����������, ���������� �� ������, ������ �� ���������� �� ������� ����
	auto makeDeletingJob(std::vector<std::string>& outBuf) -> void {
		std::vector<std::string> outBuf2;
		int i = 0;
		while (i < outBuf.size()) {
			if (outBuf[i] == "") {
				i++;
				continue;
			}
			// ������� ������ �� ���������, ��������� �� ������, ������� ��������� � ��� �����
			if ((outBuf[i] == "*" && outBuf[i - 1] == "*") ||
				(outBuf[i] == "&" && outBuf[i - 1] == "*") ||
				(outBuf[i] == "&" && outBuf[i - 1] == "&") ||
				(outBuf[i] == "*" && outBuf[i - 1] == "&"))
			{
				if (outBuf[i - 2] == "int" ||
					outBuf[i - 2] == "double" ||
					outBuf[i - 2] == "char" ||
					outBuf[i - 2] == "type" ||
					outBuf[i - 2] == "," || outBuf[i - 2] == "(" || outBuf[i - 2] == ">")
				{
					i++;
					continue;
				}
			}
			// ������� ��������� ��������� � ������
			if ((outBuf[i] == "*") || (outBuf[i] == "&")) {
				if ((outBuf[i - 1] == "int" ||
					outBuf[i - 1] == "double" ||
					outBuf[i - 1] == "char" ||
					outBuf[i - 1] == "type" ||
					outBuf[i - 1] == "," || outBuf[i - 1] == "(" || outBuf[i - 1] == ">") && outBuf[i+1] !=  "=")
				{
					i++;
					continue;
				}
			}

			outBuf2.push_back(outBuf[i]);
			i++;
		}
		outBuf.clear();
		outBuf = outBuf2;
	}


	//������ ����� ������� �� func, � ����� ���������� - �� �� ���. ����������� � 
	//����������������� ���� *= += -= ++ --
	auto makeReplacing(std::vector<std::string>& outBuf) -> void {
		int i = 0;
		while (i < outBuf.size()) {
			if (outBuf[i] == "array" ||
				outBuf[i] == "map" ||
				outBuf[i] == "set" ||
				outBuf[i] == "queue") {
				i = isFunctionStl(outBuf, i);
				continue;
			}

			if (outBuf[i] == "int" ||
				outBuf[i] == "double" ||
				outBuf[i] == "char" ||
				outBuf[i] == "type") {
				i = isFunctionSimple(outBuf, i);
				continue;
			}

			if (functions.find(outBuf[i]) != functions.end()) {
				outBuf[i] = "func";
				i++;
				continue;
			}

			//������ ���������� �� �� ��� � ������ ������� ����� ����� ��� ->
			if (variables.find(outBuf[i]) != variables.end()) {
				outBuf[i] = variables[outBuf[i]];
				if (i + 1 < outBuf.size()) {
					if (outBuf[i + 1] == ".") {
		
						if ((variables.find(outBuf[i + 2]) != variables.end())) {
							outBuf[i + 1] = variables[outBuf[i + 1]];
							i = i + 3;
							continue;
						}
						outBuf[i + 2] = "func";
						i = i + 3;
						continue;
					}
					if (outBuf[i + 1] == "-" && outBuf[i + 2] == ">") {
						if ((variables.find(outBuf[i + 3]) != variables.end())) {
							outBuf[i + 2] = variables[outBuf[i + 2]];
							i = i + 4;
							continue;
						}
						outBuf[i + 3] = "func";
						i = i + 4;
						continue;
					}
				}
				i++;
				continue;
			}
			i++;
			continue;
		}
		i = 0;
		while (i < outBuf.size()) {
			if (outBuf[i] == "") {
				i++;
				continue;
			}
			// ����������� �� �������� += *= -= /= %=
			if ((outBuf[i] == "*" || outBuf[i] == "+" || outBuf[i] == "-" || outBuf[i] == "/"
				|| outBuf[i] == "%")
				&& outBuf[i + 1] == "=") {
				int k = i;
				i--;
				std::string tmp;
				while (outBuf[i] != "," && outBuf[i] != ";"
					&& outBuf[i] != "{" && outBuf[i] != "}" && outBuf[i] != ")" &&
					outBuf[i] != "(") {
					tmp = outBuf[i] + tmp;
					i--;
				}
				 
				std::string tmp_2;
				tmp_2 = outBuf[k];
				outBuf[k] = "";
				outBuf[k + 2] = tmp + tmp_2 + outBuf[k + 2];
				i = k + 1;
				continue;
			}
			// ����������� �� �������� -- ++ 
			if ((outBuf[i] == "+" && outBuf[i + 1] == "+") ||
				(outBuf[i] == "-" && outBuf[i + 1] == "-")) {
				if (outBuf[i - 1] == ";" || outBuf[i - 1] == "," || outBuf[i - 1] == "{" || outBuf[i - 1] == "("
					|| outBuf[i - 1] == "}" || outBuf[i - 1] == ")") {
				int k = i;
				i = i + 2; 
				std::string tmp; 
				while (outBuf[i] != "," && outBuf[i] != ";" && outBuf[i] != ")"
					&& outBuf[i] != "}") {
					tmp = tmp + outBuf[i];
					i++;
				}
				std::string tmp_2 = outBuf[k];
				outBuf[k] = tmp + "=";
				outBuf[k + 1] = tmp + tmp_2 + "1";
				for (unsigned j = k+2; j < i; j++)
					outBuf[j] = "";
				i = k;
				continue;  
				}
				int k = i;
				i = i - 1; 
				std::string tmp;
				while (outBuf[i] != "," && outBuf[i] != ";"
					&& outBuf[i] != "{") {
					tmp = outBuf[i] + tmp;
					i--;
				}
				
				std::string tmp_2 = outBuf[k];
				outBuf[k] = "";
				outBuf[k + 1] = "=" + tmp + tmp_2 + '1';
				i = k + 1;
				continue;
			}
			

			outputText += outBuf[i];
			i++;
		}
	};

	public:
		Globalizator(std::string text) {
			inputText = text;
		}

	auto giveOutput()->std::string {
		makeTokenJob();
		return outputText;
	}
};