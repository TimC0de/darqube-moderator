#include <fstream>

#include "document_writer.h"

namespace darqube::moderator::json {

	DocumentWriter::DocumentWriter(std::string path) :
		m_path(path)
	{}

	void DocumentWriter::write(Json::Value document) {
		std::ofstream documentOutputStream(m_path);
		documentOutputStream << Json::writeString(m_builder, document);
		documentOutputStream.close();
	}

}