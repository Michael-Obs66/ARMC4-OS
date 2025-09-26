#ifndef ASSERT_H
#define ASSERT_H

void assert_failed(const char *file, int line, const char *expr);

#define assert(expr) ((expr) ? (void)0 : assert_failed(__FILE__, __LINE__, #expr))

#endif
