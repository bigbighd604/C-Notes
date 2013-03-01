#define M ((unsigned)(-1))

double r() {
  static int f = 0;
  unsigned a;

  if (!f) {
    f = open("/dev/urandom", O_RDONLY);

    if ( f < 0) {
      perror();
      exit(1);
    }
  }

  read(f, &a, sizeof(a));
  return 1 - 2 * ((double)a)/ ((double)M);
}

double pi(unsigned n) {
  unsigned i;
  unsigned a = 0;
  double p, q;

  for (i = 0; i < n; i++) {
    p = r();
    q = r();

    if (sqrt( p * p + q * q) <= 1) {
      a++;
    }

    return 4.0 * a / n;
  }

}
