#ifndef DARQUBE_MODERATOR_FILTER_CONFIG_H
#define DARQUBE_MODERATOR_FILTER_CONFIG_H

#include <string>
#include <variant>

namespace darqube::moderator::filter {

	class RegexFiltering {
	public:
		std::string type;
		std::string wordMatch;
		std::string replacePattern;
	};

	class FilterConfig {
	public:
		using Filtering = std::variant<std::string, RegexFiltering>;

		FilterConfig(std::string path);

		unsigned char getCensorChar();
		Filtering getFiltering();

	private:

		void config();

		std::string m_configPath;
		
		unsigned char m_censorChar;
		Filtering m_filtering;
	};

}

#endif // !defined(DARQUBE_MODERATOR_FILTER_CONFIG_H)