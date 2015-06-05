# Additions to Filesystem supporting Relative Paths

| Document Number: | DXXXX                      |
|:-----------------|:---------------------------|
| Date:            | 2015-05-12                 |
| Revises:         | None                       |
| Project:         | Programming Language C++   |
| Project number:  | TS 18822                   |
| Reply-to:        | TODO                       |

## Abstract

This paper proposes the addition of several convenience functions to the [File System TS - N4099](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4099.html) to make handling of relative paths easier.

## Table of Contents

  * [1. Introduction](#1-introduction)
  * [2. Motivation and Scope](#2-motivation-and-scope)
      * [2.1 `relative`](#21-relative)
      * [2.2 `normalize`](#22-normalize)
      * [2.3 `remove_common_prefix` and `common_prefix`](#23-remove_common_prefix-and-common_prefix)
  * [3. Design Discussion](#3-design-discussion)
      * [3.1 Free function *operations* or `path` members or both](#31-free-function-operations-or-path-members-or-both)
      * [3.2 `relative`](#32-relative)
      * [3.3 `normalize`](#33-normalize)
      * [3.4 `remove_common_prefix` and `common_prefix`](#34-remove_common_prefix-and-common_prefix)
  * [4. Proposal](#4-proposal)
  * [5. Proposed Wording](#5-proposed-wording)
  * [6. Reference Implementation](#6-reference-implementation)
  * [Acknowledgements](#acknowledgements)

## 1. Introduction

The [File System TS - N4099](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4099.html) introduces relative paths.

* They are defined in section **4.18 relative path [fs.def.relative-path]**
* A decomposition method `relative_path()` is described in section **8.4.9 path decomposition [path.decompose]**
* Two query methods to determine if a path either `has_relative_path()` or `is_relative()` described in **8.4.10 path query [path.query]**

However **there is no way to create a relative path** as a path relative to another. Methods are however provided to create absolute and canonical paths.

In section **15.1 Absolute [fs.op.absolute]**:

```cpp
path absolute(const path& p, const path& base=current_path());
```

and in section **15.2 Canonical [fs.op.canonical]**

```cpp
path canonical(const path& p, const path& base = current_path());
path canonical(const path& p, error_code& ec);
path canonical(const path& p, const path& base, error_code& ec);
```

## 2. Motivation and Scope

By providing operations to achieve *absolute* and *canonical* paths there is no impediment to providing a similar operation `relative()` (exposition only) that attempts to return a new path *that is relative* to some *start* path.

For example:

```cpp
path relative(const path& p, const path& start = current_path());
path relative(const path& p, error_code& ec);
path relative(const path& p, const path& start, error_code& ec);
```

This could return a `path`, if possible, that is relative to `start`. The implementation can make use of `absolute()` and `canonical()` when determining the relative path, if the paths exist.

The File System TS is based on the `​boost::filesystem` library and it too suffers from this anomaly. There are open tickets for this in ​Boost Trac:

  * [https://svn.boost.org/trac/boost/ticket/5897](#5897 Make path relative function)
  * [https://svn.boost.org/trac/boost/ticket/1976](#1976 Inverse function for complete)

and it is the subject of several posts on StackOverflow for example:

  * [http://stackoverflow.com/questions/10167382/boostfilesystem-get-relative-path](http://stackoverflow.com/questions/10167382/boostfilesystem-get-relative-path)
  * [http://stackoverflow.com/questions/5772992/get-relative-path-from-two-absolute-paths](http://stackoverflow.com/questions/5772992/get-relative-path-from-two-absolute-paths)

### 2.1 `relative`

The basic use case is:

```cpp
path Path  = "/a/d";
path Start = "/a/b/c";

auto RelPath = relative( Path, Start );

assert( RelPath == path("../../d") );
assert( Path == normalize( Start/RelPath ) );
```

Other languages typically provide a similar function. 

#### 2.1.1 Python `relpath()`

For example python provides `os.path.relpath()`:

> ```python
os.path.relpath(path[, start])
```
>
> Return a relative filepath to `path` either from the current directory or from an optional `start` directory. This is a path computation: the filesystem is not accessed to confirm the existence or nature of `path` or `start`.
>
> `start` defaults to `os.curdir`
>
> Note: If no such relative path exists then the function will return `'.'` - the dot path representing the current directory.

#### 2.1.2 Java `relativize()`

Similarly Java provides provides `java.nio.file.Path.relativize()`:

> ```java
Path relativize(Path other)
```
>
> Constructs a relative path between this path and a given path.
>
> Relativization is the inverse of `resolution`. This method attempts to construct a `relative` path that when `resolved` against this path, yields a path that locates the same file as the given path. For example, on UNIX, if this path is "`/a/b`" and the given path is "`/a/b/c/d`" then the resulting relative path would be "`c/d`". Where this path and the given path do not have a `root` component, then a relative path can be constructed. A `relative` path cannot be constructed if only one of the paths has a `root` component. Where both paths have a `root` component then it is implementation dependent if a relative path can be constructed. If this path and the given path are `equal` then an empty path is returned.
>
>  For any two `normalized` paths *p* and *q*, where *q* does not have a `root` component,
>
>  ```java
p.relativize(p.resolve(q)).equals(q)
```
>
> When symbolic links are supported, then whether the resulting path, when resolved against this path, yields a path that can be used to locate the *same* file as other is implementation dependent. For example, if this path is "`/a/b`" and the given path is "`/a/x`" then the resulting relative path may be "`../x`". If "`b`" is a symbolic link then is implementation dependent if "`a/b/../x`" would locate the same file as "`/a/x`".
>
> **Parameters:**
>
>    `other` - the path to relativize against this path
>
> **Returns:**
>
>    the resulting relative path, or an empty path if both paths are equal
>
> **Throws:**
>
>    `IllegalArgumentException` - if `other` is not a `Path` that can be relativized against this path

#### 2.1.3 Go `Rel()`

As part of the `filepath` package Go provides the `Rel()` function:

> ```go
func Rel(basepath, targpath string) (string, error)
```
>
> `Rel` returns a relative path that is lexically equivalent to `targpath` when joined to `basepath` with an intervening separator. That is, `Join(basepath, Rel(basepath, targpath))` is equivalent to `targpath` itself. On success, the returned path will always be relative to `basepath`, even if `basepath` and `targpath` share no elements. An error is returned if `targpath` can't be made relative to `basepath` or if knowing the current working directory would be necessary to compute it. 

### 2.2 `normalize`

In addition to reason about relative paths in the context of asserting that an absolute path can be combined with a relative path to create a new absolute path we also need a `normalize` facilty. A basic use-case for this might be:

```cpp
path Path = "/a/b/c/../.././d/."
auto NormalPath = normalize( Path );

assert( NormalPath == path("/a/d") );
```

Python and Java both provide their equivalents of this function. Note this is **not** the same as a `canonical` function. A `normalize` function is purely focused on the collapsing of redundant current "`.`", parent "`..`" and path separators at the lexical level. To achieve a normalised path in the presence of a path that exists on the filesystem then `canonical` should be used.

#### 2.2.1 Python `normpath()`

Python provides `os.path.normpath()`:

> ```python
os.path.normpath(path)
```
>
> Normalize a pathname by collapsing redundant separators and up-level references so that `A//B`,` A/B/`, `A/./B` and `A/foo/../B` all become `A/B`. This string manipulation may change the meaning of a path that contains symbolic links. On Windows, it converts forward slashes to backward slashes. To normalize case, use normcase().

#### 2.2.2 Java `normalize()`

Jave provides `java.nio.file.Path.normalize()`: 

> ```java
Path normalize()
```
>
> Returns a path that is this path with redundant name elements eliminated.
>
> The precise definition of this method is implementation dependent but in general it derives from this path, a path that does not contain redundant name elements. In many file systems, the "`.`" and "`..`" are special names used to indicate the current directory and parent directory. In such file systems all occurrences of "`.`" are considered redundant. If a "`..`" is preceded by a non-"`..`" name then both names are considered redundant (the process to identify such names is repeated until is it no longer applicable).
> 
> This method does not access the file system; the path may not locate a file that exists. Eliminating "`..`" and a preceding name from a path may result in the path that locates a different file than the original path. This can arise when the preceding name is a symbolic link.
> 
> **Returns:**
>
>    the resulting path or this path if it does not contain redundant name elements; an empty path is returned if this path does have a root component and all name elements are redundant

#### 2.2.3 Go `Clean()`

Go provides the `Clean()` function in the `filepath` package:

> ```go
func Clean(path string) string
```
>
> `Clean` returns the shortest path name equivalent to `path` by purely lexical processing. It applies the following rules iteratively until no further processing can be done:
>
> 1. Replace multiple Separator elements with a single one.
> 2. Eliminate each `.` path name element (the current directory).
> 3. Eliminate each inner `..` path name element (the parent directory) along with the non-`..` element that precedes it.
> 4. Eliminate `..` elements that begin a rooted path: that is, replace "`/..`" by "`/`" at the beginning of a path, assuming Separator is '`/`'.
>
> The returned path ends in a slash only if it represents a root directory, such as "`/`" on Unix or ``C:\`` on Windows.
>
> If the result of this process is an empty string, `Clean` returns the string "`.`". 

#### 2.2.4 C++ Boost.Filesystem `normalize()`

Earlier versions of [Boost.Filesystem](http://www.boost.org/doc/libs/release/libs/filesystem/) included a `normalize()` member function of `path` but it was removed in version [1.34](http://www.boost.org/doc/libs/1_34_0/libs/filesystem/doc/tr2_proposal.html) as part of the revamp of the library for a TR2 proposal. It's specification is shown below:

> ```cpp
path & normalize();
```
> **Postcondition**: `m_name` is in *normalized form*.
>
> **Returns**: `*this`
>
> *Normalized form*
>
> Normalized form is the same as *canonical form*, except that adjacent *name*, *parent-directory* elements are recursively removed.
> Thus a non-empty path in normal form either has no *directory-placeholders*, or consists solely of one *directory-placeholder*. If it has *parent-directory* elements, they precede all *name* elements.
>
> *Canonical form*
>
> All operations modifying *path* objects leave the *path* object in canonical form. An empty path is in canonical form. A non-empty path is converted to canonical form as if by first converting it to the conceptual model, and then:
>  *  Repeatedly replacing any leading *root-directory*, *parent-directory* elements with a single *root-directory* element. Rationale: Both POSIX and Windows specify this reduction; specifying it for *canonical form* ensures portable semantics for other operating systems.
>  *  Removing each *directory-placeholder* element.
>  *  If the path is now empty, add a single *directory-placeholder* element.

The rationale for the removal of `normalize()` (and `canonize()` which was also a `path` member) was:

> The Boost implementation has `basic_path` functions `canonize()` and `normalize()` which return cleaned up string representations of a pathname. They have been removed from the proposal as messy to specify and implement, not hugely useful, and possible to implement by users as non-member functions without any loss of functionality or efficiency. There was also a concern the proposal was getting a bit large.
>
> These functions can be added later as convenience functions if the LWG so desires..
> &mdash;Beman Dawes

However both functions are hugely useful and also specifiable. In fact `canonical()` is part of the [File System TS - N4099](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4099.html). `normalize()` is most useful when considered in the context of a relative path which does not represent a real path on the filesystem. Consider the question of creating path from a base path and a relative path - lexically. `normalize()` is required in order to create an easily interpreted representation.

### 2.3 `remove_common_prefix` and `common_prefix`

Lastly the implementation `relative` generally requires a function that can remove a common prefix, or at least return the common prefix from a number of paths passed to it. In the case of `relative` that would be `path` and `start` but in the general case it could be a range of paths.

Example: `common_prefix()`:

```cpp
path Path1 = "/a/b/c/d/e/f";
path Path2 = "/a/b/c/j/k";

auto Common = common_prefix( Path1, Path2 );

assert( Common = path("/a/b/c") );
```

Example: `remove_common_prefix()`:

```cpp
path Path1 = "/a/b/c/d/e/f";
path Path2 = "/a/b/c/j/k";

auto Common = remove_common_prefix( Path1, Path2 );

assert( Common = path("/a/b/c") );
assert( Path1 = path("d/e/f") );
assert( Path2 = path("j/k") );
```

Python provides a function that is similar but flawed in that it only compares character-by-character allowing invalid paths to be returned.

#### 2.3.1 Python `commonprefix()`

Python provides a similar function call `commonprefix()`:

> ```python
os.path.commonprefix(list)
```
>
> Return the longest path prefix (taken character-by-character) that is a prefix of all paths in `list`. If `list` is empty, return the empty string (`''`). Note that this may return invalid paths because it works a character at a time.

## 3. Design Discussion

There is a clear demarcation in the [File System TS - N4099](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4099.html) between *operations* that may touch the file system (such as `exists()`), and the `path` class itself which is a purely lexical abstraction used to store a representation of a conceptual path that may or may not have a realisation in a physical file system.

### 3.1 Free function *operations* or `path` members or both

Given that there already exists a separation between non-member operations that touch the file system and `path` with its lexical-only modifiers an argument can be made for retaining this distinction with the new proposed operations. For example having `normalize()` (or `make_normal()`) and `relativize()` (or `make_relative()`) members that are lexical only.

A `remove_common_prefix()` (or `common_prefix()`) function makes most sense as a free function as it could operate on 2 or more paths at the same time.

In the case of a `relative()` free function the expectation would be that this could implicitly touch the file system, like `canonical()` and `absolute()` (for example to default `start` to `current_path()`).

Such a separation would nicely separate the need to specify whether a call to `relative()` should touch the file system as the expected behaviour would be obtained in each case. The member function would be lexical only (as expected) and the free function would "do the right thing" should the paths in question exist on the file system (symlinks be resolved and so on).

### 3.2 `relative`

This section will focus on use cases for `relative()` (or indeed `relativize()`/`make_relative()`). 

If we assume we have a path `path` and a start directory `start` that we want to determine a relative path from, then there appear to be two main reasons to call `relative`:

  1. To obtain, if one exists, a **relative path** `rel_path` from `start` to `path` such that on exists we can say that `path` is equivalent to `start/rel_path`. For example:

| Path        | Start       | Relative Path     | 
|:------------|-------------|-------------------|
| `"/a/d"`    |  `"/a/b/c"` | `"../../d"`       | 
| `"/a/b/c"`  |  `"/a/d"`   | `"../b/c"`        | 
| `"C:\y"`    |  `"C:\x"`   | `"..\y"`          |
| `"D:\y"`    |  `"C:\x"`   | no relative path  |

  2. To obtain the **nearest path** or **proximate path** `proximate`, from `start` to `path`. If a relative path from `start` to `path` exists then that will be the `proximate` path otherwise `absolute(path)` will itself be the `proximate` path. Essentially the `proximate` path is the shortest traversal from `start` to `path`. For example:

| Path        | Start       | Proximate Path    | 
|:------------|-------------|-------------------|
| `"/a/d"`    |  `"/a/b/c"` | `"../../d"`       | 
| `"C:\y"`    |  `"C:\x"`   | `"..\y"`          |
| `"D:\y"`    |  `"C:\x"`   | `"D:\y"`          |


The solution to the second use-case can be implemented in terms of the first use-case. The implications of each use-case determine what should be returned in the event that no relative path exists.

#### 3.2.1 Return value when asking for a relative path

Given a path `Path` and a start path `Start` then there are four possible situations:

  1.  a relative path exists
  2.  the paths are the same
  3.  no relative path exists
  4.  error (from calling other operations internally)

There are a number of options of what to return for each situation. These are enumerated in as follows:

| Scenario                | Option 1        | Option 2        | Option 3        |
|:------------------------|-----------------|-----------------|-----------------|
| relative path exists    | *relative path* | *relative path* | *relative path* |
| the paths are the same  | `path(".")`     | `path(".")`     | `path()`        |
| no relative path exists | `path()`        | **error**       | **error**       |
| error                   | **error**       | **error**       | **error**       |

The basic assumption is that should a relative path `RelPath` **from** the **start** path `Start` for path `Path` exist then it should hold that:

```cpp
Path == normalize(Start/RelPath)
```

This holds for all options shown. 

**Option 1** is attractive because it additionally allows general use to not result in an error, minimising the extra code required while retaining intuitive use. For example we can write code like this:

```cpp
auto RelPath = relative( Path, Start );

if( !RelPath.empty() )
{
    // use relative path
}
else
{
    // perhaps use Path directly
}
```

It allows use-case 2 (**proximate path**) to be trivially conceptualised in terms of `relative()`. For example:

```cpp
path proximate( const path& Path, const path& Start )
{
    auto RelPath = relative( Path, Start );
    return RelPath.empty() ? Path : RelPath;
}
```

Similarly it also paves the way to more *script-like* usage should `explicit operator bool` be added to `path` to indicate if a `path` is `empty()`. **This is not proposed** and is shown only for usage comparison with languages such as Python. For example we could write:

```cpp
if( auto RelPath = relative( Path, Start ) )
{
    // use RelPath
}
else
{
    // use Path
}
```
or define `proximate()` as:

```cpp
path proximate( const path& Path, const path& Start )
{
    if( auto RelPath = relative( Path, Start ) ) return RelPath;
    return Path;
}
```

In order to support **Option 2** and **Option 3** it would be necessary to additionally specify a new error condition:

```cpp
enum class filesystem_errc {
    no_relative_path_exists = implementation defined non zero value
};
```

A similar implementation of `proximate()` using **Option 2** or **Option 3** therefore might look as follows:

```cpp
path proximate( const path& Path, const path& Start )
{
    error_code Error;
    auto RelPath = relative( Path, Start, Error );
    // Only if relative() can return other errors otherwise we can just use:
    // error_code DontCare;
    if( Error && Error != filesystem_errc::no_relative_path_exists )
    {
        // Actual Error has occured
    }
    return RelPath.empty() ? Path : RelPath;
}
```

This is still acceptable but much more verbose and requires cherry-picking the `no_relative_path_exists` error code so that we may handle this error condition from other potential "real" errors.

#### 3.2.2 Return value when asking for a proximate path

Given a path `Path` and a path `Start` which we want to determine the proximate path from, we can see there are three possible situations:

  1.  `relative( Path, Start )` exists
  2.  `relative( Path, Start )` does not exist
  3.  error (from calling other operations internally)

It is simplest to specify `proximate()` in terms of `relative()` so that handling of paths (same of different) for which a relative path exists can be handled consistently. Where no relative path exists then `Path` is returned:

| Scenario                | Result                    |
|:------------------------|---------------------------|
| relative exists         | `relative( Path, Start )` |
| no relative path        | `Path`                    |
| error                   | **error**                 |

An error will only arise from some internal call such as to the filesystem.

#### 3.2.3 Two separate functions

Given that there are two clear and distinct use-cases and that there is a reasonable vocabulary to distinguish them this proposal favours having both `relative()` and `proximate()`, standardising the vocabulary and making code more readable by making the intent clear.

#### 3.2.4 Lexical only versions

There is often a desire to restrict the behaviour of `relative()` and `proximate()` to a purely lexical operation. This is an important use case and should be supported. There are three ways in which this can be achieved:

  1.  Provide lexical-only member functions to `path`, say, `make_relative()` and `make_proximate()`, or `relative_from()` and `proximate_from()`. If such member functions were provided it may make sense to include a `make_normal()` or `normalize()` member also. 
  2.  Provide alternatively named free-functions that are lexical-only (**bikeshed warning**). Possible names (considering `relative` only for simplicity) might be `make_relative()`, `relative_path()`, `lexically_relative()` and so on.
  3.  The last option is to specify a tag of some description that is passed to `relative()` and `proximate()` that controls whether the operation is lexcial-only. At this time it appears that the motivating use-case for this is simply lexical, or not. Other specific use cases (such as normalising paths first but not resolving symlinks) are best left to the user to build on top of a lexical-only function. If that is the case this could essentially be a `bool` flag.

Having separate lexical-only functions may allow a more optimal specification and a more optimal implementation when compared to a flag based approach. This proposal has favoured the second option. This feels more in keeping with other aspects of the library where the restrictive case is given the more precise name.

#### 3.2.5 Controlling base path for relative paths

Rather than adding a separate `base` parameter to handle relative paths that are passed to `relative()` or `proximate()`, complicating the interface (`current_path()` would be the default) it is expected that users would first wrap the paths passed with `absolute()` and specify the required `base` path at that point.

#### 3.2.6 Just Working

Setting aside the case of lexical-only versions for `relative()` and `proximate()` it is worth noting that there are a number of possibilities to consider when paths may or may not exist on the filesystem. Given two paths `p1` and `p2` we can say that:

  1. `exists(p1)` may or may not be `true`
  2. `exists(p2)` may or may not be `true`
  3. `normalize(p1)` may or may not equal `p1`
  4. `normalize(p2)` may or may not equal `p2`
  5. `absolute(normalize(p1)` may or may not equal `canonical(p1)`
  6. `absolute(normalize(p2)` may or may not equal `canonical(p2)`
  7. `p1.is_relative()` may or may not be `true`
  8. `p2.is_relative()` may or may not be `true`

In fact there are quite a few possibilities that make it hard correctly right a `relative()` (and hence `proximate()`) function. Except in the case where a lexical-only analysis is desired it is expected that the function should "just work". That is, should the paths exist on the filesystem and (say) contain symlinks then any resultant relative path that is identified must be capable of successfully navigating from the start path to the path provided. Given the combinations outlined in the previous list we can see that it is important to correctly specify and implement `relative()` so that it does indeed do the right thing. It is not sufficient to provide just a lexical-only operation and ask poeple to implement a functional `relative()` function on top of it. It is far too easy to get it wrong and getting it right is not a casually trivial implementation.

As an example consider the following example implementation. It is certainly readable but not something you would want every user to have to write just to get correct behaviour:

```cpp
path
relative( const path& p, const path& start, std::error_code& ec )
{
    auto real_p = p;
    auto real_start = start;

    if( real_p.is_relative() )
        real_p = absolute( real_p );
        
    if( real_start.is_relative() )
        real_start = absolute( real_start );

    auto rel_p = normalize( real_p );
    auto rel_start = normalize( real_start );
    auto common_path = remove_common_prefix( rel_p, rel_start );

    bool path_exists = exists( common_path, ec );
    if( ec ) ec.clear();
    
    if( path_exists )
    {
        common_path = canonical( common_path, ec );
        if( ec ) return path_t();
    }

    path_exists = exists( start, ec );
    if( ec ) ec.clear();

    if( path_exists )
    {
        if( !is_directory( start ) )
        {
            ec.assign( std::errc::not_a_directory, std::generic_category() );
            return path_t();
        }
        real_start = canonical( real_start, ec );
        if( ec ) return path_t();
    }
    else
    {
        real_start = common_path / rel_start;
    }

    path_exists = exists( p, ec );
    if( ec ) ec.clear();
    
    if( path_exists )
    {
        real_p = canonical( p, ec );
        if( ec ) return path_t();
    }
    else
    {
        real_p = common_path / rel_p;
    }
    return lexically_relative( real_p, real_start );
}
```

### 3.3 `normalize`

The specification of a `normalize()` free function, member or both is relatively straightforward in comparison to `relative()`. It will be a purely lexical operation. If the `path` to be normalised exists on the filesystem (and therefore my include symlinks) then `canonical()` is the best approach to normalisation.

`normalize()` and `canonical()` both have value in specifying the effects of `relative()` which could vary depending on the existance or not of a `path`.

This proposal believes that a single free-function `normalize()` is sufficient to provide the needed functionality but would not object to a `path` member instead, or as well as the free function.

### 3.4 `remove_common_prefix` and `common_prefix`

Identifying (and in many cases removing) a common path prefix shared between mutliple paths is a common operation, and one that is often used in the implementation of `relative()`. Typically these functions would take two or more `path` objects (depending on their specification) and return a `path` object that represents the common prefix shared by all passed paths&mdash;if one exists. Such functions could be named `remove_common_prefix()`&mdash;for the case where the prefix is removed from paths passed, and `common_prefix()`&mdash;for the case where the paths passed are left unchanged. 

It makes sense for a `common_prefix()` to provide overloads for both `std::initializer_list<path>` and one taking a pair of `InputIterator`s. This could be extended further to take an additional `OutputIterator` parameter so that paths relative to the common path could be output. It also seems to make sense to provide for the most common use-case, a pair of paths, and so provide an overload taking two paths by `const` reference.

The `remove_common_prefix()` variant is a little more restricted due to the need to remove any common prefix from the paths that are passed. This rules out having a `std::initializer_list<path>` overload. The iterator overload would be similar `common_prefix()` except that it would take two `ForwardIterator` parameters. This could be implemented in terms of the three iterator overload of `common_prefix()` passing `first` to `out`. Again providing an overload for the two path common case makes a lot of sense given the additional effort required to utilise the iterator interface.

## 4. Proposal

In a nutsell the proposal is to add the following operations to the [File System TS - N4099](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4099.html).

```cpp
path common_prefix( const path& p1, const path& p2 );
template<class InputIteratorT>
  path common_prefix( InputIterator first, InputIterator last );
template<class InputIterator, class OutputIterator>
  path common_prefix( InputIterator first, InputIterator last, OutputIterator out );
path common_prefix( initializer_list<path> ); 

path lexically_relative( const path& p, const path& start );
path lexically_proximate( const path& p, const path& start );

path normalize(const path& p);

path proximate(const path& p, const path& start = current_path());
path proximate(const path& p, error_code& ec);
path proximate(const path& p, const path& start, error_code& ec);

path relative(const path& p, const path& start = current_path());
path relative(const path& p, error_code& ec);
path relative(const path& p, const path& start, error_code& ec);

path remove_common_prefix( path& p1, path& p2 );
template <class ForwardIterator>
  path remove_common_prefix( ForwardIterator first, ForwardIterator last );
```

## 5. Proposed Wording

This section offers tentative draft wording so that the impact on the [File System TS - N4099](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4099.html) can be better appreciated. 

Insert the section:

----

**4.18 proximate path [fs.def.proximate-path]**

A path that may be absolute or relative but which captures the shortest traversal from one path to another. If the start path shares the same root then proximate path will be relative, otherwise absolute beginning with the root of the path you want to traverse to.

----

and bump the following sub-section numbers up by 0.1.

Modify section:

**6 Header <filesystem> synopsis [fs.filesystem.synopsis]**

by adding the operational functions shown in the appropriate alphabetical order:

----
```cpp
path common_prefix( const path& p1, const path& p2 );
template<class InputIteratorT>
  path common_prefix( InputIterator first, InputIterator last );
template<class InputIterator, class OutputIterator>
  path common_prefix( InputIterator first, InputIterator last, OutputIterator out );
path common_prefix( initializer_list<path> ); 

path lexically_relative( const path& p, const path& start );
path lexically_proximate( const path& p, const path& start );

path normalize(const path& p);

path proximate(const path& p, const path& start = current_path());
path proximate(const path& p, error_code& ec);
path proximate(const path& p, const path& start, error_code& ec);

path relative(const path& p, const path& start = current_path());
path relative(const path& p, error_code& ec);
path relative(const path& p, const path& start, error_code& ec);

path remove_common_prefix( path& p1, path& p2 );
template <class ForwardIterator>
  path remove_common_prefix( ForwardIterator first, ForwardIterator last );
```
----

Insert the sections:

----

**15.3 Common Prefix [fs.op.common_prefix]**

```cpp
template <class InputIterator>
  path common_prefix( InputIterator first, InputIterator last )
```
  * *Effects:* Return a common prefix from the sequence of paths defined by the range `[first,last)`

  * *Returns:* a path representing the common prefix if any, `path()` otherwise.

```cpp
template <class InputIterator, class OutputIterator>
  path common_prefix( InputIterator first, InputIterator last, OutputIterator out )
```
  * *Effects:* Return a common prefix from the sequence of paths defined by the range `[first,last)`. The remaining relative path for each path in the range is placed in `out`. The path placed in `out` for an input path `first` with a common prefix `common` shall satisy the expression `*first == normalize( common/*out )`. 

  * *Returns:* a path representing the common prefix if any, `path()` otherwise.

```cpp
path common_prefix( std::initializer_list<path> )
```
  * *Effects:* Return a common prefix from the sequence of paths referred to by the `initializer_list<path>` object. 

  * *Returns:* a path representing the common prefix if any, `path()` otherwise.

```cpp
path common_prefix( const path& p1, const path& p2 )
```
  * *Effects:* Return a common prefix for the paths `p1` and `p2`.

  * *Returns:* a path representing the common prefix if any, `path()` otherwise.


**15.27 Lexically Proximate [fs.op.lexically_proximate]**

```cpp
path lexically_proximate(const path& p, const path& start);
```

  * *Effects:* Return the proximate path from `start` to `p`. This is a lexical-only analysis.

  * *Returns:* Returns as if by `lexically_relative( p, start ).empty() ? p : lexically_relative( p, start )`.


**15.28 Lexically Relative [fs.op.lexically_relative]**

```cpp
path lexically_relative(const path& p, const path& start);
```

  * *Effects:* Return a relative path from `start` to `p` if one exists. This is a lexical-only analysis.

  * *Returns:* A path representing a relative path from `start` to `p` if one exists, `path()` otherwise. If `p` and `start` are the same then `"."` is returned. If a non-empty path is returned then it will satisfy `p == normalize( start / lexically_relative( p, start ) )`.


**15.29 Normalize [fs.op.normalize]**

```cpp
path normalize(const path& p);
```
 * *Effects:* Return a normalized path of `p` by collapsing all redundant current "`.`", parent "`..`" directory elements and directory-separator elements.

 * *Returns:* A normalized path which may be relative or absolute, though it will not contain any current "`.`" directory element or any parent "`..`" directory elements after the first non-parent element.


**15.31 Proximate [fs.op.proximate]**

```cpp
path proximate(const path& p, const path& start = current_path());
path proximate(const path& p, error_code& ec);
path proximate(const path& p, const path& start, error_code& ec);
```

  * *Effects:* Return a proximate path to `p` from the current directory or from an optional `start` path.

  * *Returns:* Returns as if by `relative( p, start ).empty() ? p : relative( p, start )`.

  * *Throws:* As specified in Error reporting.

  * *Remarks:* `exists(start) && !is_directory(start)` is an error.


**15.33 Relative [fs.op.relative]**

```cpp
path relative(const path& p, const path& start = current_path());
path relative(const path& p, error_code& ec);
path relative(const path& p, const path& start, error_code& ec);
```

  * *Effects:* Return a relative path to `p` from the current directory or from an optional `start` path.

  * *Returns:* A relative path, if the paths share a common 'root-name', otherwise `path()`. The relative path returned will satisfy the conditions shown in the following list. The `common` path is the common path that is shared between `p` and `start`. `rel_p` and `rel_start` are the divergent relative paths that remain after the `common` path is removed.
    * if `exists(start)`
      * if `exists(p)` then `equivalent(start/relative(p,start),p) == true`
      * else `normalize(canonical(start)/relative(p,start)) == canonical(common)/normalize(rel_p)`
    * else
      * if `exists(p)` then `normalize(canonical(common)/rel_start)/relative(p,start)) == canonical(p)`
      * else `normalize(start/relative(p,start)) == normalize(p)`

  * *Throws:* As specified in Error reporting.

  * *Remarks:* `exists(start) && !is_directory(start)` is an error.


**15.36 Remove Common Prefix [fs.op.remove_common_prefix]**

```cpp
template <class ForwardIterator>
  path remove_common_prefix( ForwardIterator first, ForwardIterator last )
```
  * *Effects:* Return and remove a common prefix from the sequence of paths defined by the range `[first,last)`. If there is no common prefix the paths in the range `[first,last)` will remain unchanged.

  * *Returns:* a path representing the common prefix if any, `path()` otherwise.

```cpp
path remove_common_prefix( path& p1, path& p2 )
```
  * *Effects:* Return and remove a common prefix for the paths `p1` and `p2`. If there is no common prefix the paths `p1` and `p2` will remain unchanged.

  * *Returns:* a path representing the common prefix if any, `path()` otherwise.

----

and all intermediate and following section numbering accordingly. Also Update the contents and any cross-references as appropriate.

## 6. Reference Implementation

A prototype implementation based on [Boost.Filesystem](http://www.boost.org/doc/libs/release/libs/filesystem/) along complete with a fairly comprehensive set of tests, can be found here:

https://github.com/ja11sop/std-filesystem-relative

## Acknowledgements

The author would like to thank Christopher Kohlhoff for his suggestions and feedback.
