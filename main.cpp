#include "ULL.hpp"

int main()
{
  ULL<int> ull;
  for (int i = 1; i <= 10; ++i) {
     ull.append(i);
  }

  ull.insert_at(1, 11);
  ull.insert_at(2, 12);
  ull.insert_at(4, 13);

  auto* v = ull.get(1);
  std::cout << "v: " << *v << std::endl;

  ull.print_list();

  return 0;
}
