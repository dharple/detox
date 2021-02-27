#include <stdio.h>
#include <check.h>

#include "../../src/clean_string.h"

START_TEST(test_clean_string_lower)
{
    char *output;

    fprintf(stderr, "in\n");

    // legacy tests
    output = clean_lower("lower", NULL);
    ck_assert_str_eq(output, "lower");
    free(output);

    output = clean_lower("L0W3R", NULL);
    ck_assert_str_eq(output, "l0w3r");
    free(output);

    output = clean_lower("UPPER", NULL);
    ck_assert_str_eq(output, "upper");
    free(output);

    output = clean_lower("UPPer_2", NULL);
    ck_assert_str_eq(output, "upper_2");
    free(output);

    // confirm NULL works
    output = clean_lower(NULL, NULL);
    ck_assert(output == NULL);

    fprintf(stderr, "out\n");
}
END_TEST

Suite *detox_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("detox");

    tc_core = tcase_create("clean_string");

    tcase_add_test(tc_core, test_clean_string_lower);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = detox_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_ENV);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
