#include <iostream>
#include "ULL.hpp"

using namespace std;

int main()
{
  ULL<int> ull;
  ull.append(42);
  ull.append(100);

  cout << ull.get_node_size() << endl;

  return 0;
}
