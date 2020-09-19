#ifndef EARLEY_0F361D5C_0338_44D0_814F_C83253328BB8
#define EARLEY_0F361D5C_0338_44D0_814F_C83253328BB8

#include <string_view>
#include <tuple>
#include <variant>
#include <vector>

class Parser {
  class Rule {
  public:
    explicit Rule(std::string_view left);

    Rule &operator>>(char const terminal);
    Rule &operator>>(std::string_view nonterminal);

    operator std::string_view() const;
    std::variant<char, std::string_view>
    operator[](std::size_t const index) const;
    std::size_t size() const;
    bool operator==(Rule const &r) const;

  private:
    std::string_view mLeft;
    std::vector<std::variant<char, std::string_view>> mRight;
  };

  struct State {
    std::vector<Rule>::const_iterator rule;
    std::size_t dotIndex;
    std::size_t originPosition;
    bool operator==(State const &r) const;
  };

public:
  Rule &createRule(std::string_view left);

  std::tuple<std::vector<std::size_t>, std::vector<State>>
  generateTable(std::string_view sentence) const;

private:
  std::size_t insert(std::vector<State> &states,
                     std::vector<State>::iterator begin,
                     std::vector<State>::iterator end,
                     State const &state) const;

  std::size_t mStart;
  std::vector<Rule> mRules;
};

#endif
