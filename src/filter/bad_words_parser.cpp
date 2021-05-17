#include <fstream>
#include <iostream>

#include "bad_words_parser.h"

namespace darqube::moderator::filter {

	BadWordsParser::BadWordsParser(std::string path) :
		m_path(path),
		m_badWordsCount(parseBadWordsCount()),
		m_badWords(parseBadWords())
	{ }

	int BadWordsParser::parseBadWordsCount() {
		int badWordsCount = 0;
	
		std::ifstream badWordsFile(m_path);
		std::string temp;
		while (std::getline(badWordsFile, temp)) {
			if (!temp.empty()) {
				badWordsCount++;
			}
		}
		badWordsFile.close();
		return badWordsCount;
	}

	std::vector<std::string> BadWordsParser::parseBadWords() {
		std::vector<std::string> badWords;
		badWords.reserve(m_badWordsCount);
	
		std::ifstream badWordsFile(m_path);
		std::string badWord;
		while (std::getline(badWordsFile, badWord)) {
			if (!badWord.empty()) {
				badWords.emplace_back(badWord);
			}
		}
		badWordsFile.close();
		return badWords;
	}

	int BadWordsParser::getBadWordsCount() {
		return m_badWordsCount;
	}

	std::vector<std::string>& BadWordsParser::getBadWords() {
		return m_badWords;
	}
}