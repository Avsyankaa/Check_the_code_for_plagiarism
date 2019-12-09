#include "pch.h"
#include "common.h"
#include <iostream>
#include <string>
#include <list>
#include <algorithm>
#include <unordered_set>
#include <stdio.h>
#include <stdlib.h>
const unsigned long long MersennDigit = 2147483647;

struct BitOperations {
	unsigned long long reduction(unsigned long long digit) {
		return (digit & MersennDigit) + (digit >> 31);
	}

	unsigned long long mod(unsigned long long digit) {
		return reduction(reduction(digit) + 1) - 1;
	}

	unsigned long long add(unsigned long long digit1, unsigned long long digit2) {
		return (digit1 += digit2) >= MersennDigit ? (digit1 -= MersennDigit) : digit1;
	}

	unsigned long long sub(unsigned long long digit1, unsigned long long digit2) {
		return (digit1 -= digit2) >= MersennDigit ? (digit1 += MersennDigit) : digit1;
	}
};

class Hasher {

	unsigned long long x_in_k = 0;
	unsigned long long x_in_k_mod = 0;

	//Подсчет первого хэша
	unsigned long long calculateFirstHash(std::string firtsKgram) {
		unsigned long long hash = 0;
		for (unsigned i = 1; i < k + 1; i++) {
			x_in_k = x_in_k / x;
			//std::cout << (int)firtsKgram[i - 1] << std::endl;
			hash += bitOperator.mod(bitOperator.mod(x_in_k) * firtsKgram[i - 1]);
		}
		hash = bitOperator.mod(hash);
		return hash;
	}

	//Подсчет последующего кольцевого хэша
	unsigned long long calculateNextHash(std::string prevKgram, std::string newKgram) {
		unsigned long long newHash = 0;
		newHash = bitOperator.mod(bitOperator.mod((hashes.back() - prevKgram[0] * x_in_k_mod)*x) + newKgram.back());
		return newHash;
	}

	//Заполняем хэш документа
	void fillHashTable(std::string document) {
		std::string prevKgram;
		std::string nextKgram;
		for (unsigned i = 0; i < k; i++) {
			prevKgram += document[i];
		}
		hashes.push_back(calculateFirstHash(prevKgram));
		for (unsigned i = k; i < document.length(); i++) {
			nextKgram = prevKgram.substr(1, prevKgram.size() - 1);
			nextKgram += document[i];
			hashes.push_back(calculateNextHash(prevKgram, nextKgram));
			prevKgram = nextKgram; 
		}
	}

	//Создаем отпечаток
	std::vector<std::pair<unsigned long long, int>> CreateFingerprint(std::string document) {
		fillHashTable(document);
		auto min_index = 0; //добавить проверку на очень короткий документ
		for (unsigned i = 0; i < hashes.size() - window + 1; i++) {
			if ((min_index == i - 1) || (min_index == 0)) {
				min_index = i;
				for (short j = i + 1; j < i + window - 1; j++) {
					if (hashes[j] <= hashes[min_index])
						min_index = j;
				}
				result.push_back({ hashes[min_index], min_index });
				continue;
			}
			if (hashes[i + window - 1] <= hashes[min_index]) {
				min_index = i + window - 1;
				result.push_back({ hashes[min_index], min_index });
			}
		}
		return result;
	}

public:
	short k = 10;   
	short x = 19;
	short window = 15;
	BitOperations bitOperator;
	std::vector<unsigned long long> hashes; 
	std::vector<std::pair<unsigned long long, int>> result; 
	std::vector<int> indexes;
	std::string name;

	Hasher(std::string document, std::string name1) {
		x_in_k = std::pow(x, k); 
		name = name1;
		unsigned long long help = x_in_k / x;
		x_in_k_mod = bitOperator.mod(help);
		CreateFingerprint(document);
		
	}

	Hasher() {

	}
};

struct PlagResults {
	std::string plag;
	double comp1 = 0;
	double comp2 = 0;     

	PlagResults(double comp1_s, double comp2_s, std::string name1, std::string name2) {
		comp1 = comp1_s * 100;
		comp2 = comp2_s * 100;
		plag = "Document " + name1 + " contains " + std::to_string(comp1) + "% of plagiarism of document " + name2 + '\n'
			+ "Document " + name2 + " contains " + std::to_string(comp2) + "% of plagiarism of document " + name1 + '\n';
	}

