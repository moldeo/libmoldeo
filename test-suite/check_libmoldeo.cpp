#include <../libmoldeo/moText.h>
#include <../libmoldeo/moResourceManager.h>
#include <../libmoldeo/moDataManager.h>
#include <../libmoldeo/moConsole.h>

#include "../config.h"
#include <check.h>

/*
 * Core test suite
 */
START_TEST(test_text)
{
  fail_unless(moText("hola") == moText("hola"));
  fail_unless(moText("adios") != moText("hola"));
}
END_TEST

START_TEST(test_video_graph) {
  /* TODO: Not implemented */
}
END_TEST

START_TEST(test_data_session_config) {
  bool res;
  moDataSessionConfig *DataSessionConfig = new moDataSessionConfig(
		  moText("apppath"),
		  moText("datapath"),
		  moText("config.mol"),
		  moText("session_file_name"),
		  moText("video_file_name"),
		  0, 1, 2, 3);
  fail_unless(DataSessionConfig != NULL);
  fail_unless(DataSessionConfig->GetDataPath() == moText("datapath"));
  fail_unless(DataSessionConfig->GetConsoleConfigName() == moText("config.mol"));
  fail_unless(DataSessionConfig->GetSessionFileName() == moText("session_file_name"));
  fail_unless(DataSessionConfig->GetVideoFileName() == moText("video_file_name"));
  /* TODO: Not implemented
	m_MaxKeys
	m_MaxTimecode
	m_Port
	m_Address
   */
  delete DataSessionConfig;
}
END_TEST

START_TEST(test_data_session) {
  bool res;
  moDataSession *DataSession = new moDataSession();
  fail_unless(DataSession != NULL);
  /*res = DataSession->Init(moText("my_name"),
		  NULL,
		  );*/
  delete DataSession;
}
END_TEST

START_TEST(test_data_manager) {
  bool res;
  moDataManager *DataManager = new moDataManager();
  fail_unless(DataManager != NULL);
  res = DataManager->Init( moText(""),moText("data"), moText("config.mol"));
  fail_unless(res != true);
  fail_unless(DataManager->GetDataPath() == moText("data"));
  fail_unless(DataManager->GetConsoleConfigName() == moText("config.mol"));
  delete DataManager;
}
END_TEST

START_TEST(test_resource_manager) {
  bool res;
  moResourceManager *ResourceManager = new moResourceManager();
  fail_unless(ResourceManager != NULL);
  delete ResourceManager;
}
END_TEST

START_TEST(test_console) {
  bool res;
  moConsole *MoldeoSession;
  MoldeoSession = new moConsole();
  fail_unless(MoldeoSession != NULL);
  delete MoldeoSession;
}
END_TEST

/*
 * Prepare test suite
 */
Suite *
libmoldeo_suite(void)
{
  Suite *s = suite_create("libmoldeo");
  TCase *tc = tcase_create("core");
  tcase_add_test(tc, test_text);
  tcase_add_test(tc, test_data_session_config);
  suite_add_tcase(s, tc);
  return s;
}

/*
 * Main
 */
int
main( int argc, char *argv[] )
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

