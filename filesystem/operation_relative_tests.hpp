// G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G
#ifndef FILESYSTEM_PATH_RELATIVE_TESTS_HPP_INCLUDED
#define FILESYSTEM_PATH_RELATIVE_TESTS_HPP_INCLUDED
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


using path_t = boost::filesystem::path_t;


void test_relative( const path_t& Path, const path_t& Start )
{
    BOOST_TEST_MESSAGE( "--------------------------------------------------------" );
    BOOST_TEST_MESSAGE( "From (start)    = " << Start );
    BOOST_TEST_MESSAGE( "To   (path)     = " << Path );

    auto Relative  = relative( Path, Start );
    auto Proximate = !Relative.empty() ? Relative : Path;

    BOOST_TEST_MESSAGE( "Relative        = " << Relative );
    BOOST_TEST_MESSAGE( "Proximate       = " << Proximate );

    auto LexicallyRelative = lexically_relative( Path, Start );

    BOOST_TEST_MESSAGE( "Lexically Rel   = " << LexicallyRelative );


    if( exists( Path ) && exists( Start ) )
    {
        BOOST_TEST_MESSAGE( "Real Path       = " << canonical( Path ) );
        BOOST_TEST_MESSAGE( "Real Start      = " << canonical( Start ) );

        BOOST_CHECK( Relative.empty() != equivalent( Start/Relative, Path ) );
        BOOST_TEST_MESSAGE( "equivalent      = " << equivalent( Start/Relative, Path ) );
        BOOST_TEST_MESSAGE( "equivalent(lex) = " << equivalent( Start/LexicallyRelative, Path ) );
        auto Same = equivalent( Start/Relative, Path ) == equivalent( Start/LexicallyRelative, Path );
        BOOST_TEST_MESSAGE( "rel==lex_rel?   = " << ( Same ? "Same" : "NOT Same!" ) );
    }
    else
    {
        auto RealPath   = Path;
        auto RealStart  = Start;
        auto CommonPath = remove_common_prefix( RealPath, RealStart );

        CommonPath = exists( CommonPath ) ? canonical( CommonPath ) : normalize( CommonPath );
        RealPath   = exists( Path )       ? canonical( Path )       : CommonPath/normalize( RealPath );
        RealStart  = exists( Start )      ? canonical( Start )      : CommonPath/normalize( RealStart );

        BOOST_TEST_MESSAGE( "CommonPath      = " << CommonPath );
        BOOST_TEST_MESSAGE( "Real Path       = " << RealPath );
        BOOST_TEST_MESSAGE( "Real Start      = " << RealStart );

        auto Normalized = normalize( RealStart/Relative );

        BOOST_TEST_MESSAGE( "Normalized      = " << Normalized );

        BOOST_CHECK( Relative.empty() != ( Normalized == RealPath ) );
    }
}


