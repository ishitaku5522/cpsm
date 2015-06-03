// cpsm - fuzzy path matcher
// Copyright (C) 2015 Jamie Liu
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include <boost/program_options.hpp>

#include "match.h"
#include "matcher.h"
#include "str_util.h"

namespace po = boost::program_options;

namespace cpsm {
namespace {

int main(int argc, char** argv) {
  std::cin.sync_with_stdio(false);
  std::cout.sync_with_stdio(false);
  std::cerr.sync_with_stdio(false);

  po::options_description opts_desc("Options");
  opts_desc.add_options()
      ("cur_file", po::value<std::string>()->default_value(""),
       "cur_file passed to the matcher")
      ("limit", po::value<std::size_t>()->default_value(10),
       "maximum number of matches to return")
      ("query", po::value<std::string>()->default_value(""),
       "query to match items against")
      ("help", "display this help and exit")
      ;

  po::variables_map opts;
  po::store(po::parse_command_line(argc, argv, opts_desc), opts);
  po::notify(opts);

  if (opts.count("help")) {
    std::cout << opts_desc << std::endl;
    return 0;
  }

  MatcherOpts mopts;
  mopts.cur_file = opts["cur_file"].as<std::string>();
  Matcher matcher(opts["query"].as<std::string>(), std::move(mopts));
  std::size_t limit = opts["limit"].as<std::size_t>();
  std::vector<Match<std::string>> matches;
  std::vector<char32_t> buf, buf2;
  std::string line;
  while (std::getline(std::cin, line)) {
    Match<std::string> m(std::move(line));
    if (matcher.match(m.item, m, &buf, &buf2)) {
      matches.emplace_back(std::move(m));
      if (limit) {
        std::push_heap(matches.begin(), matches.end());
        if (matches.size() > limit) {
          std::pop_heap(matches.begin(), matches.end());
          matches.pop_back();
        }
      }
    }
  }
  std::sort(matches.begin(), matches.end());
  for (auto const& m : matches) {
    std::cout << m.item << " (" << m.debug_string() << ")" << std::endl;
  }

  return 0;
}

}  // namespace
}  // namespace cpsm

int main(int argc, char** argv) {
  return cpsm::main(argc, argv);
}
