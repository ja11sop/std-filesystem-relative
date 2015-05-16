// G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G
#ifndef FILESYSTEM_PATH_RELATIVE_TESTS_HPP_INCLUDED
#define FILESYSTEM_PATH_RELATIVE_TESTS_HPP_INCLUDED
// G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G

// I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I

// Under Test Include
#include "operations.hpp"

// Boost Library Includes
#include <boost/filesystem.hpp>

// C++ Standard Library Includes
// None

// I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I I


using path_t = boost::filesystem::path;


void test_relative( const path_t& Path, const path_t& Start )
{
    BOOST_TEST_MESSAGE( "--------------------------------------------------------" );
    BOOST_TEST_MESSAGE( "Path       = " << Path );
    BOOST_TEST_MESSAGE( "Start      = " << Start );

    auto Relative = relative( Path, Start );

    BOOST_TEST_MESSAGE( "Relative   = " << Relative );

    if( exists( Path ) && exists( Start ) )
    {
        BOOST_TEST_MESSAGE( "Real Path  = " << canonical( Path ) );
        BOOST_TEST_MESSAGE( "Real Start = " << canonical( Start ) );

        BOOST_CHECK( Relative.empty() != equivalent( Start/Relative, Path ) );
    }
    else
    {
        auto RealPath   = Path;
        auto RealStart  = Start;
        auto CommonPath = remove_common_prefix( RealPath, RealStart );

        CommonPath = exists( CommonPath ) ? canonical( CommonPath ) : normalize( CommonPath );
        RealPath   = exists( Path )       ? canonical( Path )       : CommonPath/normalize( RealPath );
        RealStart  = exists( Start )      ? canonical( Start )      : CommonPath/normalize( RealStart );

        BOOST_TEST_MESSAGE( "CommonPath = " << CommonPath );
        BOOST_TEST_MESSAGE( "Real Path  = " << RealPath );
        BOOST_TEST_MESSAGE( "Real Start = " << RealStart );

        auto Normalized = normalize( RealStart/Relative );

        BOOST_TEST_MESSAGE( "Normalized = " << Normalized );

        BOOST_CHECK( Relative.empty() != ( Normalized == RealPath ) );
    }
}


