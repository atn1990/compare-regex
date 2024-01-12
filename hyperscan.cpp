#include <hs/hs.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

static int eventHandler(unsigned int id, unsigned long long from, unsigned long long to, unsigned int flags, void *ctx) {
  // printf("Match for pattern \"%s\" at offset %llu\n", (char *)ctx, to);
  std::vector<int>* ovector = (std::vector<int>*) ctx;
  ovector->push_back(from);
  ovector->push_back(to);
  return 0;
}

int main(int argc, char** argv) {
  std::ifstream ifsi("inputs.txt");
  if (!ifsi) {
    std::cout << "Failed to open inputs.txt" << std::endl;
    return 1;
  }
  std::vector<std::string> inputs;
  std::string input;
  while (std::getline(ifsi, input)) {
    std::cout << input << std::endl;
    inputs.push_back(input);
  }
  std::ifstream ifsp("patterns.txt");
  if (!ifsp) {
    std::cout << "Failed to open patterns.txt" << std::endl;
    return 1;
  }
  std::string pattern;
  while (std::getline(ifsp, pattern)) {
    std::cout << pattern << std::endl;
    hs_database_t *database;
    hs_compile_error_t *compile_err;
    if (hs_compile(pattern.c_str(), HS_FLAG_DOTALL, HS_MODE_BLOCK, nullptr, &database, &compile_err) != HS_SUCCESS) {
      fprintf(stderr, "ERROR: Unable to compile pattern \"%s\": %s\n",
              pattern.c_str(), compile_err->message);
      hs_free_compile_error(compile_err);
      continue;
    }
    hs_scratch_t *scratch = NULL;
    if (hs_alloc_scratch(database, &scratch) != HS_SUCCESS) {
      fprintf(stderr, "ERROR: Unable to allocate scratch space. Exiting.\n");
      hs_free_database(database);
      continue;
    }
    std::vector<std::string> match_list;
    for (auto& input : inputs) {
      // std::cout << input << std::endl;
      const char* a_str = input.c_str();
      uint32_t l_len = input.size();
      uint32_t l_offset = 0;
      int l_rc;
      std::vector<int> ovector;
      if (hs_scan(database, a_str, l_len, 0, scratch, eventHandler, (void *) &ovector) != HS_SUCCESS) {
        fprintf(stderr, "ERROR: Unable to scan input buffer. Exiting.\n");
        // hs_free_scratch(scratch);
        // hs_free_database(database);
        continue;
      }
      for (int i_match = 0; i_match < ovector.size()/2; ++i_match) {
        std::string l_match;
        l_match.assign(a_str + ovector[2*i_match], ovector[2*i_match+1] - ovector[2*i_match]);
        match_list.push_back(l_match);
        //NDBG_PRINT("%2d: %.*s\n", i_match, l_ovector[2*i_match+1] - l_ovector[2*i_match], a_str + l_ovector[2*i_match]);
      }
      // l_offset = ovector[1];
    }
    std::cout << "Matches: " << match_list.size() << std::endl;
    std::cout << std::endl;
    hs_free_scratch(scratch);
    hs_free_database(database);
  }
}
