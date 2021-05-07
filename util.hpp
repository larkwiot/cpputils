#include <vector>
#include <string>
#include <concepts>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>
#include "lib/ut.hpp"

#ifndef LTSTAFFEL_UTIL_HPP
#define LTSTAFFEL_UTIL_HPP

namespace lt {

template <char delim>
std::vector<std::string> split(std::string& str) {
  std::stringstream str_stream {str};
  std::vector<std::string> fields {};
  std::string l;

  while (std::getline(str_stream, l, delim)) {
    fields.emplace_back(std::move(l));
  }

  return fields;
}

template <char delim>
std::string join(std::vector<std::string> const& vec) {
  std::string result {vec.front()};

  auto end = vec.end();
  for (auto si = vec.begin() + 1; si != end; ++si) {
    result += delim;
    result += *si;
  }

  return result;
}

template <std::unsigned_integral T>
std::vector<u_char> num_to_bytes(T n) {
  auto x = n;
  std::vector<u_char> result {};

  std::copy(static_cast<const char*>(static_cast<const void*>(&x)),
            static_cast<const char*>(static_cast<const void*>(&x)) + sizeof(n),
            std::back_inserter(result));

  std::reverse(result.begin(), result.end());

  return result;
}

std::vector<u_char> read_file_bytes(std::string& fn) {
  std::ifstream fh {fn, std::ios::binary | std::ios::ate};

  boost::ut::expect(!fh == false) << "could not open file:" << fn;

  auto end = fh.tellg();
  fh.seekg(0, std::ios::beg);

  auto size = std::size_t(end - fh.tellg());

  if (size == 0) {
    return {};
  }

  std::vector<u_char> bytes(size);

  boost::ut::expect(!fh.read(reinterpret_cast<char*>(bytes.data()), bytes.size()) == false) << "could not read file:" << fn;

  return bytes;
}

std::vector<std::string> read_file_lines(std::string& fn) {
  std::ifstream fh {fn};

  boost::ut::expect(!fh == false) << "could not open file:" << fn;

  std::vector<std::string> lines {};

  std::string l;
  while (std::getline(fh, l)) {
    lines.push_back(l);
  }
  
  return lines;
}

}

#endif