void test_real_relative_paths()
{
    auto Base = boost::filesystem::current_path();

    auto test_base = Base / "test_level_0";

    auto a_level_1 = test_base / "a_level_1";
    auto a_level_2 = a_level_1 / "a_level_2";
    auto a_level_3 = a_level_2 / "a_level_3";
    auto a_level_4 = a_level_3 / "a_level_4";
    auto a_level_5 = a_level_4 / "a_level_5";

    create_directories( a_level_5 );

    auto b_level_1 = test_base / "b_level_1";
    auto b_level_2 = b_level_1 / "b_level_2";
    auto b_level_3 = b_level_2 / "b_level_3";
    auto b_level_4 = b_level_3 / "b_level_4";
    auto b_level_5 = b_level_4 / "b_level_5";

    create_directories( b_level_5 );

    auto c_level_1 = test_base / "c_level_1";
    auto c_level_2 = c_level_1 / "c_level_2";

    create_directories( c_level_2 );

    auto c_level_3 = c_level_2 / "c_level_3";

    create_directory_symlink( b_level_3, c_level_3 );

    auto c_level_4 = c_level_3 / "c_level_4";

    create_directories( c_level_4 );

    auto c_level_5 = c_level_4 / "c_level_5";

    create_directory_symlink( a_level_1, c_level_5 );

    test_relative( test_base, test_base );

    test_relative( a_level_1, test_base );
    test_relative( a_level_2, test_base );
    test_relative( a_level_3, test_base );
    test_relative( a_level_4, test_base );
    test_relative( a_level_5, test_base );

    test_relative( test_base, a_level_1 );
    test_relative( test_base, a_level_2 );
    test_relative( test_base, a_level_3 );
    test_relative( test_base, a_level_4 );
    test_relative( test_base, a_level_5 );

    test_relative( a_level_1, b_level_1 );
    test_relative( a_level_1, b_level_2 );
    test_relative( a_level_1, b_level_3 );
    test_relative( a_level_1, b_level_4 );
    test_relative( a_level_1, b_level_5 );

    test_relative( a_level_1, b_level_1 );
    test_relative( a_level_2, b_level_1 );
    test_relative( a_level_3, b_level_1 );
    test_relative( a_level_4, b_level_1 );
    test_relative( a_level_5, b_level_1 );

    test_relative( a_level_1, b_level_1 );
    test_relative( a_level_2, b_level_2 );
    test_relative( a_level_3, b_level_3 );
    test_relative( a_level_4, b_level_4 );
    test_relative( a_level_5, b_level_5 );

    test_relative( a_level_2, b_level_4 );
    test_relative( a_level_3, b_level_3 );
    test_relative( a_level_4, b_level_2 );

    test_relative( c_level_1, test_base );
    test_relative( c_level_2, test_base );
    test_relative( c_level_3, test_base );
    test_relative( c_level_4, test_base );
    test_relative( c_level_5, test_base );

    test_relative( test_base, c_level_1 );
    test_relative( test_base, c_level_2 );
    test_relative( test_base, c_level_3 );
    test_relative( test_base, c_level_4 );
    test_relative( test_base, c_level_5 );

    test_relative( c_level_1, b_level_1 );
    test_relative( c_level_1, b_level_2 );
    test_relative( c_level_1, b_level_3 );
    test_relative( c_level_1, b_level_4 );
    test_relative( c_level_1, b_level_5 );

    test_relative( c_level_1, b_level_1 );
    test_relative( c_level_2, b_level_1 );
    test_relative( c_level_3, b_level_1 );
    test_relative( c_level_4, b_level_1 );
    test_relative( c_level_5, b_level_1 );

    test_relative( c_level_1, b_level_1 );
    test_relative( c_level_2, b_level_2 );
    test_relative( c_level_3, b_level_3 );
    test_relative( c_level_4, b_level_4 );
    test_relative( c_level_5, b_level_5 );

    test_relative( c_level_2, b_level_4 );
    test_relative( c_level_3, b_level_3 );
    test_relative( c_level_4, b_level_2 );

    test_relative( c_level_1, a_level_1 );
    test_relative( c_level_1, a_level_2 );
    test_relative( c_level_1, a_level_3 );
    test_relative( c_level_1, a_level_4 );
    test_relative( c_level_1, a_level_5 );

    test_relative( c_level_1, a_level_1 );
    test_relative( c_level_2, a_level_1 );
    test_relative( c_level_3, a_level_1 );
    test_relative( c_level_4, a_level_1 );
    test_relative( c_level_5, a_level_1 );

    test_relative( c_level_1, a_level_1 );
    test_relative( c_level_2, a_level_2 );
    test_relative( c_level_3, a_level_3 );
    test_relative( c_level_4, a_level_4 );
    test_relative( c_level_5, a_level_5 );

    test_relative( c_level_2, a_level_4 );
    test_relative( c_level_3, a_level_3 );
    test_relative( c_level_4, a_level_2 );

    remove_all( test_base );
}


void test_imaginary_relative_paths()
{
    auto test_base = path_t( "/imaginary_root" ) / "test_level_0";

    auto a_level_1 = test_base / "a_level_1";
    auto a_level_2 = a_level_1 / "a_level_2";
    auto a_level_3 = a_level_2 / "a_level_3";
    auto a_level_4 = a_level_3 / "a_level_4";
    auto a_level_5 = a_level_4 / "a_level_5";

    auto b_level_1 = test_base / "b_level_1";
    auto b_level_2 = b_level_1 / "b_level_2";
    auto b_level_3 = b_level_2 / "b_level_3";
    auto b_level_4 = b_level_3 / "b_level_4";
    auto b_level_5 = b_level_4 / "b_level_5";

    test_relative( test_base, test_base );

    test_relative( a_level_1, test_base );
    test_relative( a_level_2, test_base );
    test_relative( a_level_3, test_base );
    test_relative( a_level_4, test_base );
    test_relative( a_level_5, test_base );

    test_relative( test_base, a_level_1 );
    test_relative( test_base, a_level_2 );
    test_relative( test_base, a_level_3 );
    test_relative( test_base, a_level_4 );
    test_relative( test_base, a_level_5 );

    test_relative( a_level_1, b_level_1 );
    test_relative( a_level_1, b_level_2 );
    test_relative( a_level_1, b_level_3 );
    test_relative( a_level_1, b_level_4 );
    test_relative( a_level_1, b_level_5 );

    test_relative( a_level_1, b_level_1 );
    test_relative( a_level_2, b_level_1 );
    test_relative( a_level_3, b_level_1 );
    test_relative( a_level_4, b_level_1 );
    test_relative( a_level_5, b_level_1 );

    test_relative( a_level_1, b_level_1 );
    test_relative( a_level_2, b_level_2 );
    test_relative( a_level_3, b_level_3 );
    test_relative( a_level_4, b_level_4 );
    test_relative( a_level_5, b_level_5 );

    test_relative( a_level_2, b_level_4 );
    test_relative( a_level_3, b_level_3 );
    test_relative( a_level_4, b_level_2 );
}


