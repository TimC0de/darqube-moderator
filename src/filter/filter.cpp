#include <fstream>
#include <functional>
#include <regex>
#include <algorithm>
#include <cctype>

#include "json/json.h"

#include "filter.h"
#include "bad_words_parser.h"

namespace darqube::moderator::filter {

	Filter::Filter(std::string message) :
		m_message(message),
		m_censoredWordsCount(0),
		m_totalWordsCount(0),
		m_badWordsFilePath(std::filesystem::current_path() / "etc" / "bad_words.txt"),
		m_filterConfigFilePath(std::filesystem::current_path() / "etc" / "filter_config.json")
	{}

	std::string Filter::getCensoredMessage() {
		if (!m_messageCensored) {
			censor();
		}
		return m_message;
	}

	int Filter::getCensoredWordsCount() {
		if (!m_messageCensored) {
			censor();
		}
		return m_censoredWordsCount;
	}

	int Filter::getTotalWordsCount() {
		if (!m_messageCensored) {
			censor();
		}
		return m_totalWordsCount;
	}

	void Filter::setBadWordsFilePath(std::filesystem::path badWordsFilePath) {
		m_badWordsFilePath = badWordsFilePath;
	}

	void Filter::setFilterConfigFilePath(std::filesystem::path filterConfigFilePath) {
		m_filterConfigFilePath = filterConfigFilePath;
	}

	std::filesystem::path Filter::getBadWordsFilePath() const { 
		return m_badWordsFilePath;
	}

	std::filesystem::path Filter::getFilterConfigFilePath() const {
		return m_filterConfigFilePath;
	}

	inline std::string Filter::getCensoredCharSequence(size_t size) {
		return std::string(size, m_censorChar);
	}

	std::string Filter::censorFullWord(std::string word) {
		return std::string(word.size(), m_censorChar);
	}

	std::string Filter::censorPartlyWord(std::string word) {
		return word.substr(0, 1) 
			+ std::string(word.size() - 2, m_censorChar)
			+ word.substr(word.size() - 1);
	}

	std::string Filter::censorRegexWord(std::string word) {
		// RegexFiltering regexFiltering = std::get<RegexFiltering>(m_filtering);
		// std::string replacePattern = regexFiltering.replacePattern;
		// replacePattern.find("{{CENSOR_PART}}")

		// std::regex wordMatchRegex(regexFiltering.wordMatch);
	 //    std::string new_s = std::regex_replace(word, regexFiltering.wordMatch, regexFiltering.replacePattern);
	    return word; 
	}

	void Filter::updateConfig() {
		FilterConfig filterConfig(m_filterConfigFilePath);
		m_censorChar = filterConfig.getCensorChar();
		m_filtering = filterConfig.getFiltering();
	}

	void Filter::censor() {
		updateConfig();

		// Chosing callback
		std::function<std::string(std::string)> censorWordCallback;

		if (std::holds_alternative<RegexFiltering>(m_filtering)) {
			censorWordCallback = std::bind(&Filter::censorRegexWord, this, std::placeholders::_1);
		}
		else {
			std::string filteringType = std::get<std::string>(m_filtering);

			if (filteringType == "full") {
				censorWordCallback = std::bind(&Filter::censorFullWord, this, std::placeholders::_1);
			} 
			else if (filteringType == "partly") {
				censorWordCallback = std::bind(&Filter::censorPartlyWord, this, std::placeholders::_1);
			}
		}

		// Bad words list initialization
		BadWordsParser badWordsParser(m_badWordsFilePath);
		std::vector<std::string> badWords = badWordsParser.getBadWords();

		// Going through message words
		std::function<std::string(std::string)> toLowerCallback = [](std::string s) {
			std::transform(s.begin(), s.end(), s.begin(),
	    		[](unsigned char c) { return std::tolower(c); }
    		);
    		return s;
		};

		std::regex word_regex("([@$!\\w]+)");
	    auto words_begin = std::sregex_iterator(m_message.begin(), m_message.end(), word_regex);
	    auto words_end = std::sregex_iterator();
	    for (std::sregex_iterator it = words_begin; it != words_end; it++) {
	    	std::smatch wordMatch = *it;
	    	std::string word = wordMatch.str();
	    	std::string lowerCaseWord = toLowerCallback(word);

	    	if (std::find(badWords.begin(), badWords.end(), lowerCaseWord) != badWords.end()) { // TODO: Optimize with big bad-words lists
	    		m_message = m_message.replace(wordMatch.position(), word.size(), censorWordCallback(word));
	    		m_censoredWordsCount++;
	    	}
	    	m_totalWordsCount++;
	    }

	    m_messageCensored = true;
	}
}