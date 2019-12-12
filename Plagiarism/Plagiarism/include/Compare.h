#include "common.h"

struct BitOperations {
	unsigned long long MersennDigit = 2147483647;
	unsigned long long reduction(unsigned long long);
	unsigned long long mod(unsigned long long);
	unsigned long long add(unsigned long long, unsigned long long);
	unsigned long long sub(unsigned long long, unsigned long long);
};

class Hasher {
	unsigned long long x_in_k = 0;
	unsigned long long x_in_k_mod = 0;
	unsigned long long calculateFirstHash(std::string);
	unsigned long long calculateNextHash(std::string, std::string);
	void fillHashTable(std::string);
	std::vector<std::pair<unsigned long long, int>> CreateFingerprint(std::string);
public:
	short k = 6;
	short x = 19;
	short window = 10;
	BitOperations bitOperator;
	std::vector<unsigned long long> hashes;
	std::vector<std::pair<unsigned long long, int>> result;
	std::vector<int> indexes;
	std::string name;
	Hasher(std::string, std::string);
	Hasher() {};
};

struct PlagResults {
	std::string plag;
	double comp1 = 0;
	double comp2 = 0;
	 
	PlagResults(double, double, std::string, std::string);
	PlagResults(std::string, std::string);
	PlagResults() {};
	std::string givePlagResults();
};

class Compare {
	Hasher A;
	Hasher B;
	std::vector<std::pair<std::pair<unsigned long long, int>, std::pair<unsigned long long, int>>> AUB;
	PlagResults res;
	auto sortFingerPrint(std::vector<std::pair<unsigned long long, int>>&) -> void;
	auto mergeJoin() -> void;
	auto maxContaitment() -> void;
	auto WideHash() -> void;
public:
	Compare(Hasher a, Hasher b);
	auto makeResults()->PlagResults;
};