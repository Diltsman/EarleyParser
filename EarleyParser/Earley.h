#ifndef EARLEY_0F361D5C_0338_44D0_814F_C83253328BB8
#define EARLEY_0F361D5C_0338_44D0_814F_C83253328BB8

#include <string_view>
#include <variant>
#include <vector>

class Rule {
public:
  Rule(std::string_view left);

  Rule &operator>>(char const terminal);
  Rule &operator>>(std::string_view nonterminal);

private:
  std::string_view const mLeft;
  std::vector<std::variant<char, std::string_view>> mRight;
};

#endif
