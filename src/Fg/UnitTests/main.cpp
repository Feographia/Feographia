/*****************************************************************************
 *  Project:  Feographia
 *  Purpose:  The application to work with the biblical text
 *  Author:   NikitaFeodonit, nfeodonit@yandex.com
 *****************************************************************************
 *    Copyright (c) 2017-2020 NikitaFeodonit
 *
 *    This file is part of the Feographia project.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published
 *    by the Free Software Foundation, either version 3 of the License,
 *    or (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *    See the GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program. If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/

#include "gtest/gtest.h"

// MSVC pragmas
#pragma warning(push)
#pragma warning(disable : 5039 4619)

#include "Poco/Path.h"

#pragma warning(pop)

//#include <unistd.h>
//extern char **environ;

///*inline*/ fg::filesystem::path testDir;
///*inline*/ fg::filesystem::path fontDir;
///*inline*/ fg::filesystem::path dataDir;

inline Poco::Path testDir;
inline Poco::Path fontDir;
inline Poco::Path dataDir;

int main(int argc, char** argv)
{
//  fprintf(stdout, "================================== 01\n");
//  char buff[4096];
//  printf("work dir: %s\n", getcwd(buff, 4096));

//  int i = 1;
//  char *s = *environ;

//  for (; s; i++) {
//    printf("%s\n", s);
//    s = *(environ+i);
//  }
//  fprintf(stdout, "================================== 02\n");

  testing::InitGoogleTest(&argc, argv);

  // https://stackoverflow.com/a/55579815
  // See also:
  // https://stackoverflow.com/a/42772814
  // https://habr.com/ru/post/343622/
  testDir = Poco::Path {argv[0]}.parent().makeAbsolute();
  fontDir = dataDir = testDir;
  fontDir.pushDirectory("fonts");
  dataDir.pushDirectory("data");

  return RUN_ALL_TESTS();
}


//#include <iostream>

//int main(int argc, char** argv)
//{
//  // See:
//  // https://stackoverflow.com/a/42772814
//  // https://stackoverflow.com/a/55579815
//  // https://habr.com/ru/post/343622/

//  // https://stackoverflow.com/questions/4818785/how-to-pass-parameters-to-the-gtest
//  std::cout << "Running main() from gtest_main.cc dddddd\n";
//  testing::InitGoogleTest(&argc, argv);
//  //assert(argc >= 2);
//  for(int i = 0; i < argc; i++) {
//    std::cout << i << ":" << argv[i] << "\n";
//  }

//  return RUN_ALL_TESTS();
//}

//TEST(MainTest, main)
//{
//  EXPECT_TRUE(true);
//}
