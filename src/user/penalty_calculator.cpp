#include <cmath>

#include "penalty_calculator.h"
#include "user_cache.h"

namespace darqube::moderator::user {

	PenaltyCalculator::PenaltyCalculator(std::string uid) :
		m_userCache(),
		m_userWrapper(retrieveUserFromCache(uid))
	{}

	UserWrapper& PenaltyCalculator::retrieveUserFromCache(std::string uid) {
		return m_userCache.findOrCreate(uid);
	}

	Output PenaltyCalculator::calculateOutput(FilterOutput& filterOutput) {
		Output output{
			filterOutput.censoredMessage,
			filterOutput.censoredWordsCount,
			0, 
			false
		};

		if (filterOutput.censoredWordsCount == 0) {
			m_userWrapper.addCleanMessage();
			m_userCache.save();
			return output;
		}

		/*
		 * We need to warn user about bans firstly.
		 * Warnings count will be increased with increasing reputational points of user.
	 	 */
		if (m_userWrapper.hasWarnings()) {
			m_userWrapper.warn();
			output.warn = true;
		}
		else {
			int lastPenaltyTimeIndex = m_userWrapper.getUser().lastPenaltyTime / UserWrapper::s_penaltyBase;

			/*
			 * The idea is simple: the more consistency of bad words in message, the longer user will be banned.
			 * If `nextPenaltyItemIndex` is 1 and bad words amouont in text is higher then 20%, then `nextPenaltyItemIndex` will be increased and ban time will also increase.
			 * Makes sence only when bad words are more then 3 (little guarantee that we will not receive small message full of bad words)
			 */
			int incrementor = 1;
			if (filterOutput.censoredWordsCount > 3) {
				incrementor += std::lround((float)filterOutput.censoredWordsCount / (float)filterOutput.totalWordsCount * 100) / 20;
			}

			/*
			 * The time of every ban will be increased on 5 mins(5, 10, 15, ...).
			 * `nextPenaltyTimeIndex` shows the multiplier of 5, which will be used nextly(nextPenaltyTimeIndex = 2 => ban_time = 5 * 2 = 10 mins).
			 * If user previously sent enough clean messages in row to not to increase the nextPenaltyTimeIndex, then we decrease it
			 * (can not just remove `+ 1` from `nextPenaltyTimeIndex` calculation, because if user has no ban time previously - then his `nextPenaltyTimeIndex` has to minimally be 1)
			 */
			int nextPenaltyTimeIndex = lastPenaltyTimeIndex + incrementor;
			if (nextPenaltyTimeIndex > 1 && m_userWrapper.hasEnoughCleanMessagesInRow()) {
				nextPenaltyTimeIndex--;
			}

			m_userWrapper.ban(nextPenaltyTimeIndex * UserWrapper::s_penaltyBase);
			output.banTime = nextPenaltyTimeIndex * UserWrapper::s_penaltyBase;
		}
		m_userCache.save();
		return output;
	}

}