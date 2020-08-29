#include "Earley.h"

Rule::Rule(std::string_view left) : mLeft{left} {}

Rule &Rule::operator>>(char const terminal) {
  mRight.emplace_back(terminal);
  return *this;
}

Rule &Rule::operator>>(std::string_view nonterminal) {
  mRight.emplace_back(nonterminal);
  return *this;
}

Rule &Parser::createRule(std::string_view left) {
  return mRules.emplace_back(left);
}
