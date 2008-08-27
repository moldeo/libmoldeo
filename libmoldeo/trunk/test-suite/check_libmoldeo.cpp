#include <../libmoldeo/moText.h>

#include "../config.h"
#include <check.h>

/*
 * Core test suite
 */
START_TEST(test_core)
{
  moText0 text("hola mundo!");
  fail_unless(1==1, "core test suite");
}
END_TEST

Suite *
libmoldeo_suite(void)
{
  Suite *s = suite_create("libmoldeo");
  TCase *tc = tcase_create("core");
  tcase_add_test(tc, test_core);
  suite_add_tcase(s, tc);
  return s;
}

/*
 * Main
 */
int
main()
{
  int nf;
  Suite *s = libmoldeo_suite();
  SRunner *sr = srunner_create(s);
  srunner_set_log(sr, "check_libmoldeo.log");
  srunner_run_all(sr, CK_NORMAL);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (nf == 0) ? 0 : 1;
}

