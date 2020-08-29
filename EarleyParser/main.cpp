#include "Earley.h"

int main(int argc, char *argv[]) {
  Rule p{"P"};
  p >> "S";

  Rule s1{"S"};
  s1 >> "S" >> '+' >> "M";
  Rule s2{"S"};
  s2 >> "M";

  Rule m1{"M"};
  m1 >> "M" >> '*' >> "T";
  Rule m2{"M"};
  m2 >> "T";

  Rule t1{"T"};
  t1 >> '1';
  Rule t2{"T"};
  t2 >> '2';
  Rule t3{"T"};
  t3 >> '3';
  Rule t4{"T"};
  t4 >> '4';

  return 0;
}
