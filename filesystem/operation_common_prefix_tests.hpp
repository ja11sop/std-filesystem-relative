// G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G
#ifndef FILESYSTEM_OPERATION_COMMON_PREFIX_TESTS_HPP_INCLUDED
#define FILESYSTEM_OPERATION_COMMON_PREFIX_TESTS_HPP_INCLUDED
// G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G

// I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I

// Filesystem Includes
#include "filesystem/operations.hpp"
#include "filesystem/path.hpp"

// Boost Library Includes
#include <boost/filesystem.hpp>

// C++ Standard Library Includes
// None

// I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I


using path_t = xstd::filesystem::path;


void test_remove_prefix_from_two_paths()
{
    path_t Common = "/a/b/c/d";

    path_t RelPath1 = "e/f/g/h";
    path_t RelPath2 = "j/k";

    path_t Path1 = Common / RelPath1;
    path_t Path2 = Common / RelPath2;

    auto Prefix = remove_common_prefix( Path1, Path2 );

    BOOST_CHECK( Path1  == RelPath1 );
    BOOST_CHECK( Path2  == RelPath2 );
    BOOST_CHECK( Prefix == Common );
}


void test_remove_common_prefix_from_several_paths()
{
    path_t Common = "/a/b/c";

    std::vector<path_t> RelPaths = { "d/e/f/g/h", "d/j/k", "p/q/r" };
    std::vector<path_t> Paths;

    for( const auto& Path: RelPaths )
    {
        Paths.push_back( Common / Path );
    }

    auto Prefix = boost::filesystem::remove_common_prefix( Paths.begin(), Paths.end(), Paths.begin() );

    BOOST_CHECK( Prefix == Common );

    auto Path    = Paths.cbegin();
    auto RelPath = RelPaths.cbegin();

    for( ; Path != Paths.end(); ++Path, ++RelPath )
    {
        BOOST_TEST_MESSAGE( "Path = [" << *Path << "], RelPath = [" << *RelPath << "]" );
        BOOST_CHECK( *Path == *RelPath );
    }
}


void test_common_prefix_from_several_paths()
{
    path_t Common = "/a/b/c";

    std::vector<path_t> RelPaths = { "d/e/f/g/h", "d/j/k", "p/q/r" };
    std::vector<path_t> Paths;

    for( const auto& Path: RelPaths )
    {
        Paths.push_back( Common / Path );
    }
    auto Prefix = boost::filesystem::common_prefix( Paths.begin(), Paths.end() );

    BOOST_CHECK( Prefix == Common );
}


void test_common_prefix_from_initializer_list()
{
    path_t Common = "/a/b/c";

    path_t Path1 = Common / "d/e/f/g/h";
    path_t Path2 = Common / "d/j/k";
    path_t Path3 = Common / "p/q/r";

    auto Prefix = boost::filesystem::common_prefix( { Path1, Path2, Path3 } );

    BOOST_CHECK( Prefix == Common );
}


// G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G
#endif//FILESYSTEM_OPERATION_COMMON_PREFIX_TESTS_HPP_INCLUDED
