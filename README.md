cpsm
====

cpsm is a matcher for [CtrlP][], specialized for paths.

Requirements
------------

- Vim 7.4, compiled with the `+python` flag.

- A C++ compiler supporting C++11.

- Boost (Ubuntu: package `libboost-all-dev`).

- CMake (Ubuntu: package `cmake`).

- Python headers (Ubuntu: package `python-dev`).

- Optional, required for Unicode support: ICU (Ubuntu: package `libicu-dev`).

Installation
------------

1. Install cpsm using your favorite Vim package manager. For example, with
   [Vundle](http://github.com/gmarik/Vundle.vim), this consists of adding:

        Vundle 'nixprime/cpsm'

   to your `vimrc` and then running `:PluginInstall` from Vim.

2. Build the Python module. On Linux, `cd` into `~/.vim/bundle/cpsm` and run
   `./install.sh`. Otherwise, peek inside `install.sh` and see what it does.

3. Add:

        let g:ctrlp_match_func = {'match': 'cpsm#CtrlPMatch'}

   to your `vimrc`.

Options
-------

- By default, cpsm will automatically detect the number of matcher threads
  based on the available hardware concurrency. To limit the number of threads
  that cpsm can use, add

        let g:cpsm_max_threads = (maximum number of threads)

  to your .vimrc.

- To enable Unicode support, add

        let g:cpsm_unicode = 1

  to your .vimrc. Unicode support is currently very limited, and consists
  mostly of parsing input strings as UTF-8 and handling the case of non-ASCII
  letters correctly.

Algorithm
---------

As with all CtrlP matchers I know of, cpsm is a subsequence matcher; all
characters in the query must appear in the search key in the same order, though
not necessarily adjacent. The main thing that distinguishes cpsm is its ranking
algorithm for paths.

- Consider the query `foobar` and the matches `baz/foobar.c` and
  `foo/baz/bar.c`. For a path search, the user most likely wants the first
  result: it is most likely that the user has entered part of the filename. But
  left-to-right matching will most likely favor the second result. This implies
  that we want to match at least the filename first.

- Consider the query `dfoo.c` and the matches `d/a/b/c/foo.c` and
  `d/a/b/d/foo.c`. If path components are matched left-to-right outside of the
  filename, then the two results are equivalent. In practice, the leading d is
  likely to be a disambiguating directory prefix *closer* to the filename. This
  implies that we want to match right-to-left across path components in
  general.

- Consider the query `foo` and the matches `foo_bar` and `bar_foo`. It's
  likelier that the user entered the beginning of the filename (and wants
  `foo_bar`) than that they entered a substring in the middle of the filename
  (and wants `bar_foo`). This implies that we value prefixes in path
  components, which in turn means that we need to match left-to-right within a
  path component.

- Consider the query `readme` and the matches `third_party/foo/README` and
  `third_party/bar/README`. Without additional context, these matches are
  equivalent. However, if the currently open file is `third_party/foo/foo.h`,
  then the former match is more likely to be correct. This implies that we
  value "proximity" to the current file.

License
-------

This software is licensed under the [Apache License, Version 2.0][LICENSE].

[CtrlP]: http://github.com/kien/ctrlp.vim
[LICENSE]: http://www.apache.org/licenses/LICENSE-2.0
