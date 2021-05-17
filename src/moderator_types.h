#ifndef DARQUBE_MODERATOR_TYPES_H
#define DARQUBE_MODERATOR_TYPES_H

#include <string>

namespace darqube::moderator {

	struct Input {
		std::string uid;
		std::string message;
	};

	struct Output {
		std::string censoredMessage;
		int censoredWordsCount;
		int banTime; // mins
		bool warn;
	};

	struct FilterOutput {
		std::string censoredMessage;
		int censoredWordsCount;
		int totalWordsCount;
	};

}

#endif // !defined(DARQUBE_MODERATOR_TYPES_H)