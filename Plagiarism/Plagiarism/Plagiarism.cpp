#include "pch.h"
#include "common.h" 
#include "Hasher.cpp"
#include "Globarizator.cpp"      
#include <fstream>         
               
int main()               
{        
	       
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
	std::cout << res.givePlagResults() << std::endl();
}     

 