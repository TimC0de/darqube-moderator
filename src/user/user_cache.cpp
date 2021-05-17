#include "user_cache.h"
#include "json/document_parser.h"

namespace darqube::moderator::user {

	UserCache::UserCache() :
		UserCache(std::filesystem::current_path() / "etc" / "user_cache.json")
	{}

	UserCache::UserCache(std::filesystem::path cacheFilePath) :
		m_userCacheFilePath(cacheFilePath)
	{
		parseCache();
	}

	void UserCache::save() {
		DocumentWriter userCacheWriter(m_userCacheFilePath);
		save(userCacheWriter);
	}

	void UserCache::save(DocumentWriter& documentWriter) {
		documentWriter.write(m_userCache);
	}

	void UserCache::parseCache() {
		darqube::moderator::json::DocumentParser userCacheParser(m_userCacheFilePath);
		Json::Value userCacheDocument = userCacheParser.getDocument();

		for (auto userNode : userCacheDocument) {
			std::string uid = userNode["uid"].asString();
			UserWrapper userWrapper(User{
				uid,
				userNode["messages_history"].asString(),
				userNode["last_penalty_time"].asInt(),
				userNode["remaining_warnings_count"].asInt()
			});

			m_userCache.insert(uid, userWrapper);
		}
	}

	UserWrapper& UserCache::findOrCreate(std::string uid) {
		return m_userCache.findOrCreate(uid, UserWrapper(uid));
	}

}