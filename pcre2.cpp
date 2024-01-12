#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

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
    PCRE2_SIZE l_erroffset = 0;
    PCRE2_SIZE l_length = pattern.size();
    PCRE2_SPTR l_pattern = (PCRE2_SPTR) pattern.c_str();
    uint32_t options = PCRE2_ANCHORED; // PCRE2_MULTILINE | PCRE2_UTF | PCRE2_MATCH_INVALID_UTF;
    int l_errorcode;
    pcre2_code *l_re;
    l_re = pcre2_compile(l_pattern, l_length, options, &l_errorcode, &l_erroffset, nullptr);
    if(l_re == nullptr) {
      PCRE2_UCHAR l_buffer[256];
      pcre2_get_error_message(l_errorcode, l_buffer, sizeof(l_buffer));
      std::cout << "pcre2_compile failed (offset = " << l_erroffset << "): " << l_buffer << std::endl;
      std::cout << std::endl;
      continue;
    }
    int l_rc;
    l_rc = pcre2_jit_compile(l_re, PCRE2_JIT_COMPLETE);
    if (l_rc != 0) {
      std::cout << "pcre2_jit_compile failed: " << l_rc << std::endl;
      std::cout << std::endl;
      pcre2_code_free(l_re);
      continue;
    }
    pcre2_match_data *l_match_data = pcre2_match_data_create_from_pattern(l_re, nullptr);
    if (l_match_data == nullptr) {
      std::cout << "pcre2_match_data_create_from_pattern failed" << std::endl;
      std::cout << std::endl;
      pcre2_code_free(l_re);
      continue;
    }
    std::vector<std::string> match_list;
    for (auto& input : inputs) {
      // std::cout << input << std::endl;
      const char* a_str = input.c_str();
      uint32_t l_offset = 0;
      PCRE2_SPTR l_str = (PCRE2_SPTR) a_str;
      uint32_t l_len = input.size();
      while (l_offset < l_len) {
        l_rc = pcre2_match(l_re, l_str, l_len, l_offset, 0, l_match_data, nullptr);
        std::cout << "l_rc: " << l_rc << std::endl;
        if (l_rc < 0) {
          break;
        }
        PCRE2_SIZE *l_ovector = pcre2_get_ovector_pointer(l_match_data);
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
    pcre2_match_data_free(l_match_data); // Release memory used for the match
    pcre2_code_free(l_re);
  }
  return 0;
}
