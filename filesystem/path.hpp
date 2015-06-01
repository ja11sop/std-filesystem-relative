// G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G
#ifndef XSTD_FILESYSTEM_PATH_HPP_INCLUDED
#define XSTD_FILESYSTEM_PATH_HPP_INCLUDED
// G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G

// Boost Library Includes
#include <boost/filesystem.hpp>

// C++ Standard Library Includes
// None


// n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n
namespace xstd {
namespace filesystem {
// n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n

class path : public boost::filesystem::path
{
private:
    typedef boost::filesystem::path base;

public:

    // constructors and destructor
    path() noexcept
    : base()
    {
    }

    path(const path& p)
    : base(p)
    {
    }

    path(path&& p) noexcept
    : base(std::move(p))
    {
    }

    template<class Source>
    path(const Source& source)
    : base( source )
    {
    }

    template <class InputIterator>
    path(InputIterator first, InputIterator last )
    : base( first, last )
    {
    }

    template <class Source>
    path(const Source& source, const std::locale& loc)
    : base( source, loc )
    {
    }

    template <class InputIterator>
    path(InputIterator first, InputIterator last, const std::locale& loc)
    : base( first, last, loc )
    {
    }

    // assignments
    path& operator=(const path& p)
    {
        base::operator=(p);
        return *this;
    }

    path& operator=(path&& p) noexcept
    {
        base::operator=(std::move(p));
        return *this;
    }

    template <class Source>
    path& operator=(const Source& source)
    {
        base::operator=(source);
        return *this;
    }

    template <class Source>
    path& assign(const Source& source)
    {
        base::operator=( source );
        return *this;
    }

    template <class InputIterator>
    path& assign(InputIterator first, InputIterator last)
    {
        base::assign( first, last );
        return *this;
    }

    // appends
    path& operator/=(const path& p)
    {
        base::operator/=(p);
        return *this;
    }

    template <class Source>
    path& operator/=(const Source& source)
    {
        base::operator/=(source);
        return *this;
    }

    template <class Source>
    path& append(const Source& source)
    {
        base::append(source);
        return *this;
    }

    template <class InputIterator>
    path& append(InputIterator first, InputIterator last)
    {
        base::append(first,last);
        return *this;
    }

    // concatenation
    path& operator+=(const path& x)
    {
        base::operator+=(x);
        return *this;
    }

    path& operator+=(const string_type& x)
    {
        base::operator+=(x);
        return *this;
    }

    path& operator+=(const value_type* x)
    {
        base::operator+=(x);
        return *this;
    }

    path& operator+=(value_type x)
    {
        base::operator+=(x);
        return *this;
    }

    template <class Source>
    path& operator+=(const Source& x)
    {
        base::operator+=(x);
        return *this;
    }

    template <class EcharT>
    path& operator+=(EcharT x)
    {
        base::operator+=(x);
        return *this;
    }

    template <class Source>
    path& concat(const Source& x)
    {
        base::concat(x);
        return *this;
    }

    template <class InputIterator>
    path& concat(InputIterator first, InputIterator last)
    {
        base::concat(first, last);
        return *this;
    }

    // modifiers
    path& make_preferred()
    {
        base::make_preferred();
        return *this;
    }

    path& remove_filename()
    {
        base::remove_filename();
        return *this;
    }

    path& replace_filename(const path& replacement)
    {
        base::remove_filename();
        base::operator/=(replacement);
        return *this;
    }

    path& replace_extension(const path& replacement = path())
    {
        base::replace_extension( replacement );
        return *this;
    }

//    path& make_normal()
//    {
//        static const path dot(".");
//        static const path dotdot("..");
//
//        path norm_p;
//        bool relative = true;
//
//        for( const auto& elem : *this )
//        {
//            if( elem == dot )
//            {
//                continue;
//            }
//            else if( elem == dotdot )
//            {
//                if( relative )
//                {
//                    norm_p /= dotdot;
//                }
//                else
//                {
//                    norm_p = norm_p.parent_path();
//                    if( norm_p.empty() )
//                    {
//                        relative = true;
//                    }
//                }
//            }
//            else
//            {
//                relative = false;
//                norm_p /= elem;
//            }
//        }
//        *this = std::move( norm_p );
//        return *this;
//    }

//    path& make_proximate(const path& start)
//    {
//        static const path dot(".");
//        static const path dotdot("..");
//
//        auto p_elem = begin();
//        auto p_end  = end();
//
//        auto start_elem = start.begin();
//        auto start_end  = start.end();
//
//        if( *p_elem != *start_elem )
//        {
//            return *this;
//        }
//
//        for( ; p_elem != p_end && start_elem != start_end; ++p_elem, ++start_elem )
//        {
//            if( *p_elem != *start_elem )
//            {
//                break;
//            }
//        }
//
//        path relative_path;
//
//        if( start_elem == start_end )
//        {
//            relative_path /= dot;
//        }
//        for( ; start_elem != start_end; ++start_elem )
//        {
//            relative_path /= dotdot;
//        }
//        for( ; p_elem != p_end; ++p_elem )
//        {
//            relative_path /= *p_elem;
//        }
//
//        *this = std::move( relative_path );
//        return *this;
//    }

    // decomposition
    path root_name() const
    {
        return base::root_name();
    }
    path root_directory() const
    {
        return base::root_directory();
    }
    path root_path() const
    {
        return base::root_path();
    }
    path relative_path() const
    {
        return base::relative_path();
    }
    path parent_path() const
    {
        return base::parent_path();
    }
    path filename() const
    {
        return base::filename();
    }
    path stem() const
    {
        return base::stem();
    }
    path extension() const
    {
        return base::extension();
    }

    // query
    explicit operator bool() const noexcept
    {
        return !empty();
    }
};


// n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n
}
}
// n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n n

// G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G G
#endif
