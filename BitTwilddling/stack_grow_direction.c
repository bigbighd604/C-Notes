
int a(int *p0) {
  int p;
  if (p0) return p0 > &p;
  return a(&p);
}

int main() {
  puts( a(0) ? "y": "n");
}
