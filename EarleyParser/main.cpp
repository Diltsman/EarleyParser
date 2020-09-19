#include "Earley.h"
#include <iostream>

int main(int argc, char *argv[]) {
  Parser p;
  p.createRule("P") >> "S";
  p.createRule("S") >> "S" >> '+' >> "M";
  p.createRule("S") >> "M";
  p.createRule("M") >> "M" >> '*' >> "T";
  p.createRule("M") >> "T";
  p.createRule("T") >> '1';
  p.createRule("T") >> '2';
  p.createRule("T") >> '3';
  p.createRule("T") >> '4';

  auto const table = p.generateTable("2+3*4");

  auto const &offsets = std::get<0>(table);
  auto const &entries = std::get<1>(table);

  for (std::size_t i = 0; i < entries.size(); ++i) {
    if (auto location = std::find(std::begin(offsets), std::end(offsets), i);
        location != std::end(offsets)) {
      std::cout << "***" << (location - std::begin(offsets))
                << "*** --------------------------------\n";
    }
    auto const &entry = entries[i];
    std::cout << entry.originPosition << '\t'
              << static_cast<std::string_view>(*entry.rule) << " -->";
    for (std::size_t j = 0; j < entry.rule->size(); ++j) {
      if (entry.dotIndex == j) {
        std::cout << " *";
      }
      if (std::holds_alternative<char>((*entry.rule)[j])) {
        std::cout << " '" << std::get<char>((*entry.rule)[j]) << '\'';
      } else if (std::holds_alternative<std::string_view>((*entry.rule)[j])) {
        std::cout << " \"" << std::get<std::string_view>((*entry.rule)[j])
                  << '"';
      }
    }
    if (entry.dotIndex == entry.rule->size()) {
      std::cout << " *";
    }
    std::cout << '\n';
  }

  return 0;
}
