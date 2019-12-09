#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include "..\Plagiarism\Globarizator.cpp"
#include "..\Plagiarism\Hasher.cpp"

TEST(Glodalizator, Code1) {
	std::ifstream task("..\\PlagTests\\testDocs\\TEST1\\TEST1.dat");
	std::string a;
	std::getline(task, a, '\0');
	Globalizator glob(a);
	std::ifstream answ("..\\PlagTests\\testDocs\\TEST1\\TEST1.ans");
	std::string b;
	answ >> b;
	a = glob.giveOutput();
    EXPECT_EQ(a, b);
}

TEST(Plagiarism, Code1) {
	std::ifstream doc1("..\\PlagTests\\testDocs\\TEST2\\TEST2_1.dat");
	std::string a;
	std::getline(doc1, a, '\0');
	std::ifstream doc2("..\\PlagTests\\testDocs\\TEST2\\TEST2_2.dat");
	std::string b;
	std::getline(doc2, b, '\0');
	Globalizator glob1(a);
	Globalizator glob2(b);
	Hasher hash1(glob1.giveOutput(), "..\\PlagTests\\testDocs\\TEST2\\TEST2_2.dat");
	Hasher hash2(glob2.giveOutput(), "..\\PlagTests\\testDocs\\TEST2\\TEST2_1.dat");
	Compare comp(hash1, hash2);
	PlagResults res = comp.makeResults();
	std::ifstream answ("..\\PlagTests\\testDocs\\TEST2\\TEST2.ans");
	bool check = false;
	if (res.comp1 == 100 && res.comp2 == 100)
		check = true;
	EXPECT_EQ(check, true);
}

TEST(Glodalizator, Code2) {
	std::ifstream task("..\\PlagTests\\testDocs\\TEST3\\TEST3.dat");
	std::string a;
	std::getline(task, a, '\0');
	Globalizator glob(a);
	std::ifstream answ("..\\PlagTests\\testDocs\\TEST3\\TEST3.ans");
	std::string b;
	answ >> b;
	a = glob.giveOutput();
	EXPECT_EQ(a, b);
}

TEST(Plagiarism, Code2) {
	std::ifstream doc1("..\\PlagTests\\testDocs\\TEST4\\TEST4_1.dat");
	std::string a;
	std::getline(doc1, a, '\0');
	std::ifstream doc2("..\\PlagTests\\testDocs\\TEST4\\TEST4_2.dat");
	std::string b;
	std::getline(doc2, b, '\0');
	Globalizator glob1(a);
	Globalizator glob2(b);
	Hasher hash1(glob1.giveOutput(), "..\\PlagTests\\testDocs\\TEST4\\TEST4_1.dat");
	Hasher hash2(glob2.giveOutput(), "..\\PlagTests\\testDocs\\TEST4\\TEST4_2.dat");
	Compare comp(hash1, hash2);
	PlagResults res = comp.makeResults();
	bool check = false;
	if (res.comp1 > 70 && res.comp2 > 70)
		check = true;
	EXPECT_EQ(check, true);
} 

TEST(Plagiarism, Code2_difficult) {
	std::ifstream doc1("..\\PlagTests\\testDocs\\TEST5\\TEST5_1.dat");
	std::string a;
	std::getline(doc1, a, '\0');
	std::ifstream doc2("..\\PlagTests\\testDocs\\TEST5\\TEST5_2.dat");
	std::string b;
	std::getline(doc2, b, '\0');
	Globalizator glob1(a);
	Globalizator glob2(b);
	Hasher hash1(glob1.giveOutput(), "..\\PlagTests\\testDocs\\TEST5\\TEST5_1.dat");
	Hasher hash2(glob2.giveOutput(), "..\\PlagTests\\testDocs\\TEST5\\TEST5_2.dat");
	Compare comp(hash1, hash2);
	PlagResults res = comp.makeResults();
	bool check = false;
	if (res.comp1 > 70 && res.comp2 > 70)
		check = true;
	EXPECT_EQ(check, true);
}


TEST(Plagiarism, DifferentCodes) {
	std::ifstream doc1("..\\PlagTests\\testDocs\\TEST6\\TEST6_1.dat");
	std::string a;
	std::getline(doc1, a, '\0');
	std::ifstream doc2("..\\PlagTests\\testDocs\\TEST6\\TEST6_2.dat");
	std::string b;
	std::getline(doc2, b, '\0');
	Globalizator glob1(a);
	Globalizator glob2(b);
	Hasher hash1(glob1.giveOutput(), "..\\PlagTests\\testDocs\\TEST6\\TEST6_1.dat");
	Hasher hash2(glob2.giveOutput(), "..\\PlagTests\\testDocs\\TEST6\\TEST6_2.dat");
	Compare comp(hash1, hash2);
	PlagResults res = comp.makeResults();
	EXPECT_EQ(res.givePlagResults(), "Plagiarism didn't found\n");
}

TEST(Glodalizator, Code3) {
	std::ifstream task("..\\PlagTests\\testDocs\\TEST7\\TEST7.dat");
	std::string a;
	std::getline(task, a, '\0');
	Globalizator glob(a);
	std::ifstream answ("..\\PlagTests\\testDocs\\TEST7\\TEST7.ans");
	std::string b;
	answ >> b;
	a = glob.giveOutput();
	EXPECT_EQ(a, b);
}

TEST(Plagiarism, DifCodesOneTask) {
	std::ifstream doc1("..\\PlagTests\\testDocs\\TEST8\\TEST8_1.dat");
	std::string a;
	std::getline(doc1, a, '\0');
	std::ifstream doc2("..\\PlagTests\\testDocs\\TEST8\\TEST8_2.dat");
	std::string b;
	std::getline(doc2, b, '\0');
	Globalizator glob1(a);
	Globalizator glob2(b);
	Hasher hash1(glob1.giveOutput(), "..\\PlagTests\\testDocs\\TEST8\\TEST8_1.dat");
	Hasher hash2(glob2.giveOutput(), "..\\PlagTests\\testDocs\\TEST8\\TEST8_2.dat");
	Compare comp(hash1, hash2);
	PlagResults res = comp.makeResults();
	EXPECT_EQ(res.givePlagResults(), "Plagiarism didn't found\n");
}