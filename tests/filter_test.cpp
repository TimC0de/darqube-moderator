#include <filesystem>
#include <fstream>

#include <gtest/gtest.h>

#include "bad_words_parser.h"
#include "filter_config.h"
#include "filter.h"
using namespace darqube::moderator::filter;

TEST(FilterTest, badWordsParsingTest) {
	// Creating temporary bad words file
	std::string testBadWordsFileContent = "test1\ntest2\ntest3\ntest4\n";
	std::filesystem::path testBadWordsFilePath = std::filesystem::temp_directory_path() / "test_bad_words.txt";
	std::ofstream testBadWordsOutStream(testBadWordsFilePath);
	ASSERT_TRUE(testBadWordsOutStream.is_open());

	testBadWordsOutStream << testBadWordsFileContent;
	testBadWordsOutStream.close();

	std::vector<std::string> badWords = { "test1", "test2", "test3", "test4" };
	int badWordsCount = badWords.size();

	BadWordsParser badWordsParser(testBadWordsFilePath);
	std::vector<std::string> parsedBadWords = badWordsParser.getBadWords();
	int parsedBadWordsCount = badWordsParser.getBadWordsCount();
	ASSERT_TRUE(std::filesystem::remove(testBadWordsFilePath)) << "Failed to delete temporary bad words file: " << testBadWordsFilePath;

	// Testing data
	ASSERT_EQ(badWordsCount, parsedBadWordsCount);

	int i = 0;
	while (i < badWordsCount) {
		ASSERT_EQ(badWords[i], parsedBadWords[i]);
		i++;
	}

	// Testing existence of current path /etc/bad_words.txt
	// Current path - path where executable is running
	ASSERT_TRUE(std::filesystem::exists(std::filesystem::current_path()  / "etc" / "bad_words.txt")) << "No bad words list \"${PROJECT_ROOT}/etc/bad_words.txt\" found";
}

TEST(FilterTest, filterConfigTest) {
	// Create config file
	std::string testConfigFileContent = "{\"censor_char\": \"*\", \"filtering\": \"testing\"}";
	std::filesystem::path testConfigPath = std::filesystem::temp_directory_path() / "test_config.json";
	std::ofstream testConfigOutStream(testConfigPath);
	ASSERT_TRUE(testConfigOutStream.is_open());

	testConfigOutStream << testConfigFileContent;
	testConfigOutStream.close();

	FilterConfig filterConfig(testConfigPath);
	ASSERT_TRUE(std::filesystem::remove(testConfigPath)) << "Failed to delete temporary config file: " << testConfigPath;

	// Testing data
	ASSERT_EQ(filterConfig.getCensorChar(), '*');
	ASSERT_TRUE(std::holds_alternative<std::string>(filterConfig.getFiltering()));
	ASSERT_EQ(std::get<std::string>(filterConfig.getFiltering()), "testing");

	// Testing existence of current path /etc/filter_config.json
	// Current path - path where executable is running
	ASSERT_TRUE(std::filesystem::exists(std::filesystem::current_path() / "etc" / "filter_config.json"))  << "No json document \"${PROJECT_ROOT}/etc/filter_config.json\" found";
}

