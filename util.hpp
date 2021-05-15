#include <vector>
#include <string>
#include <concepts>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <type_traits>

#include "lib/ut.hpp"

#ifndef LTSTAFFEL_UTIL_HPP
#define LTSTAFFEL_UTIL_HPP

namespace lt {

// need this so that we can do fatal asserts, but will
// keep fully qualifying namespaces for clarity
using namespace boost::ut;

// strings ////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

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

// bits & bytes ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

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

// files //////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

std::vector<u_char> read_file_bytes(std::string& fn) {
  std::ifstream fh {fn, std::ios::binary | std::ios::ate};

  boost::ut::expect((!fh == false) >> boost::ut::fatal) << "could not open file:" << fn;

  auto end = fh.tellg();
  fh.seekg(0, std::ios::beg);

  auto size = std::size_t(end - fh.tellg());

  if (size == 0) {
    return {};
  }

  std::vector<u_char> bytes(size);

  boost::ut::expect(
    (!fh.read(reinterpret_cast<char*>(bytes.data()), bytes.size()) == false)
     >> boost::ut::fatal
  ) << "could not read file:" << fn;

  return bytes;
}

std::vector<std::string> read_file_lines(std::string& fn) {
  std::ifstream fh {fn};

  boost::ut::expect((!fh == false) >> boost::ut::fatal) << "could not open file:" << fn;

  std::vector<std::string> lines {};

  std::string l;
  while (std::getline(fh, l)) {
    lines.push_back(l);
  }
  
  return lines;
}

// math ///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template <std::integral T>
T gcd(T a, T b) {
  while (a != b) {
    if (a > b) {
      a -= b;
    } else {
      b -= a;
    }
  }

  return a;
}

template <std::integral T>
std::pair<T, T> get_prime_count(T n, T p) {
  using namespace boost::ut;

  T count = 0;

  while (n % p == 0) {
    count++;
    n /= p;
  }

  boost::ut::expect((count > 0) >> boost::ut::fatal);

  return {n, count};
}

template <typename T>
concept PrimeTableElement = (std::same_as<T, bool> || std::integral<T>);

template <std::integral T, PrimeTableElement U>
std::unordered_map<T, T> prime_factors(T n, std::vector<U> primes) {
  std::unordered_map<T, T> factors {};

  if (std::is_same<U, bool>::value) {
    auto sz = primes.size();
    for (T i = 2; i <= static_cast<T>(sz); ++i) {
      if (i > n) {
        break;
      }

      if (primes[i - 1] == false) {
        auto [new_n, count] = get_prime_count(n, i);
        factors.emplace(i, count);

        if (new_n > 1) {
          n = new_n;
        } else {
          break;
        }
      }
    }
  } else {
    for (auto p : primes) {
      if (p > n) {
        break;
      }

      // we need this cast because otherwise p is actually a
      // std::_Bit_reference type?? I guess that's what an auto
      // iterator actually is
      auto [new_n, count] = get_prime_count(n, static_cast<T>(p));
      factors.emplace(p, count);

      if (new_n > 1) {
        n = new_n;
      } else {
        break;
      }
    }
  }

  return factors;
}

template <std::integral T>
bool is_prime(T n) {
  if (n == 2) {
    return true;
  }

  if (n % 2 == 0) {
    return false;
  }

  for (T i = 3; i * i < n; i += 2) {
    if (n % i == 0) {
      return false;
    }
  }

  return true;
}

// graph //////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template <typename T>
class AdjacencyList {
  std::unordered_map<T*, std::vector<T*>> g;

 public:
  ~AdjacencyList() {
    for (auto pair : this->g) {
      delete pair.first;
    }

    return;
  }

  void add_vertex(T* vx) {
    this->g.emplace(vx, std::vector<T*>{});
    return;
  }

  void remove_vertex(T* vx) {
    this->g.erase(vx);

    for (auto pair : this->g) {
      auto end = pair.second.end();
      for (auto neighbor = pair.second.begin(); neighbor != end; ++neighbor) {
        if (*neighbor == vx) {
          pair.second.erase(neighbor);
          this->g.at(pair.first) = pair.second;
          break;
        }
      }
    }

    delete vx;

    return;
  }

  void add_edge(T* va, T* vb) {
    this->g.at(va).emplace_back(vb);
    return;
  }

  void remove_edge(T* va, T* vb) {
    auto end = this->g.at(va).end();
    for (auto vi = this->g.at(va).begin(); vi != end; ++vi) {
      if (*vi == vb) {
        this->g.at(va).erase(vi);
        return;
      }
    }

    throw std::out_of_range("adjacent vertex not found");
  }

  auto get_graph() {
    return this->g;
  }

  void print_graph() {
    for (auto p : this->g) {
      if (p.second.size() == 0) {
        std::cout << p.first << " -> []\n";
        return;
      }

      std::cout << p.first << " -> [" << p.second.front();
      auto end = p.second.end();
      for (auto e = p.second.begin() + 1; e != end; ++e) {
        std::cout << ", " << *e;
      }
      std::cout << "]\n";
    }

    return;
  }
};

}

#endif
