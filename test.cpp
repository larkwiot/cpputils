#include "util.hpp"

using namespace boost::ut;

int main() {
  "split"_test = [] {
    std::string test_str1 = "hello world!";
    std::vector<std::string> test_str1_res1 = {"hello", "world!"};
    std::vector<std::string> test_str1_res2 = {"hell", " w", "rld!"};
    expect(lt::split<' '>(test_str1) == test_str1_res1);
    expect(lt::split<'o'>(test_str1) == test_str1_res2);
  };

  "join"_test = [] {
    std::string test_str1 = "hello world!";
    std::vector<std::string> test_str1_res1 = {"hello", "world!"};
    std::vector<std::string> test_str1_res2 = {"hell", " w", "rld!"};
    expect(lt::join<' '>(test_str1_res1) == test_str1);
    expect(lt::join<'o'>(test_str1_res2) == test_str1);
  };

  "num_to_bytes"_test = [] {
    std::vector<u_char> test_byte1_res1 {0, 0, 0, 0xff};
    std::vector<u_char> test_byte1_res2 {0, 0, 0xff, 0};
    std::vector<u_char> test_byte1_res3 {0, 0xff, 0, 0};
    std::vector<u_char> test_byte1_res4 {0xff, 0, 0, 0};
    expect(lt::num_to_bytes(0xffu) == test_byte1_res1);
    expect(lt::num_to_bytes(0xff00u) == test_byte1_res2);
    expect(lt::num_to_bytes(0xff0000u) == test_byte1_res3);
    expect(lt::num_to_bytes(0xff000000u) == test_byte1_res4);
  };

  "read_file_bytes"_test = [] {
    std::vector<u_char> test_file1_res1 {73, 32, 97, 109, 32, 97, 32, 116, 101, 115, 116};
    std::string test_file1 = "test_file.txt";
    expect(lt::read_file_bytes(test_file1) == test_file1_res1);
  };

  "read_file_lines"_test = [] {
    std::vector<std::string> test_file_res1 {"I am a test"};
    std::string test_file1 = "test_file.txt";
    expect(lt::read_file_lines(test_file1) == test_file_res1);
  };
}