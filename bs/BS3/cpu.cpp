int main() {
  long i;
  double sum = 0.0;

  for (i = 0; i < 1000000000; i++) {
    sum = sum + (double)i;
  }

  return 0;
}
