// G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G
#ifndef XSTD_FILESYSTEM_OPERATIONS_HPP_INCLUDED
#define XSTD_FILESYSTEM_OPERATIONS_HPP_INCLUDED
// G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G

// xstd Includes
#include <filesystem/path.hpp>

// Boost Library Includes
#include <boost/filesystem.hpp>

// C++ Standard Library Includes
#include <initializer_list>
#include <functional>


// n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n
namespace boost {
namespace filesystem {
// n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n


//using path_t = xstd::filesystem::path;
using path_t = boost::filesystem::path;


// Helper function to make implementation easier - not part of the proposal

template<class InputIteratorT>
auto common_prefix_helper( InputIteratorT First, InputIteratorT Last )
{
    using iter_range_t = std::pair<path_t::iterator, path_t::iterator>;
    std::vector<iter_range_t> Ranges;

    const path_t& Elem = *First;
    if( Elem.begin()->empty() )
    {
        return std::make_pair( path_t(), std::move( Ranges ) );
    }

    for( ; First != Last; ++First )
    {
        const path_t& Path = *First;
        Ranges.emplace_back( std::make_pair( Path.begin(), Path.end() ) );
    }

    auto increment = [&Ranges]()
    {
        for( auto& Range: Ranges )
        {
            ++(Range.first);
        }
    };

    auto not_at_end = [&Ranges]()
    {
        for( auto& Range: Ranges )
        {
            if( Range.first == Range.second ) return false;
        }
        return true;
    };

    path_t Common;
    for( ; not_at_end(); increment() )
    {
        auto MatchIter = Ranges.begin()->first;
        bool Matches = true;
        for( auto& Range: Ranges )
        {
            auto ElementIter = Range.first;
            if( *MatchIter != *ElementIter )
            {
                Matches = false;
                break;
            }
        }
        if( Matches )
        {
            Common /= *MatchIter;
        }
        else break;
    }
    return std::make_pair( Common, std::move(Ranges) );
}


//! \brief  Return and remove a common prefix from the sequence of paths defined
//!         by the range [first,last)
//!
//! \param  first - a ForwardIterator to the start of the range
//!
//! \param  last - a ForwardIterator to the end of the range
//!
//! \return a path representing the common prefix, if any, path() otherwise
template <class ForwardIterator>
path_t
remove_common_prefix( ForwardIterator First, ForwardIterator Last )
{
    return common_prefix( First, Last, First );
}


//! \brief  Remove a common prefix from the paths passed as arguments
//!
//! \param  p1 - a path object
//!
//! \param  p2 - a path object
//!
//! \return a path representing the common prefix, if any, path() otherwise.
//!         If a non-empty common prefix exists p1 and p2 will be changed to
//!         relative paths with the common prefix removed.
inline
path_t
remove_common_prefix( path_t& p1, path_t& p2 )
{
    std::array<std::reference_wrapper<path_t>, 2> Paths = { p1, p2 };
    return remove_common_prefix( Paths.begin(), Paths.end() );
}


//! \brief  Return a common prefix from the sequence of paths defined
//!         by the range [first,last) and place the remaining relative
//!         in out
//!
//! \param  first - an InputIterator to the start of the range
//!
//! \param  last  - an InputIterator to the end of the range
//!
//! \param  out   - an OutputIterator to the start of the output range
//!
//! \return a path representing the common prefix, if any, path() otherwise
template <class InputIterator, class OutputIterator>
path_t
common_prefix( InputIterator First, InputIterator Last, OutputIterator Out )
{
    auto Result = common_prefix_helper( First, Last );

    auto& Common = Result.first;
    auto& Ranges = Result.second;

    auto Range = Ranges.begin();
    auto End   = Ranges.end();

    for( ; Range != End; ++Range, ++Out )
    {
        path_t Trimmed;
        auto Element = Range->first;
        for( ; Element != Range->second; ++Element )
        {
            Trimmed /= *Element;
        }
        path_t& Path = *Out;
        Path = std::move( Trimmed );
    }
    return Common;
}


//! \brief  Return a common prefix from the paths `p1` and `p2`
//!
//! \param  p1 - a path object
//!
//! \param  p2 - a path object
//!
//! \return a path representing the common prefix, if any, path() otherwise
inline
path_t
common_prefix( const path_t& p1, const path_t& p2 )
{
    std::array<std::reference_wrapper<const path_t>, 2> Paths = { p1, p2 };
    return common_prefix_helper( Paths.begin(), Paths.end() ).first;
}


//! \brief  Return a common prefix from the sequence of paths defined
//!         by the range [first,last)
//!
//! \param  first - an InputIterator to the start of the range
//!
//! \param  last  - an InputIterator to the end of the range
//!
//! \return a path representing the common prefix, if any, path() otherwise
template<class InputIteratorT>
path_t
common_prefix( InputIteratorT First, InputIteratorT Last )
{
    return common_prefix_helper( First, Last ).first;
}


//! \brief  Return a common prefix from the sequence of paths referred to
//!         by the initializer_list<path>
//!
//! \param  list - an initializer_list of paths
//!
//! \return a path representing the common prefix, if any, path() otherwise
inline
path_t
common_prefix( std::initializer_list<path_t> List )
{
    return common_prefix_helper( List.begin(), List.end() ).first;
}


//! \brief  Return a normalized path by collapsing all redundant
//!         current ".", parent ".." directory elements and
//!         directory-separator elements
//!
//! \param  p - the path that we want a normalized path of
//!
//! \return a path representing a normalized version of p
inline
path_t
normalize( const path_t& p )
{
    static const path_t dot(".");
    static const path_t dotdot("..");

    path_t norm_p;
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


//! \brief  Return a relative path from `start` to `p` if one
//!         exists. This is a lexical-only analysis
//!
//! \param  p - the path we want a relative path to
//!
//! \param  start - the path that we want the relative path from
//!
//! \return a path representing a relative path from `start` to `p`
//!         if one exists, `path()` otherwise. If `p` and `start` are
//!         the same then `"."` is returned. If a non-empty path is
//!         returned then it will satisfy:
//!
//!         `p == normalize( start / lexically_relative( p, start ) )`
inline
path_t
lexically_relative( const path_t& p, const path_t& start )
{
    static const path_t dot(".");
    static const path_t dotdot("..");

    auto p_elem = p.begin();
    auto p_end  = p.end();

    auto start_elem = start.begin();
    auto start_end  = start.end();

    if( *p_elem != *start_elem )
    {
        return path_t();
    }

    for( ; p_elem != p_end && start_elem != start_end; ++p_elem, ++start_elem )
    {
        if( *p_elem != *start_elem )
        {
            break;
        }
    }

    path_t relative_path;

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


//! \brief Return a relative path to `p` from the current
//!        directory or from an optional `start` path.
//!
//! \param  `p` - the path we want a relative path to
//!
//! \param  `start` - the path that we want the relative path from
//!
//! \return A relative path, if the paths share a common 'root-name',
//!         otherwise `path()`. The relative path returned will satisfy
//!         the conditions shown in the following list. The common
//!         path is the common path that is shared between `p` and `start`.
//!         `rel_p` and `rel_start` are the divergent relative paths that
//!         remain after the common path is removed.
//!
//!         * if `exists(start)`
//!           * if `exists(p)` then `equivalent(start/relative(p,start),p) == true`
//!           * else `normalize(canonical(start)/relative(p,start)) == canonical(common)/normalize(rel_p)`
//!         * else
//!           * if `exists(p)` then `normalize(canonical(common)/rel_start)/relative(p,start)) == canonical(p)`
//!           * else `normalize(start/relative(p,start)) == normalize(p)`
//!
//! \throw As specified in Error reporting.
//!
//! \note `exists(start) && !is_directory(start)` is an error.
inline
path_t
relative( const path_t& p, const path_t& start, boost::system::error_code& ec )
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
    auto common_path = remove_common_prefix( rel_p, rel_start );

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
            return path_t();
        }
    }

