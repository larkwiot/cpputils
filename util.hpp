#include <vector>
#include <string>
#include <string>

// string split
// string join

// int-type to byte vector

// read file bytes
// read file lines
std::vector<std::string> read_file_lines(std::string fn) {
  std::ifstream fh {fn};

  if (!fh) {
    std::cerr << "[!] error: could not open " << fn << "\n";
    abort();
  }

  auto lines = std::vector<std::string> {};

  std::string l;
  while (std::getline(fh, l)) {
    lines.push_back(l);
  }
  
  return lines;
}