#include <filesystem>
#include <fstream>

#include <gtest/gtest.h>

#include "moderator.h"
using namespace darqube::moderator;

TEST(ModeratorTest, moderatorTest) {
	// Clear user cache file
	std::filesystem::path userCachePath = std::filesystem::current_path() / "etc" / "user_cache.json";
	std::ofstream userCacheOutStream(userCachePath);
	userCacheOutStream << "[]";
	userCacheOutStream.close();

	// Passively receive 3 warnings
	for (int i = 0; i < 3; i++) {
		Input input{
			"test_user1",
			"Fuck you, nigga."
		};
		Output output = censorMessage(input);
		ASSERT_EQ(output.censoredMessage, "**** you, *****.");
		ASSERT_EQ(output.censoredWordsCount, 2);
		ASSERT_EQ(output.warn, true);
	}

	// Receive first ban
	{
		Input input{
			"test_user1",
			"Fuck you, nigga."
		};
		
		Output output = censorMessage(input);
		ASSERT_EQ(output.censoredMessage, "**** you, *****.");
		ASSERT_EQ(output.censoredWordsCount, 2);
		ASSERT_EQ(output.banTime, 5); // Default value of penalty base
	}

	// Receive second ban, but increased a lot
	{
		Input input{
			"test_user1",
			"Fuck you fucking nigga motherfucker asshole bitch fuck you in the ass."
		};
		Output output = censorMessage(input);
		ASSERT_EQ(output.censoredMessage, "**** you ******* ***** ************ ******* ***** **** you in the ***.");
		ASSERT_EQ(output.censoredWordsCount, 8);
		ASSERT_EQ(output.banTime, 25); // 8/12 words are bad => 75% => nextPenaltyTimeIndex + 3 = 5 - multiplier of penalty base(default is 5)
	}

	// Clear user cache at the end
	userCacheOutStream = std::ofstream(userCachePath);
	userCacheOutStream << "[]";
	userCacheOutStream.close();
}