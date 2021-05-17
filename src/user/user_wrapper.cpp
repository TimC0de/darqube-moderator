#include "user_wrapper.h"

namespace darqube::moderator::user {

	const int UserWrapper::s_penaltyBase = 5;
	const size_t UserWrapper::s_normalCleanMessagesCount = 20;
	const size_t UserWrapper::s_sameSeverityCleanMessagesCount = 10;

	UserWrapper::UserWrapper(std::string uid) :
		m_isNew(true),
		m_user(getEmptyUser(uid)),
		m_bannedMessagePosition(m_user.messagesHistory.find('b'))
	{}

	UserWrapper::UserWrapper(User user) :
		m_isNew(false),
		m_user(user),
		m_bannedMessagePosition(m_user.messagesHistory.find('b'))
	{}

	User UserWrapper::getEmptyUser(std::string uid) {
		return User{uid, "", 0, 3};
	}

	void UserWrapper::ban(int mins) {
		m_user.lastPenaltyTime = mins;

		if (m_bannedMessagePosition != std::string::npos) {
			m_user.messagesHistory = "b";
			m_bannedMessagePosition = 0;
		}
		else {
			m_user.messagesHistory += 'b';
			m_bannedMessagePosition = m_user.messagesHistory.size() - 1;
		}
	}

	void UserWrapper::warn() {
		m_user.remainingWarningsCount--;

		if (m_bannedMessagePosition != std::string::npos) {
			m_user.messagesHistory = "b";
			m_bannedMessagePosition = 0;
		}
		else {
			m_user.messagesHistory += 'b';
			m_bannedMessagePosition = m_user.messagesHistory.size() - 1;
		}
	}

	void UserWrapper::addCleanMessage() {
		m_user.messagesHistory += 'c';

		if ((m_user.messagesHistory.size() - (m_bannedMessagePosition + 1)) >= s_normalCleanMessagesCount) {
			m_user.messagesHistory = "";
			m_bannedMessagePosition = std::string::npos;

			if (m_user.lastPenaltyTime > 0) {
				m_user.lastPenaltyTime -= s_penaltyBase;
			}

			if (m_user.remainingWarningsCount < 3) {
				m_user.remainingWarningsCount++;
			}
		}
	}

	bool UserWrapper::hasWarnings() {
		return m_user.remainingWarningsCount > 0;
	}

	bool UserWrapper::hasEnoughCleanMessagesInRow() {
		return (m_user.messagesHistory.size() - (m_bannedMessagePosition + 1)) >= s_sameSeverityCleanMessagesCount;
	}

	bool UserWrapper::isNew() {
		return m_isNew;
	}

	User& UserWrapper::getUser() {
		return m_user;
	}

	Json::Value UserWrapper::toJson() const {
		Json::Value userJson;
		userJson["uid"] = m_user.uid;
		userJson["messages_history"] = m_user.messagesHistory;
		userJson["last_penalty_time"] = m_user.lastPenaltyTime;
		userJson["remaining_warnings_count"] = m_user.remainingWarningsCount;
		return userJson;
	}

	void UserWrapper::clear() {
		m_bannedMessagePosition = std::string::npos;
	}

}