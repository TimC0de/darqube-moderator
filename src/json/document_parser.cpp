#include <fstream>

#include "document_parser.h"

namespace darqube::moderator::json {

	DocumentParser::DocumentParser(std::string path) :
		m_documentPath(path),
		m_document(parseDocument())
	{}

	Json::Value DocumentParser::parseDocument() {
		Json::Value document;
		std::ifstream documentFile(m_documentPath);

		Json::CharReaderBuilder documentReadBuilder;
		documentReadBuilder["collectComments"] = false;
		Json::parseFromStream(documentReadBuilder, documentFile, &document, NULL);

		documentFile.close();
		return document;
	}

	Json::Value DocumentParser::getDocument() {
		return m_document;
	}

}