TEST(FilterTest, filterConfigRegexTest) {
	// Create config file with regex filtering
	std::string testConfigFileContent = "{\"censor_char\": \".\", \"filtering\": {\"type\": \"regex\", \"word_match\": \"test_word\", \"replace_pattern\": \"test_pattern\"}}";
	std::filesystem::path testConfigPath = std::filesystem::temp_directory_path() / "test_config.json";
	std::ofstream testConfigOutStream(testConfigPath);
	ASSERT_TRUE(testConfigOutStream.is_open());

	testConfigOutStream << testConfigFileContent;
	testConfigOutStream.close();

	FilterConfig filterConfig(testConfigPath);
	ASSERT_TRUE(std::filesystem::remove(testConfigPath)) << "Failed to delete temporary config file: " << testConfigPath;
	
	// Testing data
	ASSERT_EQ(filterConfig.getCensorChar(), '.');

	ASSERT_TRUE(std::holds_alternative<RegexFiltering>(filterConfig.getFiltering()));
	RegexFiltering regexFilteringConfig 
		= std::get<RegexFiltering>(filterConfig.getFiltering());
	
	ASSERT_EQ(regexFilteringConfig.type, "regex");
	ASSERT_EQ(regexFilteringConfig.wordMatch, "test_word");
	ASSERT_EQ(regexFilteringConfig.replacePattern, "test_pattern");

	// Testing existence of current path /etc/filter_config.json
	// Current path - path where executable is running
	ASSERT_TRUE(std::filesystem::exists(std::filesystem::current_path() / "etc" / "filter_config.json"))  << "No json document \"${PROJECT_ROOT}/etc/filter_config.json\" found";
}

TEST(FilterTest, censorFullTest) {
	// Creating temporary bad words file
	std::string testBadWordsFileContent = "test1\ntest2\ntest3\ntest4\n";
	std::filesystem::path testBadWordsFilePath = std::filesystem::temp_directory_path() / "test_bad_words.txt";
	std::ofstream testBadWordsOutStream(testBadWordsFilePath);
	ASSERT_TRUE(testBadWordsOutStream.is_open());

	testBadWordsOutStream << testBadWordsFileContent;
	testBadWordsOutStream.close();

	// Create full-word config file
	std::string testFullWordConfigFileContent = "{\"censor_char\": \"*\", \"filtering\": \"full\"}";
	std::filesystem::path testFullWordConfigPath = std::filesystem::temp_directory_path() / "test_full_word_config.json";
	std::ofstream testFullWordConfigOutStream(testFullWordConfigPath);
	ASSERT_TRUE(testFullWordConfigOutStream.is_open());

	testFullWordConfigOutStream << testFullWordConfigFileContent;
	testFullWordConfigOutStream.close();

	// Test filter
	std::string testMessage = "The words test1 and test2 need to be censored. Also test5 will be remained here.";
	Filter filter(testMessage);

	// Configuring filter
	filter.setBadWordsFilePath(testBadWordsFilePath);
	filter.setFilterConfigFilePath(testFullWordConfigPath);

	// Testing data
	filter.censor();
	ASSERT_EQ(filter.getCensoredMessage(), "The words ***** and ***** need to be censored. Also test5 will be remained here.");
	ASSERT_EQ(filter.getCensoredWordsCount(), 2);

	// Deleting files
	ASSERT_TRUE(std::filesystem::remove(testFullWordConfigPath)) << "Failed to delete temporary config file: " << testFullWordConfigPath;
	ASSERT_TRUE(std::filesystem::remove(testBadWordsFilePath)) << "Failed to delete temporary bad words file: " << testBadWordsFilePath;
}

TEST(FilterTest, censorPartlyTest) {
	// Creating temporary bad words file
	std::string testBadWordsFileContent = "test1\ntest2\ntest3\ntest4\n";
	std::filesystem::path testBadWordsFilePath = std::filesystem::temp_directory_path() / "test_bad_words.txt";
	std::ofstream testBadWordsOutStream(testBadWordsFilePath);
	ASSERT_TRUE(testBadWordsOutStream.is_open());

	testBadWordsOutStream << testBadWordsFileContent;
	testBadWordsOutStream.close();

	// Create partly-word config file
	std::string testPartlyWordConfigFileContent = "{\"censor_char\": \"*\", \"filtering\": \"partly\"}";
	std::filesystem::path testPartlyWordConfigPath = std::filesystem::temp_directory_path() / "test_partly_word_config.json";
	std::ofstream testPartlyWordConfigOutStream(testPartlyWordConfigPath);
	ASSERT_TRUE(testPartlyWordConfigOutStream.is_open());

	testPartlyWordConfigOutStream << testPartlyWordConfigFileContent;
	testPartlyWordConfigOutStream.close();

	// Test filter
	std::string testMessage = "The words test1 and test2 need to be censored. Also test5 will be remained here.";
	Filter filter(testMessage);

	// Configuring filter
	filter.setBadWordsFilePath(testBadWordsFilePath);
	filter.setFilterConfigFilePath(testPartlyWordConfigPath);

	// Testing data
	filter.censor();
	ASSERT_EQ(filter.getCensoredMessage(), "The words t***1 and t***2 need to be censored. Also test5 will be remained here.");
	ASSERT_EQ(filter.getCensoredWordsCount(), 2);

	// Deleting files
	ASSERT_TRUE(std::filesystem::remove(testPartlyWordConfigPath)) << "Failed to delete temporary config file: " << testPartlyWordConfigPath;
	ASSERT_TRUE(std::filesystem::remove(testBadWordsFilePath)) << "Failed to delete temporary bad words file: " << testBadWordsFilePath;
}

