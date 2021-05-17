#ifndef DARQUBE_MODERATOR_JSON_DOCUMENT_PARSER_H
#define DARQUBE_MODERATOR_JSON_DOCUMENT_PARSER_H

#include <string>
#include "json/json.h"

namespace darqube::moderator::json {

	class DocumentParser {
	public:

		DocumentParser(std::string path);
		Json::Value getDocument();

	private:

		Json::Value parseDocument();

		std::string m_documentPath;
		Json::Value m_document;
	};

}

#endif // !defined(DARQUBE_MODERATOR_JSON_DOCUMENT_PARSER_H)