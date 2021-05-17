#ifndef DARQUBE_MODERATOR_FILTER_H
#define DARQUBE_MODERATOR_FILTER_H

#include <filesystem>
#include <string>
#include <vector>

#include "filter_config.h"

namespace darqube::moderator::filter {

	class Filter {
	public:

		Filter(std::string message);

		void censor();
		std::string getCensoredMessage();
		int getCensoredWordsCount();
		int getTotalWordsCount();

		void updateConfig();

		void setBadWordsFilePath(std::filesystem::path badWordsPath);
		void setFilterConfigFilePath(std::filesystem::path filterConfigFilePath);

		std::filesystem::path getBadWordsFilePath() const;
		std::filesystem::path getFilterConfigFilePath() const;

	private:

		inline std::string getCensoredCharSequence(size_t size);
		std::string censorFullWord(std::string word);
		std::string censorPartlyWord(std::string word);
		std::string censorRegexWord(std::string word);

		bool m_messageCensored;
		int m_censoredWordsCount;
		int m_totalWordsCount;
		std::string m_message;

		std::filesystem::path m_badWordsFilePath;
		std::filesystem::path m_filterConfigFilePath;

		unsigned char m_censorChar;
		FilterConfig::Filtering m_filtering;		

	};

}

#endif // !defined(DARQUBE_MODERATOR_FILTER_H)