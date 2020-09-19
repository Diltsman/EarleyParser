#include "Earley.h"
#include <algorithm>
#include <iterator>
#include <optional>

Parser::Rule::Rule(std::string_view left) : mLeft{left} {}

Parser::Rule &Parser::Rule::operator>>(char const terminal) {
  mRight.emplace_back(terminal);
  return *this;
}

Parser::Rule &Parser::Rule::operator>>(std::string_view nonterminal) {
  mRight.emplace_back(nonterminal);
  return *this;
}

Parser::Rule::operator std::string_view() const { return mLeft; }

std::variant<char, std::string_view>
Parser::Rule::operator[](std::size_t const index) const {
  return mRight[index];
}

std::size_t Parser::Rule::size() const { return mRight.size(); }

bool Parser::Rule::operator==(Rule const &r) const {
  return mLeft == r.mLeft && mRight == r.mRight;
}

Parser::Rule &Parser::createRule(std::string_view left) {
  if (mRules.empty()) {
    mStart = 0;
  }
  auto location = std::upper_bound(
      std::begin(mRules), std::end(mRules), left,
      [](std::string_view l, std::string_view r) { return l < r; });
  if (!mRules.empty() && static_cast<std::size_t>(std::distance(
                             std::begin(mRules), location)) <= mStart) {
    ++mStart;
  }
  return *mRules.insert(location, Rule{left});
}

std::tuple<std::vector<std::size_t>, std::vector<Parser::State>>
Parser::generateTable(std::string_view sentence) const {
  std::tuple<std::vector<std::size_t>, std::vector<State>> table;
  auto &offsets = std::get<0>(table);
  auto &entries = std::get<1>(table);

  // Add starting rule
  offsets.push_back(0);
  entries.push_back({std::cbegin(mRules) + mStart, 0, 0});
  for (std::string_view::size_type i = 0; i <= sentence.size(); ++i) {
    // for (auto i = std::cbegin(sentence); i != std::cend(sentence); ++i) {
    std::optional<char> c = i == sentence.size()
                                ? std::optional<char>{}
                                : std::optional<char>{sentence[i]};
    // char const c = *i;
    // Prep for the next character
    offsets.push_back(entries.size());
    auto next = std::rbegin(offsets);
    auto current = next + 1;
    for (auto j = *current; j < *next; ++j) {
      auto const &entry = entries[j];
      auto const &rule = *entry.rule;
      if (rule.size() <= entry.dotIndex) {
        // Completion
        std::string_view const left = rule;
        auto const begin = offsets[entry.originPosition];
        auto const end = offsets[entry.originPosition + 1];
        for (auto k = begin; k != end; ++k) {
          auto const &entry = entries[k];
          if (entry.rule->size() > entry.dotIndex &&
              std::holds_alternative<std::string_view>(
                  (*entry.rule)[entry.dotIndex]) &&
              std::get<std::string_view>((*entry.rule)[entry.dotIndex]) ==
                  left) {
            *next += insert(entries, std::begin(entries) + *current,
                            std::begin(entries) + *next,
                            Parser::State{entry.rule, entry.dotIndex + 1,
                                          entry.originPosition});
          }
        }
      } else if (std::holds_alternative<char>(rule[entry.dotIndex])) {
        // Scanning
        char const terminal = std::get<char>(rule[entry.dotIndex]);
        if (c.has_value() && terminal == *c) {
          insert(entries, std::begin(entries) + *next, std::end(entries),
                 Parser::State{entry.rule, entry.dotIndex + 1,
                               entry.originPosition});
        }
      } else if (std::holds_alternative<std::string_view>(
                     rule[entry.dotIndex])) {
        // Prediction
        std::string_view const nonterminal =
            std::get<std::string_view>(rule[entry.dotIndex]);
        auto range =
            std::equal_range(std::begin(mRules), std::end(mRules), nonterminal);
        for (auto k = std::get<0>(range); k < std::get<1>(range); ++k) {
          *next += insert(entries, std::begin(entries) + *current,
                          std::begin(entries) + *next, Parser::State{k, 0, i});
        }
      }
    }
  }
  // Get rid of the extra offset
  // TODO: Does it need to keep this so processing the table is consistent?
  offsets.pop_back();

  return table;
}

std::size_t Parser::insert(std::vector<State> &states,
                           std::vector<State>::iterator begin,
                           std::vector<State>::iterator end,
                           State const &state) const {
  bool const matched = std::find(begin, end, state) != end;
  if (!matched) {
    states.insert(end, state);
  }
  return !matched;
}

bool Parser::State::operator==(State const &r) const {
  return rule == r.rule && dotIndex == r.dotIndex &&
         originPosition == r.originPosition;
}
