#include <string>
#include <filesystem>
#include <fstream>

#include <gtest/gtest.h>
#include <exception>

#include "user_wrapper.h"
#include "user_cache.h"
using namespace darqube::moderator::user;

TEST(UserTest, userWrapperTest) {
	std::string uid = "test_uid123";
	UserWrapper testUserWrapper(uid);
	User& testUser = testUserWrapper.getUser();

	ASSERT_EQ(testUser.messagesHistory, "");
	ASSERT_EQ(testUser.remainingWarningsCount, 3);
}

TEST(UserTest, userWrapperWarningTest) {
	std::string uid = "test_uid123";
	UserWrapper testUserWrapper(uid);
	User& testUser = testUserWrapper.getUser();

	testUserWrapper.warn();
	ASSERT_EQ(testUser.messagesHistory, "b");
	ASSERT_EQ(testUser.remainingWarningsCount, 2);
}

TEST(UserTest, userWrapperBanningAndHistoryCleaningTest) {
	std::string uid = "test_uid123";
	UserWrapper testUserWrapper(User{
		uid,
		"b",
		0,
		2
	});
	User& testUser = testUserWrapper.getUser();

	testUserWrapper.addCleanMessage();
	ASSERT_EQ(testUser.messagesHistory, "bc");

	testUserWrapper.addCleanMessage();
	ASSERT_EQ(testUser.messagesHistory, "bcc");

	ASSERT_FALSE(testUserWrapper.hasEnoughCleanMessagesInRow());

	testUserWrapper.ban(5);
	ASSERT_EQ(testUser.messagesHistory, "b");
	ASSERT_EQ(testUser.lastPenaltyTime, 5);
}

TEST(UserTest, userWrapperWarningsCountTest) {
	std::string uid = "test_uid123";
	UserWrapper testUserWrapper(User{
		uid,
		"b",
		0,
		2
	});
	User& testUser = testUserWrapper.getUser();

	// Testing Warnings count and adding warnings
	testUserWrapper.warn();
	testUserWrapper.warn();
	ASSERT_FALSE(testUserWrapper.hasWarnings());
	ASSERT_EQ(testUser.messagesHistory, "b");
}

TEST(UserTest, userWrapperWarningsMaximumCountTest) {
	std::string uid = "test_uid123";
	UserWrapper testUserWrapper(User{
		uid,
		"b",
		0,
		2
	});
	User& testUser = testUserWrapper.getUser();

	// Increment warnings count pasively
	for (int i = 0; i < 40; i++) {
		testUserWrapper.addCleanMessage();
	}

	ASSERT_EQ(testUser.remainingWarningsCount, 3);
}

TEST(UserTest, historyCleaningTest) {
	std::string uid = "test_uid123";
	UserWrapper testUserWrapper(User{
		uid,
		"b",
		0,
		2
	});
	User& testUser = testUserWrapper.getUser();

	// Check history cleaning and warnings incrementing after enoough 'c'(20 by default)
	testUser.messagesHistory = "bcccccccccccccccccc"; // 18 'c'
	testUser.remainingWarningsCount = 2;
	testUserWrapper.addCleanMessage();
	ASSERT_EQ(testUser.messagesHistory, "bccccccccccccccccccc"); // 19 'c'
	ASSERT_EQ(testUser.remainingWarningsCount, 2);

	testUserWrapper.addCleanMessage();
	ASSERT_EQ(testUser.messagesHistory, "");
	ASSERT_EQ(testUser.remainingWarningsCount, 3); // added one

	// Test user with long uncleared messages history
	testUserWrapper.clear();

	testUser.messagesHistory = "cccccccccccccccccc";
	ASSERT_EQ(testUser.messagesHistory, "cccccccccccccccccc");

	testUserWrapper.ban(5);
	ASSERT_EQ(testUser.messagesHistory, "ccccccccccccccccccb");
	ASSERT_EQ(testUser.lastPenaltyTime, 5);

	testUserWrapper.addCleanMessage();
	testUserWrapper.addCleanMessage();
	testUserWrapper.addCleanMessage();
	testUserWrapper.addCleanMessage();
	testUserWrapper.addCleanMessage();
	testUserWrapper.addCleanMessage();
	ASSERT_EQ(testUser.messagesHistory, "ccccccccccccccccccbcccccc");

	testUserWrapper.ban(5);
	ASSERT_EQ(testUser.messagesHistory, "b");
}

TEST(UserTest, userWrapperNewPropertyTest) {
	std::string uid = "test_uid123";
	UserWrapper testUserWrapper(uid);
	User& testUser = testUserWrapper.getUser();

	ASSERT_TRUE(testUserWrapper.isNew());

	std::string uid1 = "test_uid1234";
	User testUser1{uid, "bcc", 4, 3};
	UserWrapper testUserWrapper1(testUser1);
	ASSERT_FALSE(testUserWrapper1.isNew());
}

TEST(UserTest, userWrapperDecreasingPenaltyTime) {
	std::string uid = "test_uid123";
	UserWrapper testUserWrapper(User{
		uid,
		"b",
		15,
		0
	});
	User& testUser = testUserWrapper.getUser();

	// Increment warnings count pasively
	for (int i = 0; i < 40; i++) {
		testUserWrapper.addCleanMessage();
	}

	ASSERT_EQ(testUser.messagesHistory, "");
	ASSERT_EQ(testUser.lastPenaltyTime, 5);

	// Increment warnings count pasively
	for (int i = 0; i < 40; i++) {
		testUserWrapper.addCleanMessage();
	}

	ASSERT_EQ(testUser.messagesHistory, "");
	ASSERT_EQ(testUser.lastPenaltyTime, 0);
}

TEST(UserTest, userCacheTest) {

	std::filesystem::path testUserCacheFilePath = std::filesystem::temp_directory_path() / "test_user_cache.json";
	// Create empty user cache file
	{
		std::ofstream testUserCacheOutStream(testUserCacheFilePath);
		testUserCacheOutStream << "{[]}";
		testUserCacheOutStream.close();
	}

	// Creating user wrapper with user cache and test saving back when destruction comes
	{
		UserCache testUserCache(testUserCacheFilePath);

		std::string uid = "test_uid123";
		UserWrapper& testUserWrapper = testUserCache.findOrCreate(uid);
		User& testUser = testUserWrapper.getUser();

		testUser.messagesHistory = "bccccccccc";
		testUser.lastPenaltyTime = 10;
		testUser.remainingWarningsCount = 1;

		DocumentWriter documentWriter(testUserCacheFilePath);
		documentWriter.setOption("indentation", "");
		testUserCache.save(documentWriter);
	}

	std::ifstream testUserCacheInStream(testUserCacheFilePath);
	std::string testUserCacheContent, temp;
	while (std::getline(testUserCacheInStream, temp)) {
		testUserCacheContent += temp;
	}

	ASSERT_EQ(
		testUserCacheContent,
		"[{\"last_penalty_time\":10,\"messages_history\":\"bccccccccc\",\"remaining_warnings_count\":1,\"uid\":\"test_uid123\"}]"
	); // Properties are sorted in ASC order without any ability to change this behavior

	ASSERT_TRUE(std::filesystem::remove(testUserCacheFilePath)) << "Failed to delete temporary user cache file file: " << testUserCacheFilePath;
}

TEST(UserTest, penaltyCalculatingTest) {
	// TODO
}