void test_real_relative_paths()
{
    path_t Base = boost::filesystem::current_path();

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


void multiple_nested_symlinks()
{
//     a / b / c / testfile
//     a / d / e --> ../../a/b
//     m / n --> /a
//     x / y / z --> /m/n/d
//
//     If we start in "y", we will see the following directory structure:
//     y
//     |-- z
//     |-- e
//     |-- c
//     |-- testfile

    path_t Base = boost::filesystem::current_path();

    auto test_base = Base / "test_level_0";

    auto dir_a = test_base / "dir_a";
    auto dir_b = dir_a / "dir_b";
    auto dir_c = dir_b / "dir_c";

    create_directories( dir_c );

    auto testdir = dir_c / "testdir";

    create_directories( testdir );

    auto dir_d = dir_a / "dir_d";

    create_directories( dir_d );

    auto dir_e = dir_d/ "dir_e";

    create_directory_symlink( "../../dir_a/dir_b", dir_e );

    auto dir_m = test_base / "dir_m";

    create_directories( dir_m );

    auto dir_n = dir_m / "dir_n";

    create_directory_symlink( dir_a, dir_n );

    auto dir_x = test_base / "dir_x";
    auto dir_y = dir_x / "dir_y";

    create_directories( dir_y );

    auto dir_z = dir_y / "dir_z";

    create_directory_symlink( dir_m / "dir_n/dir_d", dir_z );

    test_relative( dir_x, testdir );
    test_relative( testdir, dir_x );

    test_relative( dir_y, testdir );
    test_relative( testdir, dir_y );

    test_relative( dir_z, testdir );
    test_relative( testdir, dir_z );

    test_relative( dir_m, testdir );
    test_relative( testdir, dir_m );

    test_relative( dir_n, testdir );
    test_relative( testdir, dir_n );

    test_relative( dir_d, testdir );
    test_relative( testdir, dir_d );

    test_relative( dir_e, testdir );
    test_relative( testdir, dir_e );

    test_relative( dir_d, dir_x );
    test_relative( dir_x, dir_d );

    test_relative( dir_d, dir_y );
    test_relative( dir_y, dir_d );

    test_relative( dir_e, dir_y );
    test_relative( dir_y, dir_e );

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
    path_t Base = boost::filesystem::current_path();

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
    path_t Base = boost::filesystem::current_path();

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
    path_t base_0 = boost::filesystem::current_path() / "level_0";

    auto base_1 = path_t( "//root_x/imaginary_level_0" );
    auto base_2 = path_t( "//root_y/imaginary_level_0" );

    auto path_1 = base_0 / "level_1";
    auto path_2 = base_1 / "level_1";

    auto rel_path = relative( path_1, path_2 );

    BOOST_CHECK( rel_path.empty() );
}


void test_same_paths_imaginary()
{
    auto base_0 = path_t( "//root_x/imaginary_level_0" );

    auto path_1 = base_0 / "level_1";
    auto path_2 = base_0 / "level_1";

    auto rel_path = relative( path_1, path_2 );

    auto dot = path_t(".");

    BOOST_CHECK( rel_path == dot );

    test_relative( path_1, path_2 );
}


void test_same_paths()
{
    path_t test_base = boost::filesystem::current_path() / "level_0";

    auto path_1 = test_base / "level_1";

    create_directories( path_1 );

    auto rel_path = relative( path_1, path_1 );

    auto dot = path_t(".");

    BOOST_CHECK( rel_path == dot );

    BOOST_CHECK( equivalent( path_1/dot, path_1 ) );
    BOOST_CHECK( equivalent( canonical( path_1/dot ), canonical( path_1 ) ) ) ;

    test_relative( path_1, path_1 );

    remove_all( test_base );
}


void test_paper_paths()
{
//        path         start    =    rel_path
//       ------       -------       ----------
//
//  1.   /a/d         /a/b/c          ../../d
//  2.   /a/b/c       /a/d            ../b/c
//  3.   //c_drive/y  //c_drive/x     ..\y
//  4.   //d_drive/y  //c_drive/x     //d_drive/y

    test_relative( "/a/d",        "/a/b/c" );
    test_relative( "/a/b/c",      "/a/d"   );
    test_relative( "//C_drive/y", "//C_drive/x" );
    test_relative( "//D_drive/y", "//C_drive/x" );
}


void check_semantics()
{
    // TODO Write this as proper tests
    auto Dot   = boost::filesystem::path(".");
    auto Empty = boost::filesystem::path();

    std::cout << "                dot = " << Dot   << std::endl;
    std::cout << "              empty = " << Empty << std::endl;

    std::cout << "    is_dir(  dot  ) : " << std::boolalpha << is_directory(Dot)    << std::endl;
    std::cout << "    is_dir( empty ) : " << std::boolalpha << is_directory(Empty)  << std::endl;

    std::cout << "    status(  dot  ) : " << status(Dot).type()   << std::endl;
    std::cout << "    status( empty ) : " << status(Empty).type() << std::endl;

    std::cout << "       status_error = " << boost::filesystem::status_error   << std::endl;
    std::cout << "       type_unknown = " << boost::filesystem::type_unknown   << std::endl;
    std::cout << "     directory_file = " << boost::filesystem::directory_file << std::endl;
    std::cout << "     file_not_found = " << boost::filesystem::file_not_found << std::endl;

    std::cout << "     current_path() : " << boost::filesystem::current_path() << std::endl;

    current_path(Dot);
    std::cout << "  current_path(dot) : " << boost::filesystem::current_path() << std::endl;

    current_path(Empty); // THROWS std::runtime_error: boost::filesystem::current_path: No such file or directory
    std::cout << "current_path(Empty) : " << boost::filesystem::current_path() << std::endl;
}


// G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G
#endif//FILESYSTEM_PATH_RELATIVE_TESTS_HPP_INCLUDED
