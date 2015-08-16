
// T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T
#define BOOST_TEST_MODULE filesystem_relative
#include <boost/test/included/unit_test.hpp>
#include "filesystem/operation_relative_tests.hpp"
// T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T


BOOST_AUTO_TEST_CASE( test_case_real_relative_paths )
{
    test_real_relative_paths();
}

BOOST_AUTO_TEST_CASE( test_case_multiple_nested_symlinks )
{
    multiple_nested_symlinks();
}

BOOST_AUTO_TEST_CASE( test_case_imaginary_relative_paths )
{
    test_imaginary_relative_paths();
}

BOOST_AUTO_TEST_CASE( test_case_real_and_imaginary_relative_paths )
{
    test_real_and_imaginary_relative_paths();
}

BOOST_AUTO_TEST_CASE( test_case_real_and_imaginary_relative_paths_with_parent_and_current_directories )
{
    test_real_and_imaginary_relative_paths_with_parent_and_current_directories();
}

BOOST_AUTO_TEST_CASE( test_case_non_relative_paths )
{
    test_non_relative_paths();
}

BOOST_AUTO_TEST_CASE( test_case_empty_path_return )
{
    test_empty_path_return();
}

BOOST_AUTO_TEST_CASE( test_case_same_paths_imaginary )
{
    test_same_paths_imaginary();
}

BOOST_AUTO_TEST_CASE( test_case_same_paths )
{
    test_same_paths();
}

BOOST_AUTO_TEST_CASE( test_case_paper_paths )
{
    test_paper_paths();
}

BOOST_AUTO_TEST_CASE( test_check_semantics )
{
    check_semantics();
}

