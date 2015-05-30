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
  * [3. Design Discussion](#3-design-discussion)
  * [4. Proposal](#4-proposal)
  * [5. Proposed Wording](#5-proposed-wording)
  * [6. Reference Implementation](#6-reference-implementation)
  * [Acknowledgements](#acknowledgements)

## 1. Introduction

The [File System TS - N4099](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4099.html) introduces relative paths.

* They are defined in section **4.18 relative path [fs.def.relative-path]**
* A decomposition method `relative_path()` is described in section **8.4.9 path decomposition [path.decompose]**
* Two query methods to determine if a path either `has_relative_path()` or `is_relative()` described in **8.4.10 path query [path.query]**

However there is no way to create a relative path as a path relative to another. Methods are however provided to create absolute and canonical paths.

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

In addition to reason about relative paths in the context of asserting that an absolute path can be combined with a relative path to create a new absolute path we also need a `normalize` facilty.

Python and Java both provide their equivalents of this function. Note this is **not** the same as a `canonical` function. A `normalize` function is purely focused on the collapsing of redundant current "`.`", parent "`..`" and path separators at the lexical level. To achieve a normalised in the presence of a path that exists on the filesystem then `canonical` should be used.

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

### 2.3 `remove_common_prefix`

Lastly the implementation `relative` generally requires a function that can remove a common prefix, or at least return the common prefix from a number of paths passed to it. In the case of `relative` that would be `path` and `start` but in the general case it could be a range of paths. Python provides a function that is similar but flawed in that it only compares character-by-character allowing invalid paths to be returned.

#### 2.3.1 Python `commonprefix()`

Python provides a similar function call `commonprefix()`:

> **`os.path.commonprefix(list)`**
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

  1. To obtain, if one exists, a **relative path** `rel_path` from `start` to `path` such that on exists we can say that `path` is equivalent to `start/rel_path`.

  2. To obtain the **nearest path** or **proximate path** `proximate`, from `start` to `path`. If a relative path from `start` to `path` exists then that will be the `proximate` path otherwise `absolute(path)` will itself be the `proximate` path. Essentially the `proximate` path is the shortest traversal from `start` to `path`.

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

This holds for all options shown. **Option 1** is attractive because it additionally allows general use to not result in an error, minimising the extra code required while retaining intuitive use. This is important as it paves the way to code such as the following (in reality you need to test `empty()`):

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

and also it allows use-case 2 (**proximate path**) to be trivially conceptualised in terms of `relative()`. For example:

```cpp
path proximate( const path& Path, const path& Start )
{
    if( auto RelPath = relative( Path, Start ) ) return RelPath;
    return Path;
}
```

In fact unless `operator bool` is added to `path` to indicate whether a `path` is `empty()` or not the previous example would need to be rewritten in terms of `empty()`. This would be the slightly less intuitive, but equally trivial:

```cpp
path proximate( const path& Path, const path& Start )
{
    auto RelPath = relative( Path, Start );
    return RelPath.empty() ? Path : RelPath;
}
```

In order to support **Option 2** and **Option 3** it would be necessary to additionally specify a new error condition:

```cpp
enum class filesystem_errc {
    no_relative_path_exists = implementation defined
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

#### 3.2.2 Return value when asking for a proximate path

Given a path `Path` and a path `Start` which we want to determine the proximate path from, we can see there are three possible situations:

  1.  `relative( Path, Start )` exists
  2.  `relative( Path, Start )` does not exist
  3.  error (from calling other operations internally)

It is simplest to specify `proximate()` in terms of `relative()` so that handling of paths (same of different) for which a relative path exists can be handled consistently. Where no relative path exists then `Path` is returned:

| Scenario                | Option 1                  |
|:------------------------|---------------------------|
| relative exists         | `relative( Path, Start )` |
| no relative path        | `Path`                    |
| error                   | **error**                 |

An error will only arise from some internal call such as to the filesystem.

#### 3.2.3 Two separate functions

Given that there are two clear and distinct use-cases and that there is a reasonable vocabulary to distinguish them this proposal favours having both `relative()` and `proximate()`, standardising the vocabulary and making code more readable by making the intent clear.

#### 3.2.4 Lexical only versions

There is often a desire to restrict the behaviour of `relative()` and `proximate()` to a purely lexical operation. This is an important use case and should be supported. There are three ways in which this can be achieved:

  1.  Provide lexical-only member functions to `path`, say, `make_relative()` and `make_proximate()`
  2.  Provide alternatively named free-functions that are lexical-only (**bikeshed warning**). Possible names (considering `relative` only for simplicity) might be `make_relative()`, `relative_path()`, `lexical_relative()` and so on.
  3.  The last option is to specify a tag of some description that is passed to `relative()` and `proximate()` that controls whether the operation is lexcial-only. At this time it appears that the motivating use-case for this is simply lexical, or not. Other specific use cases (such as normalising paths first but not resolving symlinks) are best left to the user to build on top of a lexical-only function.


#### 3.2.5 Controlling base path for relative paths

Rather than adding a separate `base` parameter to handle relative paths that are passed to `relative()` or `proximate()`, complicating the interface (`current_path()` would be the default) it is expected that users would first wrap the paths passed with `absolute()` and specify the required `base` path at that point.

## 4. Proposal

The proposal is to add the following operations to ... **TODO**

## 5. Proposed Wording

**TO BE COMPLETED**

Insert the section:

----

**4.18 proximate path [fs.def.proximate-path]**

A path that may be absolute or relative but which captures the shortest traversal from one path to another. If the start path shares the same root then proximate path will be relative, otherwise absolute beginning with the root of the path you want to traverse to.

----

and bump the following sub-section numbers up by 0.1.

Modify section:

**6 Header <filesystem> synopsis [fs.filesystem.synopsis]**

by adding the operational functions after `canonical`:

----
```cpp
path normalize(const path& p) noexcept;

path proximate(const path& p, const path& start = current_path());
path proximate(const path& p, error_code& ec);
path proximate(const path& p, const path& start, error_code& ec);

path relative(const path& p, const path& start = current_path());
path relative(const path& p, error_code& ec);
path relative(const path& p, const path& start, error_code& ec);
```
----

Insert the section:

----
**15.3 Normalize [fs.op.normalize]**

```cpp
path normalize(const path& p) noexcept;
```

 * *Overview:*
    Return a normalized path of `p` by collapsing all redundant current "`.`", parent "`..`" directory elements and directory-separator elements.

 * *Returns:*
    A normalized path which may be relative or absolute, though it will not contain any current "`.`" directory element or any parent "`..`" directory elements after the first non-parent element.

**15.4 Relative [fs.op.relative]**

```cpp
path relative(const path& p, const path& start = current_path());
path relative(const path& p, error_code& ec);
path relative(const path& p, const path& start, error_code& ec);
```

  * *Overview:* Return a relative path of `p` to the current directory or from an optional `start` path.

  * *Returns:* A relative path, if the paths share a common 'root-name', otherwise `path()`. The relative path returned will satisfy the conditions shown in the following list. The `common` path is the common path that is shared between `p` and `start`. `rel_p` and `rel_start` are the divergent relative paths that remain after the `common` path is removed.
    * if `exists(start)`
      * if `exists(p)` then `equivalent(start/relative(p,start),p) == true`
      * else `normalize(canonical(start)/relative(p,start)) == canonical(common)/normalize(rel_p)`
    * else
      * if `exists(p)` then `normalize(canonical(common)/rel_start)/relative(p,start)) == canonical(p)`
      * else `normalize(start/relative(p,start)) == normalize(p)`

  * *Throws:* As specified in Error reporting.

  * *Remarks:* `exists(start) && !is_directory(start)` is an error.

----

and bump all following sections up by 0.2. Update the contents and any cross-references accordingly.

## 6. Reference Implementation

A prototype implementation based on [Boost.Filesystem](http://www.boost.org/doc/libs/release/libs/filesystem/) along with tests can be found here, complete with a comprehensive set of tests.

https://github.com/ja11sop/std-filesystem-relative

## Acknowledgements

**TODO**
