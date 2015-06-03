
// T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T
#define BOOST_TEST_MODULE filesystem_common_prefix
#include <boost/test/included/unit_test.hpp>
#include "filesystem/operation_common_prefix_tests.hpp"
// T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T T


BOOST_AUTO_TEST_CASE( test_case_remove_prefix_from_two_paths )
{
    test_remove_prefix_from_two_paths();
}

BOOST_AUTO_TEST_CASE( test_case_remove_common_prefix_from_several_paths )
{
    test_remove_common_prefix_from_several_paths();
}

BOOST_AUTO_TEST_CASE( test_case_common_prefix_from_two_paths )
{
    test_common_prefix_from_two_paths();
}

BOOST_AUTO_TEST_CASE( test_case_common_prefix_from_several_paths )
{
    test_common_prefix_from_several_paths();
}

BOOST_AUTO_TEST_CASE( test_case_common_prefix_from_initializer_list )
{
    test_common_prefix_from_initializer_list();
}