void test_real_and_imaginary_relative_paths()
{
    auto Base = boost::filesystem::current_path();

    auto test_base = Base / "test_level_0";

    auto a_level_1 = test_base / "a_level_1";
    auto a_level_2 = a_level_1 / "a_level_2";
    auto a_level_3 = a_level_2 / "a_level_3";
    auto a_level_4 = a_level_3 / "a_level_4";
    auto a_level_5 = a_level_4 / "a_level_5";

    create_directories( a_level_5 );

    auto b_level_1 = test_base / "_b_level_1";
    auto b_level_2 = b_level_1 / "_b_level_2";
    auto b_level_3 = b_level_2 / "_b_level_3";
    auto b_level_4 = b_level_3 / "_b_level_4";
    auto b_level_5 = b_level_4 / "_b_level_5";

    test_relative( a_level_1, test_base );
    test_relative( a_level_2, test_base );
    test_relative( a_level_3, test_base );
    test_relative( a_level_4, test_base );
    test_relative( a_level_5, test_base );

    test_relative( test_base, a_level_1 );
    test_relative( test_base, a_level_2 );
    test_relative( test_base, a_level_3 );
    test_relative( test_base, a_level_4 );
    test_relative( test_base, a_level_5 );

    test_relative( a_level_1, b_level_1 );
    test_relative( a_level_1, b_level_2 );
    test_relative( a_level_1, b_level_3 );
    test_relative( a_level_1, b_level_4 );
    test_relative( a_level_1, b_level_5 );

    test_relative( a_level_1, b_level_1 );
    test_relative( a_level_2, b_level_1 );
    test_relative( a_level_3, b_level_1 );
    test_relative( a_level_4, b_level_1 );
    test_relative( a_level_5, b_level_1 );

    test_relative( a_level_1, b_level_1 );
    test_relative( a_level_2, b_level_2 );
    test_relative( a_level_3, b_level_3 );
    test_relative( a_level_4, b_level_4 );
    test_relative( a_level_5, b_level_5 );

    test_relative( a_level_2, b_level_4 );
    test_relative( a_level_3, b_level_3 );
    test_relative( a_level_4, b_level_2 );

    auto c_level_1 = test_base / "c_level_1";
    auto c_level_2 = c_level_1 / "c_level_2";

    create_directories( c_level_2 );

    auto c_level_3 = c_level_2 / "c_level_3";

    create_directory_symlink( a_level_3, c_level_3 );

    auto c_level_4 = c_level_3 / "c_level_4";
    auto c_level_5 = c_level_4 / "c_level_5";

    create_directories( c_level_5 );

    test_relative( c_level_1, b_level_1 );
    test_relative( c_level_1, b_level_2 );
    test_relative( c_level_1, b_level_3 );
    test_relative( c_level_1, b_level_4 );
    test_relative( c_level_1, b_level_5 );

    test_relative( c_level_1, b_level_1 );
    test_relative( c_level_2, b_level_1 );
    test_relative( c_level_3, b_level_1 );
    test_relative( c_level_4, b_level_1 );
    test_relative( c_level_5, b_level_1 );

    test_relative( c_level_1, b_level_1 );
    test_relative( c_level_2, b_level_2 );
    test_relative( c_level_3, b_level_3 );
    test_relative( c_level_4, b_level_4 );
    test_relative( c_level_5, b_level_5 );

    test_relative( c_level_2, b_level_4 );
    test_relative( c_level_3, b_level_3 );
    test_relative( c_level_4, b_level_2 );

    remove_all( test_base );
}


