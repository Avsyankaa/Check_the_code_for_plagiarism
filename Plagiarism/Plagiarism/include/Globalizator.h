#include "common.h"
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

	auto makeTokenJob()->std::vector<std::string>;
	auto workWithWord(std::string, std::vector<std::string>&) -> void;
	auto makeTypeConvertJob(std::vector<std::string>&) -> void;
	auto isFunctionSimple(std::vector<std::string>&, int) -> int;
	auto isFunctionStl(std::vector<std::string>&, int) -> int;
	auto makeDeletingJob(std::vector<std::string>&) -> void;
	auto makeReplacing(std::vector<std::string>&) -> void;
public:
	Globalizator(std::string);
	auto giveOutput()->std::string;
};