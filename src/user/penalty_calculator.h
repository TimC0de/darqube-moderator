#ifndef DARQUBE_MODERATOR_USER_PENALTY_CALCULATOR_H
#define DARQUBE_MODERATOR_USER_PENALTY_CALCULATOR_H

#include <string>

#include "moderator_types.h"
#include "user_wrapper.h"
#include "user_cache.h"

namespace darqube::moderator::user {

	class PenaltyCalculator {
	public:

		PenaltyCalculator(std::string uid);
		Output calculateOutput(FilterOutput& filterOutput);

	private:

		UserWrapper& retrieveUserFromCache(std::string uid);

		UserCache m_userCache;
		UserWrapper& m_userWrapper;

	};

}

#endif // !defined(DARQUBE_MODERATOR_USER_PENALTY_CALCULATOR_H)