#ifndef DARQUBE_MODERATOR_USER_SERIALIZBABLE_MAP_H
#define DARQUBE_MODERATOR_USER_SERIALIZBABLE_MAP_H

#include <map>
#include <type_traits>

#include "json/serializable.h"

namespace darqube::moderator::user {

	template<typename kT, typename vT>
	class SerializableMap : public darqube::moderator::json::Serializable {

		using key_type = kT;
		using value_type = vT;

		using is_serializable = std::conjunction<
					std::bool_constant<
						!std::is_pointer<value_type>::value
					>,
					std::is_base_of<
						darqube::moderator::json::Serializable,
						value_type
					>
				>;

	public:

		static_assert(is_serializable::value, "Value type must be serializable");

		value_type& findOrCreate(const key_type& key, const value_type& value) {
			typename std::map<key_type, value_type>::iterator it;
			if ((it = m_map.find(key)) == m_map.end()) {
				return insert(key, value);
			}
			return it->second;
		}

		value_type& at(key_type& key) {
			return m_map.at(key);
		}

		value_type& insert(const key_type& key, const value_type& value) {
			const auto [insertedIterator, isSuccess] = m_map.insert(std::make_pair(key, value));
			return insertedIterator->second;
		}

		Json::Value toJson() {
			Json::Value result;
			for (const auto [key, value] : m_map) {
				result.append(value.toJson());
			}
			return result;
		}

	private:

		std::map<key_type, value_type> m_map;

	};

}

#endif // !defined(DARQUBE_MODERATOR_USER_SERIALIZABLE_MAP_H)