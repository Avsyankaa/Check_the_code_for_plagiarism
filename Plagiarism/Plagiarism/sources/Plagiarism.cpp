
#include "../include/Compare.h"
#include "../include/Globalizator.h"   
               
int main(int argc, char *argv[])
{                
	if (argc != 2) {
		std::cout << "Error: no arguments. Enter a path to config file" << std::endl;
		return 0;
	}
	std::ifstream configFile(argv[1]);
	std::string codeFirst;
	std::string codeSecond;
	std::string answerFile; 
	configFile >> codeFirst;
	configFile >> codeSecond;
	configFile >> answerFile;
	std::ifstream doc1(codeFirst);
	std::string a;
	std::getline(doc1, a, '\0');
	std::ifstream doc2(codeSecond);
	std::string b;
	std::getline(doc2, b, '\0');
	Globalizator glob1(a);
	Globalizator glob2(b); 
	Hasher hash1(glob1.giveOutput(), codeFirst);
	Hasher hash2(glob2.giveOutput(), codeSecond);
	Compare comp(hash1, hash2); 
	PlagResults res = comp.makeResults();
	std::ofstream answer(answerFile);
	answer << res.givePlagResults() << std::endl;
}     
    
 