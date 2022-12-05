#include <iostream>
#include <vector>

using namespace std;

size_t binsearch(const vector<int> &a, int s, int li, int re) {
  if (re < li) {
    return -1;
  }
  int mi = li + (re - li) / 2;
  if (s == a[mi]) {
    return mi;
  } else if (s < a[mi]) {
    return binsearch(a, s, li, mi - 1);
  } else { // s > a[mi]
    return binsearch(a, s, mi+1, re);
  }
}

int main() {
  int n = 10;
  vector<int> array {2,3,5,7,11,13,17,19,23,29};
  
  cout << "Zahlen im Array:";
  for(int i : array) {
	  cout << " " << i;
  }
  cout << endl;

  while (true) {
    int zahl = 0;
    cout << "Welche Zahl soll gesucht werden?" << endl;
    cin >> zahl;

    int stelle = binsearch(array, zahl, 0, array.size() - 1);
    if (stelle >= 0) {
      cout << "Zahl " << zahl << " an Stelle " << stelle << " gefunden."
           << endl;
    } else {
      cout << "Zahl " << zahl << " nicht gefunden." << endl;
    }
  }
}
