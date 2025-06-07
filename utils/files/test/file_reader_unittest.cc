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