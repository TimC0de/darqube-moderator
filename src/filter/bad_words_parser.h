#ifndef DARQUBE_MODERATOR_BAD_WORDS_PARSER_H
#define DARQUBE_MODERATOR_BAD_WORDS_PARSER_H

#include <vector>
#include <string>

namespace darqube::moderator::filter {

	class BadWordsParser {
	public:

		BadWordsParser(std::string path);

		int getBadWordsCount();
		std::vector<std::string>& getBadWords();

	private:

		int parseBadWordsCount();
		std::vector<std::string> parseBadWords();

		std::string m_path;
		int m_badWordsCount;
		std::vector<std::string> m_badWords;

	};
}

#endif // !defined(DARQUBE_MODERATOR_BAD_WORDS_PARSER_H)