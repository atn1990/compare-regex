#include <pcre.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

int main(int argc, char** argv) {
  std::cout << "PCRE version " << pcre_version() << std::endl;
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
    pcre* l_re;
    const char* l_error;
    int l_erroffset;
    l_re = pcre_compile(pattern.c_str(), PCRE_ANCHORED, &l_error, &l_erroffset, nullptr);
    if (l_re == nullptr) {
      std::cout << "pcre_compile failed (offset = " << l_erroffset << "): " << l_error << std::endl;
      std::cout << std::endl;
      continue;
    }
    std::vector<std::string> match_list;
    for (auto& input : inputs) {
      // std::cout << input << std::endl;
      const char* a_str = input.c_str();
      uint32_t l_len = input.size();
      uint32_t l_offset = 0;
      int l_rc;
      int l_ovector[100];
      while (l_offset < l_len) {
        l_rc = pcre_exec(l_re, 0, a_str, l_len, l_offset, 0, l_ovector, sizeof(l_ovector));
        std::cout << "l_rc: " << l_rc << std::endl;
        if (l_rc < 0) {
          break;
        }
        for (int i_match = 0; i_match < l_rc; ++i_match) {
          std::string l_match;
          l_match.assign(a_str + l_ovector[2*i_match], l_ovector[2*i_match+1] - l_ovector[2*i_match]);
          match_list.push_back(l_match);
          //NDBG_PRINT("%2d: %.*s\n", i_match, l_ovector[2*i_match+1] - l_ovector[2*i_match], a_str + l_ovector[2*i_match]);
        }
        l_offset = l_ovector[1];
      }
    }
    std::cout << "Matches: " << match_list.size() << std::endl;
    std::cout << std::endl;
  }
}