	PlagResults(std::string name1, std::string name2) {
		plag = "Plagiarism didn't found\n";
	}

	PlagResults(){
	}
	auto givePlagResults()->std::string {
		return plag;
	}

};


class Compare {
	Hasher A;
	Hasher B;
	std::vector<std::pair<std::pair<unsigned long long, int>, std::pair<unsigned long long, int>>> AUB;
	PlagResults res;

	// Сортируем отпечатки
	auto sortFingerPrint(std::vector<std::pair<unsigned long long, int>>& vect) -> void {
		std::unordered_map<unsigned long long, int> tmp_table;
		std::vector<unsigned long long> tmp_vect;
		for (unsigned i = 0; i < vect.size(); i++) {
			tmp_table[vect[i].first] = vect[i].second;
			tmp_vect.push_back(vect[i].first);
		}
		std::sort(tmp_vect.begin(), tmp_vect.end());
		vect.clear();
		for (unsigned i = 0; i < tmp_vect.size(); i++) {
			vect.push_back({ tmp_vect[i],tmp_table[tmp_vect[i]] });
		}
	}

	//Занимаемся слиянием списков, чтобы найти объединение двух множеств отпечатков
	auto mergeJoin() -> void {
		int i = 0;
		int j = 0;
		sortFingerPrint(A.result);
		sortFingerPrint(B.result);
		while (i < A.result.size() && j < B.result.size()) {
			while (A.result[i].first < B.result[j].first) {
				if (i == A.result.size() - 1) {
					i++;
					break;
				}
				i++;
			}
			if (i >= A.result.size())
				break;
			if (j >= B.result.size())
				break;
			if (A.result[i].first == B.result[j].first) {
				AUB.push_back({ A.result[i], B.result[j] });
				i++;
				if (i == A.result.size())
					break;
			}
			if (i >= A.result.size())
				break;
			if (j >= B.result.size())
				break;
			while (A.result[i].first > B.result[j].first) {
				if (j == B.result.size() - 1) {
					j++;
					break;
				}
				j++;
			}
		}
	}

	// Подсчитываем предварительную меру плагиата
	auto maxContaitment() -> void {
		double maxContainment1 = (double)AUB.size() / (double)A.result.size();
		if (maxContainment1 < 0.2) {
			PlagResults res2(A.name, B.name);
			res = res2;
			return;
		}
		WideHash();
	}

	//Если предварительная мера не маловата, вычисляем более точную меру
	auto WideHash() -> void {
		int i = 0;
		std::vector<std::pair<int, int>> high_boundaries;
		std::vector<std::pair<int, int>> low_boundaries;
		std::vector<bool> result;
		result.resize(A.hashes.size());
		for (unsigned i = 0; i < A.hashes.size(); i++) {
			result[i] = false;
		}
		while (i != AUB.size()) {
			int A1 = AUB[i].first.second;
			int A2 = AUB[i].first.second;
			int B1 = AUB[i].second.second;
			int B2 = AUB[i].second.second;
			while (A.hashes[A1] == B.hashes[B1]) {
				result[A1] = true;
				A1--;
				B1--;
				if (A1 >= 0 && B1 >= 0) {
					result[A1] = true;
					continue;
				}
				break;
			}

			while (A.hashes[A2] == B.hashes[B2]) {
				result[A2] = true;
				A2++;
				B2++;
				if (A2 < A.hashes.size() && B2 < B.hashes.size()) {
					result[A2] = true;
					continue;
				}
				break;
			}
			i++;
		}
		unsigned counter = 0;
		for (unsigned i = 0; i < A.hashes.size(); i++) {
			if (result[i] == true)
				counter++;
		}
		double maxContaitment2 = (double)counter / (double)A.hashes.size();
		double maxContaitment3 = (double)counter / (double)B.hashes.size();
		PlagResults res2(maxContaitment2, maxContaitment3, A.name, B.name);
		res = res2;
	}


public:
	
	Compare(Hasher a, Hasher b) {
		A = a;
		B = b;
		if (A.result.size() > B.result.size())
			std::swap(A, B);
		mergeJoin();
		maxContaitment();
	}

	auto makeResults() -> PlagResults {
		return res;
	}
	
};