TEST(FilterTest, censorPartlyCaseSensitiveTest) {
	// Creating temporary bad words file
	std::string testBadWordsFileContent = "test1\ntest2\ntest3\ntest4\n";
	std::filesystem::path testBadWordsFilePath = std::filesystem::temp_directory_path() / "test_bad_words.txt";
	std::ofstream testBadWordsOutStream(testBadWordsFilePath);
	ASSERT_TRUE(testBadWordsOutStream.is_open());

	testBadWordsOutStream << testBadWordsFileContent;
	testBadWordsOutStream.close();

	// Create partly-word config file
	std::string testPartlyWordConfigFileContent = "{\"censor_char\": \"*\", \"filtering\": \"partly\"}";
	std::filesystem::path testPartlyWordConfigPath = std::filesystem::temp_directory_path() / "test_partly_word_config.json";
	std::ofstream testPartlyWordConfigOutStream(testPartlyWordConfigPath);
	ASSERT_TRUE(testPartlyWordConfigOutStream.is_open());

	testPartlyWordConfigOutStream << testPartlyWordConfigFileContent;
	testPartlyWordConfigOutStream.close();

	// Test filter
	std::string testMessage = "The words Test1 and Test2 need to be censored. Also test5 will be remained here.";
	Filter filter(testMessage);

	// Configuring filter
	filter.setBadWordsFilePath(testBadWordsFilePath);
	filter.setFilterConfigFilePath(testPartlyWordConfigPath);

	// Testing data
	filter.censor();
	ASSERT_EQ(filter.getCensoredMessage(), "The words T***1 and T***2 need to be censored. Also test5 will be remained here.");
	ASSERT_EQ(filter.getCensoredWordsCount(), 2);

	// Deleting files
	ASSERT_TRUE(std::filesystem::remove(testPartlyWordConfigPath)) << "Failed to delete temporary config file: " << testPartlyWordConfigPath;
	ASSERT_TRUE(std::filesystem::remove(testBadWordsFilePath)) << "Failed to delete temporary bad words file: " << testBadWordsFilePath;
}

TEST(FilterTest, realDataFullTest) {
	// Testing data
	std::string niceMessage = "Everything is fucking terrible, nigga. I think you're an ashole, actually";
	Filter filter(niceMessage);
	ASSERT_EQ(filter.getBadWordsFilePath(), std::filesystem::path("/mnt/d/c++/darqube-moderator/build/etc/bad_words.txt"));
	ASSERT_EQ(filter.getFilterConfigFilePath(), std::filesystem::path("/mnt/d/c++/darqube-moderator/build/etc/filter_config.json"));

	filter.censor();


	ASSERT_EQ(filter.getCensoredMessage(), "Everything is ******* terrible, *****. I think you're an ******, actually");
	ASSERT_EQ(filter.getCensoredWordsCount(), 3);
	ASSERT_EQ(filter.getTotalWordsCount(), 12); // `you're` processed as different words
}