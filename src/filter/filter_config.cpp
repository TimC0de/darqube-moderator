#include "filter_config.h"

#include "json/document_parser.h"

namespace darqube::moderator::filter {

	FilterConfig::FilterConfig(std::string path) : 
		m_configPath(path)
	{
		config();
	}

	void FilterConfig::config() {
		darqube::moderator::json::DocumentParser configParser(m_configPath);
		Json::Value document = configParser.getDocument();

		m_censorChar = document["censor_char"].asString().at(0);
		
		if (document["filtering"].isString()) {
			m_filtering = document["filtering"].asString();
		} 
		else {
			m_filtering = RegexFiltering{
				document["filtering"]["type"].asString(),
				document["filtering"]["word_match"].asString(),
				document["filtering"]["replace_pattern"].asString()
			};
		}
	}

	unsigned char FilterConfig::getCensorChar() {
		return m_censorChar;
	}

	FilterConfig::Filtering FilterConfig::getFiltering() {
		return m_filtering;
	}

}