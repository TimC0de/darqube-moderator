#ifndef DARQUBE_MODERATOR_JSON_SERIALIZABLE_H
#define DARQUBE_MODERATOR_JSON_SERIALIZABLE_H

#include "json/json.h"

namespace darqube::moderator::json {

	class Serializable {
	public:

		virtual Json::Value toJson() const {
			return Json::Value();
		}

	};

}

#endif // !defined(DARQUBE_MODERATOR_JSON_SERIALIZABLE_H)