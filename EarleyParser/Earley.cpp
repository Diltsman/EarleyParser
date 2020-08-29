#include "Earley.h"

Parser::Rule::Rule(std::string_view left) : mLeft{left} {}

Parser::Rule &Parser::Rule::operator>>(char const terminal) {
  mRight.emplace_back(terminal);
  return *this;
}

Parser::Rule &Parser::Rule::operator>>(std::string_view nonterminal) {
  mRight.emplace_back(nonterminal);
  return *this;
}

Parser::Rule &Parser::createRule(std::string_view left) {
  return mRules.emplace_back(left);
}
