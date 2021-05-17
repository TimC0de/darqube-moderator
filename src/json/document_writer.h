#ifndef DARQUBE_MODERATOR_JSON_DOCUMENT_WRITER_H
#define DARQUBE_MODERATOR_JSON_DOCUMENT_WRITER_H

#include <string>
#include <type_traits>

#include "json/json.h"
#include "json/writer.h"

#include "serializable.h"

namespace darqube::moderator::json {

	class DocumentWriter {
	public:

		DocumentWriter(std::string path);
		
		template<typename vT>
		void setOption(std::string key, vT value) {
			m_builder[key] = value;
		}

		template<typename SerializableT>
		void write(SerializableT document) {
			static_assert(std::is_base_of<Serializable, SerializableT>::value, "Document must be of type Serializable");
			
			write(document.toJson());
		}

		void write(Json::Value document);

	private:

		std::string m_path;
		Json::StreamWriterBuilder m_builder;

	};

}

#endif // !defined(DARQUBE_MODERATOR_JSON_DOCUMENT_WRITER_H)