void test_real_and_imaginary_relative_paths_with_parent_and_current_directories()
{
    auto Base = boost::filesystem::current_path();

    auto test_base = Base / "test_level_0";

    auto a_level_1 = test_base / "a_level_1";
    auto a_level_2 = a_level_1 / "a_level_2";
    auto a_level_3 = a_level_2 / "..";
    auto a_level_4 = a_level_3 / "a_level_4";
    auto a_level_5 = a_level_4 / "a_level_5";

    create_directories( a_level_5 );

    auto b_level_1 = test_base / "_b_level_1";
    auto b_level_2 = b_level_1 / "_b_level_2";
    auto b_level_3 = b_level_2 / "..";
    auto b_level_4 = b_level_3 / "_b_level_4";
    auto b_level_5 = b_level_4 / "_b_level_5";

    test_relative( a_level_1, test_base );
    test_relative( a_level_2, test_base );
    test_relative( a_level_3, test_base );
    test_relative( a_level_4, test_base );
    test_relative( a_level_5, test_base );

    test_relative( test_base, a_level_1 );
    test_relative( test_base, a_level_2 );
    test_relative( test_base, a_level_3 );
    test_relative( test_base, a_level_4 );
    test_relative( test_base, a_level_5 );

    test_relative( a_level_1, b_level_1 );
    test_relative( a_level_1, b_level_2 );
    test_relative( a_level_1, b_level_3 );
    test_relative( a_level_1, b_level_4 );
    test_relative( a_level_1, b_level_5 );

    test_relative( a_level_1, b_level_1 );
    test_relative( a_level_2, b_level_1 );
    test_relative( a_level_3, b_level_1 );
    test_relative( a_level_4, b_level_1 );
    test_relative( a_level_5, b_level_1 );

    test_relative( a_level_1, b_level_1 );
    test_relative( a_level_2, b_level_2 );
    test_relative( a_level_3, b_level_3 );
    test_relative( a_level_4, b_level_4 );
    test_relative( a_level_5, b_level_5 );

    test_relative( a_level_2, b_level_4 );
    test_relative( a_level_3, b_level_3 );
    test_relative( a_level_4, b_level_2 );

    auto c_level_1 = test_base / "c_level_1";
    auto c_level_2 = c_level_1 / "c_level_2";

    create_directories( c_level_2 );

    auto c_level_3 = c_level_2 / "c_level_3";

    create_directory_symlink( a_level_3, c_level_3 );

    auto c_level_4 = c_level_3 / "..";
    auto c_level_5 = c_level_4 / "c_level_5";

    create_directories( c_level_5 );

    test_relative( c_level_1, b_level_1 );
    test_relative( c_level_1, b_level_2 );
    test_relative( c_level_1, b_level_3 );
    test_relative( c_level_1, b_level_4 );
    test_relative( c_level_1, b_level_5 );

    test_relative( c_level_1, b_level_1 );
    test_relative( c_level_2, b_level_1 );
    test_relative( c_level_3, b_level_1 );
    test_relative( c_level_4, b_level_1 );
    test_relative( c_level_5, b_level_1 );

    test_relative( c_level_1, b_level_1 );
    test_relative( c_level_2, b_level_2 );
    test_relative( c_level_3, b_level_3 );
    test_relative( c_level_4, b_level_4 );
    test_relative( c_level_5, b_level_5 );

    test_relative( c_level_2, b_level_4 );
    test_relative( c_level_3, b_level_3 );
    test_relative( c_level_4, b_level_2 );

    remove_all( test_base );
}


void test_non_relative_paths()
{
    auto test_base_1 = path_t( "//root_x/imaginary_root_1" );
    auto test_base_2 = path_t( "//root_y/imaginary_root_2" );

    auto a_level_1 = test_base_1 / "a_level_1";
    auto a_level_2 = a_level_1 / "a_level_2";

    auto b_level_1 = test_base_2 / "b_level_1";
    auto b_level_2 = b_level_1 / "b_level_2";

    test_relative( a_level_1, test_base_2 );
    test_relative( a_level_2, test_base_2 );

    test_relative( test_base_2, a_level_1 );
    test_relative( test_base_2, a_level_2 );

    test_relative( b_level_1, test_base_1 );
    test_relative( b_level_2, test_base_1 );

    test_relative( test_base_1, b_level_1 );
    test_relative( test_base_1, b_level_2 );

    test_relative( a_level_1, b_level_1 );
    test_relative( a_level_1, b_level_2 );

    test_relative( a_level_1, b_level_1 );
    test_relative( a_level_2, b_level_1 );

    test_relative( a_level_1, b_level_1 );
    test_relative( a_level_2, b_level_2 );
}


void test_empty_path_return()
{
    auto base_0 = boost::filesystem::current_path() / "level_0";

    auto base_1 = path_t( "//root_x/imaginary_level_0" );
    auto base_2 = path_t( "//root_y/imaginary_level_0" );

    auto path_1 = base_0 / "level_1";
    auto path_2 = base_1 / "level_1";

    auto rel_path = relative( path_1, path_2 );

    BOOST_CHECK( rel_path.empty() );
}

// G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G
#endif//FILESYSTEM_PATH_RELATIVE_TESTS_HPP_INCLUDED
