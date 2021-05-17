#ifndef DARQUBE_MODERATOR_USER_USER_WRAPPER_H
#define DARQUBE_MODERATOR_USER_USER_WRAPPER_H

#include <string>

#include "json/serializable.h"

namespace darqube::moderator::user {

	struct User {
		std::string uid;
		std::string messagesHistory;
		int lastPenaltyTime;
		int remainingWarningsCount;
	};

	class UserWrapper : public darqube::moderator::json::Serializable {
	public:

		UserWrapper(std::string uid);
		UserWrapper(User user);

		void ban(int mins);
		void warn();

		void addCleanMessage();

		bool hasWarnings();
		bool hasEnoughCleanMessagesInRow();

		bool isNew();
		User& getUser();
		Json::Value toJson() const;

		void clear();

		static const int s_penaltyBase;

	private:

		User getEmptyUser(std::string uid);

		bool m_isNew;
		User m_user;
		size_t m_bannedMessagePosition;

		static const size_t s_normalCleanMessagesCount; // normal amount of clean messages in a row(increases warnings count and clears history)
		static const size_t s_sameSeverityCleanMessagesCount; // amout of clean messages in a row with same power of penalty as previous banned message had(in minutes)

	};

}

#endif // !defined(DARQUBE_MODERATOR_USER_USER_WRAPPER_H)