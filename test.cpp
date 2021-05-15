#include "util.hpp"

using namespace boost::ut;

struct Point {
      int x;
      int y;

      bool operator== (Point const & other) const {
        return this->x == other.x && this->y == other.y;
      }
    };

namespace std {
  template <>
  struct hash<Point> {
    std::size_t operator() (Point const & p) const {
      return (p.x << sizeof(p.x)) + p.y;
    }
  };
}   

int main() {
  "String Operations"_test = [] {
    should("split") = [] {
      std::string test_str1 = "hello world!";
      std::vector<std::string> test_str1_res1 = {"hello", "world!"};
      std::vector<std::string> test_str1_res2 = {"hell", " w", "rld!"};

      expect(lt::split<' '>(test_str1) == test_str1_res1);
      expect(lt::split<'o'>(test_str1) == test_str1_res2);
    };

    should("join") = [] {
      std::string test_str1 = "hello world!";
      std::vector<std::string> test_str1_res1 = {"hello", "world!"};
      std::vector<std::string> test_str1_res2 = {"hell", " w", "rld!"};

      expect(lt::join<' '>(test_str1_res1) == test_str1);
      expect(lt::join<'o'>(test_str1_res2) == test_str1);
    };
  };

  "Bits & Bytes"_test = [] {
    should("num_to_bytes") = [] {
      std::vector<u_char> test_byte1_res1 {0, 0, 0, 0xff};
      std::vector<u_char> test_byte1_res2 {0, 0, 0xff, 0};
      std::vector<u_char> test_byte1_res3 {0, 0xff, 0, 0};
      std::vector<u_char> test_byte1_res4 {0xff, 0, 0, 0};

      expect(lt::num_to_bytes(0xffu) == test_byte1_res1);
      expect(lt::num_to_bytes(0xff00u) == test_byte1_res2);
      expect(lt::num_to_bytes(0xff0000u) == test_byte1_res3);
      expect(lt::num_to_bytes(0xff000000u) == test_byte1_res4);
    };
  };

  "File Operations"_test = [] {
    should("read_file_bytes") = [] {
      std::vector<u_char> test_file1_res1 {73, 32, 97, 109, 32, 97, 32, 116, 101, 115, 116};
      std::string test_file1 = "test_file.txt";
      expect(lt::read_file_bytes(test_file1) == test_file1_res1);
    };

    should("read_file_lines") = [] {
      std::vector<std::string> test_file_res1 {"I am a test"};
      std::string test_file1 = "test_file.txt";
      expect(lt::read_file_lines(test_file1) == test_file_res1);
    };
  };

  "Math"_test = [] {
    should("gcd") = [] {
      expect(lt::gcd(1, 2) == 1);
      expect(lt::gcd(1234*5, 1234) == 1234);
    };

    should("prime_factors") = [] {
      std::vector<int> test_factors_primes1 {2, 3};
      std::vector<bool> test_factors_primes2 {true, false, false};
      std::unordered_map<int, int> test_factors_res1 {
        {2, 2}
      };
      std::unordered_map<int, int> test_factors_res2 {
        {2, 2},
        {3, 1}
      };
      
      expect(lt::prime_factors(4, test_factors_primes1) == test_factors_res1);
      expect(lt::prime_factors(12, test_factors_primes1) == test_factors_res2);
      expect(lt::prime_factors(4, test_factors_primes2) == test_factors_res1);
      expect(lt::prime_factors(12, test_factors_primes2) == test_factors_res2);
    };

    should("is_prime") = [] {
      expect(lt::is_prime(13) == true);
      expect(lt::is_prime(14) == false);
      expect(lt::is_prime(1487) == true);
      expect(lt::is_prime(10) == false);
    };
  };

  "AdjacencyList"_test = [] {
    lt::AdjacencyList<Point> g {};
    std::unordered_map<Point*, std::vector<Point*>> graph_res {};
    auto x = new Point {1, 2};
    auto y = new Point {3, 4};

    should("add_vertex") = [&g, &graph_res, x] {
      g.add_vertex(x);
      graph_res.emplace(x, std::vector<Point*>{});

      expect(g.get_graph() == graph_res);
    };

    should("add_edge") = [&g, &graph_res, x, y] {
      g.add_vertex(y);
      graph_res.emplace(y, std::vector<Point*>{});

      g.add_edge(x, y);
      graph_res.at(x).emplace_back(y);

      expect(g.get_graph() == graph_res);
    };

    should("remove_vertex") = [&g, &graph_res, x, y] {
      graph_res.erase(y);
      auto y_loc = graph_res.at(x).begin();

      expect(*y_loc == y) << "test graph iterator does not point to correct pointer value for y";      

      graph_res.at(x).erase(y_loc);
      g.remove_vertex(y);

      expect(g.get_graph() == graph_res);
    };

    should("remove_edge") = [&g, &graph_res, x] {
      auto z = new Point {5, 6};

      g.add_vertex(z);
      g.add_edge(z, x);
      g.remove_edge(z, x);
      graph_res.emplace(z, std::vector<Point*>{});

      expect(g.get_graph() == graph_res);
    };
  };
}
