#include <filesystem>
#include <fstream>

#include <gtest/gtest.h>

#include "document_parser.h"
#include "document_writer.h"
using namespace darqube::moderator::json;

TEST(JsonTest, documentParserTest) {
	// Create file for testing
	std::string testJsonFileContent = "{\"test_str\": \"test\", \"test_int\": 10}";
	std::filesystem::path testJsonPath = std::filesystem::temp_directory_path() / "test_json.json";
	std::ofstream testJsonOutStream(testJsonPath);
	ASSERT_TRUE(testJsonOutStream.is_open());

	testJsonOutStream << testJsonFileContent;
	testJsonOutStream.close();

	// Parsing via document parser
	DocumentParser documentParser(testJsonPath);
	Json::Value parsedDocument = documentParser.getDocument();
	ASSERT_TRUE(std::filesystem::remove(testJsonPath)) << "Failed to delete temporary json file: " << testJsonPath;

	// Censor character assertions
	ASSERT_TRUE(parsedDocument.isMember("test_str"));
	ASSERT_FALSE(parsedDocument["test_str"].isNull());
	ASSERT_EQ(parsedDocument["test_str"].asString(), std::string("test"));

	// Filtering type assertions
	ASSERT_TRUE(parsedDocument.isMember("test_int"));
	ASSERT_FALSE(parsedDocument["test_int"].isNull());
	ASSERT_EQ(parsedDocument["test_int"].asInt(), 10);
}

TEST(JsonTest, documentWriterTest) {
	// Document writer test
	Json::Value documentToWrite;
	documentToWrite["prop1"] = "test_property";
	documentToWrite["prop2"] = 11;

	Json::Value array;
	array.append("one");
	array.append("two");
	array.append("three");
	documentToWrite["prop3"] = array;

	std::filesystem::path testJsonPath = std::filesystem::temp_directory_path() / "test_json.json";
	DocumentWriter documentWriter(testJsonPath);
	documentWriter.setOption("indentation", "");
	documentWriter.write(documentToWrite);

	// Testing data writen by document writer
	std::ifstream testJsonInStream(testJsonPath);
	std::string testJsonFileContent;
	{
		std::string temp;
		while (std::getline(testJsonInStream, temp)) {
			testJsonFileContent += temp;
		}
	}
	testJsonInStream.close();
	ASSERT_EQ(testJsonFileContent, "{\"prop1\":\"test_property\",\"prop2\":11,\"prop3\":[\"one\",\"two\",\"three\"]}");

	ASSERT_TRUE(std::filesystem::remove(testJsonPath)) << "Failed to delete temporary json file: " << testJsonPath;
}