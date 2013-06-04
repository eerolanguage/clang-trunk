// RUN: %clang_cc1 -triple x86_64-apple-darwin -fsyntax-only -Wtautological-constant-out-of-range-compare -verify %s 
// rdar://12202422

int value(void);

int main()
{
    int a = value();
    if (a == 0x1234567812345678L) // expected-warning {{comparison of constant 1311768465173141112 with expression of type 'int' is always false}}
        return 0;
    if (a != 0x1234567812345678L) // expected-warning {{comparison of constant 1311768465173141112 with expression of type 'int' is always true}}
        return 0;
    if (a < 0x1234567812345678L)  // expected-warning {{comparison of constant 1311768465173141112 with expression of type 'int' is always true}}
        return 0;
    if (a <= 0x1234567812345678L)  // expected-warning {{comparison of constant 1311768465173141112 with expression of type 'int' is always true}}
        return 0;
    if (a > 0x1234567812345678L)  // expected-warning {{comparison of constant 1311768465173141112 with expression of type 'int' is always false}}
        return 0;
    if (a >= 0x1234567812345678L)  // expected-warning {{comparison of constant 1311768465173141112 with expression of type 'int' is always false}}
        return 0;

    if (0x1234567812345678L == a) // expected-warning {{comparison of constant 1311768465173141112 with expression of type 'int' is always false}}
        return 0;
    if (0x1234567812345678L != a) // expected-warning {{comparison of constant 1311768465173141112 with expression of type 'int' is always true}}
        return 0;
    if (0x1234567812345678L < a)  // expected-warning {{comparison of constant 1311768465173141112 with expression of type 'int' is always false}}
        return 0;
    if (0x1234567812345678L <= a)  // expected-warning {{comparison of constant 1311768465173141112 with expression of type 'int' is always false}}
        return 0;
    if (0x1234567812345678L > a) // expected-warning {{comparison of constant 1311768465173141112 with expression of type 'int' is always true}}
        return 0;
    if (0x1234567812345678L >= a) // expected-warning {{comparison of constant 1311768465173141112 with expression of type 'int' is always true}}
        return 0;
    if (a == 0x1234567812345678LL) // expected-warning {{comparison of constant 1311768465173141112 with expression of type 'int' is always false}}
      return 0;
    if (a == -0x1234567812345678L) // expected-warning {{comparison of constant -1311768465173141112 with expression of type 'int' is always false}}
      return 0;
    if (a < -0x1234567812345678L) // expected-warning {{comparison of constant -1311768465173141112 with expression of type 'int' is always false}}
      return 0;
    if (a > -0x1234567812345678L) // expected-warning {{comparison of constant -1311768465173141112 with expression of type 'int' is always true}}
      return 0;
    if (a <= -0x1234567812345678L) // expected-warning {{comparison of constant -1311768465173141112 with expression of type 'int' is always false}}
      return 0;
    if (a >= -0x1234567812345678L) // expected-warning {{comparison of constant -1311768465173141112 with expression of type 'int' is always true}}
      return 0;


    if (a == 0x12345678L) // no warning
      return 1;

    short s = value();
    if (s == 0x1234567812345678L) // expected-warning {{comparison of constant 1311768465173141112 with expression of type 'short' is always false}}
        return 0;
    if (s != 0x1234567812345678L) // expected-warning {{comparison of constant 1311768465173141112 with expression of type 'short' is always true}}
        return 0;
    if (s < 0x1234567812345678L) // expected-warning {{comparison of constant 1311768465173141112 with expression of type 'short' is always true}}
        return 0;
    if (s <= 0x1234567812345678L) // expected-warning {{comparison of constant 1311768465173141112 with expression of type 'short' is always true}}
        return 0;
    if (s > 0x1234567812345678L) // expected-warning {{comparison of constant 1311768465173141112 with expression of type 'short' is always false}}
        return 0;
    if (s >= 0x1234567812345678L) // expected-warning {{comparison of constant 1311768465173141112 with expression of type 'short' is always false}}
        return 0;

    if (0x1234567812345678L == s) // expected-warning {{comparison of constant 1311768465173141112 with expression of type 'short' is always false}}
        return 0;
    if (0x1234567812345678L != s) // expected-warning {{comparison of constant 1311768465173141112 with expression of type 'short' is always true}}
        return 0;
    if (0x1234567812345678L < s) // expected-warning {{comparison of constant 1311768465173141112 with expression of type 'short' is always false}}
        return 0;
    if (0x1234567812345678L <= s) // expected-warning {{comparison of constant 1311768465173141112 with expression of type 'short' is always false}}
        return 0;
    if (0x1234567812345678L > s) // expected-warning {{comparison of constant 1311768465173141112 with expression of type 'short' is always true}}
        return 0;
    if (0x1234567812345678L >= s) // expected-warning {{comparison of constant 1311768465173141112 with expression of type 'short' is always true}}
        return 0;
    long l = value();
    if (l == 0x1234567812345678L)
        return 0;
    if (l != 0x1234567812345678L)
        return 0;
    if (l < 0x1234567812345678L)
        return 0;
    if (l <= 0x1234567812345678L)
        return 0;
    if (l > 0x1234567812345678L)
        return 0;
    if (l >= 0x1234567812345678L)
        return 0;

    if (0x1234567812345678L == l)
        return 0;
    if (0x1234567812345678L != l)
        return 0;
    if (0x1234567812345678L < l)
        return 0;
    if (0x1234567812345678L <= l)
        return 0;
    if (0x1234567812345678L > l)
        return 0;
    if (0x1234567812345678L >= l)
        return 0;

    unsigned un = 0;
    if (un == 0x0000000000000000L)
        return 0;
    if (un != 0x0000000000000000L)
        return 0;
    if (un < 0x0000000000000000L)
        return 0;
    if (un <= 0x0000000000000000L)
        return 0;
    if (un > 0x0000000000000000L)
        return 0;
    if (un >= 0x0000000000000000L)
        return 0;

    if (0x0000000000000000L == un)
        return 0;
    if (0x0000000000000000L != un)
        return 0;
    if (0x0000000000000000L < un)
        return 0;
    if (0x0000000000000000L <= un)
        return 0;
    if (0x0000000000000000L > un)
        return 0;
    if (0x0000000000000000L >= un)
        return 0;
    float fl = 0;
    if (fl == 0x0000000000000000L) // no warning
      return 0;

    float dl = 0;
    if (dl == 0x0000000000000000L) // no warning
      return 0;

    enum E {
    yes,
    no, 
    maybe
    };
    enum E e;

    if (e == 0x1234567812345678L) // expected-warning {{comparison of constant 1311768465173141112 with expression of type 'enum E' is always false}}
      return 0;

    return 1;
}

typedef enum {
    alpha=0, bravo, charlie, delta, echo
} named_t;

static int bar(named_t foo) {
    return foo > 42;
}