    path_exists = exists( start, ec );
    if( ec )
    {
        ec.clear();
    }
    if( path_exists )
    {
        if( !is_directory( start ) )
        {
            ec.assign( boost::system::errc::not_a_directory, boost::system::generic_category() );
            return path_t();
        }
        real_start = canonical( real_start, ec );
        if( ec )
        {
            return path_t();
        }
    }
    else
    {
        real_start = common_path / rel_start;
    }

    path_exists = exists( p, ec );
    if( ec )
    {
        ec.clear();
    }
    if( path_exists )
    {
        real_p = canonical( p, ec );
        if( ec )
        {
            return path_t();
        }
    }
    else
    {
        real_p = common_path / rel_p;
    }
    return lexically_relative( real_p, real_start );
}


//! \brief Return a relative path to `p` from the current
//!        directory or from an optional `start` path.
//!
//! \param  `p` - the path we want a relative path to
//!
//! \return A relative path, if the paths share a common 'root-name',
//!         otherwise `path()`. The relative path returned will satisfy
//!         the conditions shown in the following list. The common
//!         path is the common path that is shared between `p` and `start`.
//!         `rel_p` and `rel_start` are the divergent relative paths that
//!         remain after the common path is removed.
//!
//!         * if `exists(start)`
//!           * if `exists(p)` then `equivalent(start/relative(p,start),p) == true`
//!           * else `normalize(canonical(start)/relative(p,start)) == canonical(common)/normalize(rel_p)`
//!         * else
//!           * if `exists(p)` then `normalize(canonical(common)/rel_start)/relative(p,start)) == canonical(p)`
//!           * else `normalize(start/relative(p,start)) == normalize(p)`
//!
//! \throw As specified in Error reporting.
inline
path_t
relative( const path_t& p, boost::system::error_code& ec )
{
    auto start = current_path( ec );
    if( ec )
    {
        return path_t();
    }
    return relative( p, start, ec );
}


//! \brief Return a relative path to `p` from the current
//!        directory or from an optional `start` path.
//!
//! \param  `p` - the path we want a relative path to
//!
//! \param  `start` - the path that we want the relative path from
//!
//! \return A relative path, if the paths share a common 'root-name',
//!         otherwise `path()`. The relative path returned will satisfy
//!         the conditions shown in the following list. The common
//!         path is the common path that is shared between `p` and `start`.
//!         `rel_p` and `rel_start` are the divergent relative paths that
//!         remain after the common path is removed.
//!
//!         * if `exists(start)`
//!           * if `exists(p)` then `equivalent(start/relative(p,start),p) == true`
//!           * else `normalize(canonical(start)/relative(p,start)) == canonical(common)/normalize(rel_p)`
//!         * else
//!           * if `exists(p)` then `normalize(canonical(common)/rel_start)/relative(p,start)) == canonical(p)`
//!           * else `normalize(start/relative(p,start)) == normalize(p)`
//!
//! \throw As specified in Error reporting.
//!
//! \note `exists(start) && !is_directory(start)` is an error.
inline
path_t
relative( const path_t& p, const path_t& start = current_path() )
{
    boost::system::error_code local_ec;
    auto result = relative( p, start, local_ec );
    if( local_ec )
    {
        BOOST_FILESYSTEM_THROW
        (   boost::filesystem::filesystem_error
            (   "boost::filesystem::relative",
                p, start,
                local_ec   )   );
    }
    return result;
}


//! \brief  Return a proximate path from `start` to `p`.
//!         This is a lexical-only analysis
//!
//! \param  p - the path we want a proximate path to
//!
//! \param  start - the path that we want the proximate path from
//!
//! \return returns `lexically_relative( p, start )` if it exists,
//!         otherwise `p`
inline
path_t
lexically_proximate( const path_t& p, const path_t& start )
{
    auto rel_path = lexically_relative( p, start );
    return rel_path.empty() ? p : rel_path;
}


//! \brief Return a proximate path to `p` from the current
//!        directory or from an optional `start` path.
//!
//! \param  `p` - the path we want a proximate path to
//!
//! \param  `start` - the path that we want the proximate path from
//!
//! \return Returns as if by `relative( p, start ).empty() ? p : relative( p, start )`.
//!
//! \throw As specified in Error reporting.
//!
//! \note `exists(start) && !is_directory(start)` is an error.
inline
path_t
proximate( const path_t& p, const path_t& start, boost::system::error_code& ec )
{
    auto rel_path = relative( p, start, ec );
    return rel_path.empty() ? p : rel_path;
}


//! \brief Return a proximate path to `p` from the current
//!        directory or from an optional `start` path.
//!
//! \param  `p` - the path we want a proximate path to
//!
//! \return Returns as if by `relative( p, start ).empty() ? p : relative( p, start )`.
//!
//! \throw As specified in Error reporting.
inline
path_t
proximate( const path_t& p, boost::system::error_code& ec )
{
    auto start = current_path( ec );
    if( ec )
    {
        return path_t();
    }
    return proximate( p, start, ec );
}


//! \brief Return a proximate path to `p` from the current
//!        directory or from an optional `start` path.
//!
//! \param  `p` - the path we want a proximate path to
//!
//! \param  `start` - the path that we want the proximate path from
//!
//! \return Returns as if by `relative( p, start ).empty() ? p : relative( p, start )`.
//!
//! \throw As specified in Error reporting.
//!
//! \note `exists(start) && !is_directory(start)` is an error.
inline
path_t
proximate( const path_t& p, const path_t& start = current_path() )
{
    boost::system::error_code local_ec;
    auto result = proximate( p, start, local_ec );
    if( local_ec )
    {
        BOOST_FILESYSTEM_THROW
        (   boost::filesystem::filesystem_error
            (   "boost::filesystem::proximate",
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
