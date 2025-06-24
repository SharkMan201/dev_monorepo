//
// Created by abdoe on 7/06/2025.
//
#include "utils/files/file_reader.h"

#include <gtest/gtest.h>
#include <windows.h>

TEST(FileReaderTest, ReaderDummyFile) {
  const std::string file_path = "_main/utils/files/test/dummy_file.txt";
  const std::string &file_content = FilesUtils::FileReader::readFile(file_path);
  EXPECT_EQ("1\n2\n3", file_content);
}

TEST(FileReaderTest, ReaderDummyFileLines) {
  const std::string file_path = "_main/utils/files/test/dummy_file.txt";
  const auto &file_content = FilesUtils::FileReader::readFileLines(file_path);
  EXPECT_EQ(3, file_content.size());
  EXPECT_EQ("1", file_content[0]);
  EXPECT_EQ("2", file_content[1]);
  EXPECT_EQ("3", file_content[2]);
}