#include "Earley.h"

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

  return 0;
}
