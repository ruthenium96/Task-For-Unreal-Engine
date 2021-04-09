#include "dictionary.h"

bool is_seqstring(const std::string & text, const std::string & pattern) {
    int i = 0;
    int j = 0;
    while (i < text.size() && j < pattern.size()) {
        if (text[i] == pattern[j]) {
            j++;
        }
        i++;
    }
    if (j == pattern.size()) {
        return true;
    } else {
        return false;
    }
}

bool is_substring(const std::string & text, const std::string & pattern) {
    std::string tmp = pattern + "$" + text;
    int length = tmp.length();
    std::vector<int> z_fun(length, length);

    int l = 0;
    int r = 0;

    for (int i = 1; i < length; ++i) {
        z_fun[i] = (r > i) ? std::min(z_fun[i - l], r - i) : 0;
        while (i + z_fun[i] < length && tmp[z_fun[i]] == tmp[i + z_fun[i]]) {
            ++z_fun[i];
        }

        if (i + z_fun[i] > r) {
            l = i;
            r = i + z_fun[i];
        }
        if (z_fun[i] == pattern.length()) {
            return true;
        }
    }
    return false;
}

bool is_string(const std::string & text, const std::string & pattern, const bool is_seq) {
    return is_seq ? is_seqstring(text, pattern) : is_substring(text, pattern);
}

void find_in_previous(const std::vector<std::string>& previous,
          std::vector<std::string>& current, const std::string & pattern, const bool is_seq) {
    current.clear();
    for (const auto& s : previous) {
        if (is_string(s, pattern, is_seq)) {
            current.emplace_back(s);
        }
    }
}

void find_in_txt(std::vector<std::string>& current,
                 const std::string & pattern, const bool is_seq) {
    std::ifstream in("words.txt");
    std::string line;
    current.clear();
    if (in.is_open()) {
        while (getline(in, line)) {
            if (is_string(line, pattern, is_seq)) {
                current.emplace_back(line);
            }
        }
    }
    in.close();
}

void find_in(const std::vector<std::string>& previous,
          std::vector<std::string>& current, const std::string & pattern,
          const bool is_seq, const bool from_previous) {
    if (from_previous) {
        find_in_previous(previous, current, pattern, is_seq);
    } else {
        find_in_txt(current, pattern, is_seq);
    }
}