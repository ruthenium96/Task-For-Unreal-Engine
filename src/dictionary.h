#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>

bool is_seqstring(const std::string & text, const std::string & pattern);

bool is_substring(const std::string & text, const std::string & pattern);

bool is_string(const std::string & text, const std::string & pattern, bool is_seq);

void find_in_previous(const std::vector<std::string>& previous,
          std::vector<std::string>& current, const std::string & pattern, bool is_seq);

void find_in_txt(std::vector<std::string>& current, const std::string & pattern, bool is_seq);

void find_in(const std::vector<std::string>& previous,
             std::vector<std::string>& current, const std::string & pattern,
             bool is_seq, bool from_previous);