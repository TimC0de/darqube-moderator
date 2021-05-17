#ifndef DARQUBE_MODERATOR_USER_USER_CACHE_H
#define DARQUBE_MODERATOR_USER_USER_CACHE_H

#include <filesystem>
#include <string>

#include "json/json.h"

#include "json/document_writer.h"
#include "serializable_map.h"
#include "user_wrapper.h"

namespace darqube::moderator::user {

	using DocumentWriter = darqube::moderator::json::DocumentWriter;

	class UserCache {

		template<typename key_type, typename value_type>
		using serializable_map = SerializableMap<key_type, value_type>;

	public:

		UserCache();
		UserCache(std::filesystem::path cacheFilePath);

		UserWrapper& findOrCreate(std::string uid);

		void save();
		void save(DocumentWriter& documentWriter);

	private:

		void parseCache();

		std::filesystem::path m_userCacheFilePath;
		serializable_map<std::string, UserWrapper> m_userCache;

	};

}

#endif // !defined(DARQUBE_MODERATOR_USER_USER_CACHE_H)