#include <vector>
#include <string>
#include <concepts>
#include <fstream>
#include <iostream>
#include <algorithm>

#ifndef LTSTAFFEL_UTIL_HPP
#define LTSTAFFEL_UTIL_HPP

namespace lt {

template <char delim>
std::vector<std::string> split(std::string& str) {
  std::vector<std::string> fields {};
  std::string l;

  while (std::getline(str, l, delim)) {
    fields.emplace_back(std::move(l));
  }

  return fields;
}

template <char delim>
std::string join(std::vector<std::string> const& vec) {
  std::string result {vec.at(0)};

  for (auto s : vec) {
    result += delim += s;
  }

  return result;
}

// int-type to byte vector


// read file bytes


std::vector<std::string> read_file_lines(std::string& fn) {
  std::ifstream fh {fn};

  if (!fh) {
    std::cerr << "[!] error: could not open " << fn << "\n";
    abort();
  }

  std::vector<std::string> lines {};

  std::string l;
  while (std::getline(fh, l)) {
    lines.push_back(l);
  }
  
  return lines;
}

}

#endif