// G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G
#ifndef XBOOST_FILESYSTEM_PATH_HPP_INCLUDED
#define XBOOST_FILESYSTEM_PATH_HPP_INCLUDED
// G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G

// Boost Library Includes
#include <boost/filesystem.hpp>

// C++ Standard Library Includes
// None


// n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n
namespace boost {
namespace filesystem {
// n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n


path
remove_common( path& p1, path& p2 )
{
    auto p1_elem = p1.begin();
    auto p1_end  = p1.end();

    auto p2_elem = p2.begin();
    auto p2_end  = p2.end();

    if( *p1_elem != *p2_elem )
    {
        return path();
    }

    path common_path;
    for( ; p1_elem != p1_end && p2_elem != p2_end; ++p1_elem, ++p2_elem )
    {
        if( *p1_elem == *p2_elem )
        {
            common_path /= *p1_elem;
        }
        else break;
    }

    path trimmed_p1;
    path trimmed_p2;

    for( ; p1_elem != p1_end; ++p1_elem )
    {
        trimmed_p1 /= *p1_elem;
    }

    for( ; p2_elem != p2_end; ++p2_elem )
    {
        trimmed_p2 /= *p2_elem;
    }

    p1 = trimmed_p1;
    p2 = trimmed_p2;

    return common_path;
}


path
normalize( const path& p )
{
    static const path dot(".");
    static const path dotdot("..");

    path norm_p;
    bool relative = true;

    for( const auto& elem : p )
    {
        if( elem == dot )
        {
            continue;
        }
        else if( elem == dotdot )
        {
            if( relative )
            {
                norm_p /= dotdot;
            }
            else
            {
                norm_p = norm_p.parent_path();
                if( norm_p.empty() )
                {
                    relative = true;
                }
            }
        }
        else
        {
            relative = false;
            norm_p /= elem;
        }
    }
    return norm_p;
}


path
relative_to( const path& p, const path& start = current_path() )
{
    static const path dot(".");
    static const path dotdot("..");

    auto p_elem = p.begin();
    auto p_end  = p.end();

    auto start_elem = start.begin();
    auto start_end  = start.end();

    if( *p_elem != *start_elem )
    {
        return path();
    }

    for( ; p_elem != p_end && start_elem != start_end; ++p_elem, ++start_elem )
    {
        if( *p_elem != *start_elem )
        {
            break;
        }
    }

    path relative_path;

    if( start_elem == start_end )
    {
        relative_path /= dot;
    }
    for( ; start_elem != start_end; ++start_elem )
    {
        relative_path /= dotdot;
    }
    for( ; p_elem != p_end; ++p_elem )
    {
        relative_path /= *p_elem;
    }

    return relative_path;
}


path
relativise( const path& p, const path& start = current_path() )
{
    return relative_to( normalize( absolute( p ) ), normalize( absolute( start ) ) );
}


path
relative( const path& p, const path& start, system::error_code& ec )
{
    auto real_p = p;
    auto real_start = start;

    if( real_p.is_relative() )
    {
        real_p = absolute( real_p );
    }
    if( real_start.is_relative() )
    {
        real_start = absolute( real_start );
    }

    auto rel_p = normalize( real_p );
    auto rel_start = normalize( real_start );
    auto common_path = remove_common( rel_p, rel_start );

    bool path_exists = exists( common_path );
    if( ec )
    {
        ec.clear();
    }
    if( path_exists )
    {
        common_path = canonical( common_path, ec );
        if( ec )
        {
            return path();
        }
    }

    path_exists = exists( start );
    if( ec )
    {
        ec.clear();
    }
    if( path_exists )
    {
        if( !is_directory( start ) )
        {
            ec.assign( system::errc::not_a_directory, system::generic_category() );
            return path();
        }
        real_start = canonical( real_start, ec );
        if( ec )
        {
            return path();
        }
    }
    else
    {
        real_start = common_path / rel_start;
    }

    path_exists = exists( p );
    if( ec )
    {
        ec.clear();
    }
    if( path_exists )
    {
        real_p = canonical( p, ec );
        if( ec )
        {
            return path();
        }
    }
    else
    {
        real_p = common_path / rel_p;
    }
    ec.clear();
    return relative_to( real_p, real_start );
    // How about: return real_p.make_relative( real_start ); ?
}


path
relative( const path& p, system::error_code& ec )
{
    return relative( p, current_path(), ec );
}


path
relative( const path& p, const path& start = current_path() )
{
    system::error_code local_ec;
    auto result = relative( p, start, local_ec );
    if( local_ec )
    {
        BOOST_FILESYSTEM_THROW
        (   filesystem_error
            (   "boost::filesystem::relative",
                p, start,
                local_ec   )   );
    }
    return result;
}


// n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n
}
}
// n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n

// G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G
#endif
