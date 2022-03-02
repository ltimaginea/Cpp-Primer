# header guard

`头文件保护符 (header guard)` 使用 `#define guard` 或者 `#pragma once` 来防止头文件被某个文件重复包含。应该优先使用属于 C++ 标准的 `#define guard` ，而 `#pragma once` 是不属于 C++ 标准的同时也是不可移植的，所以不推荐使用 `#pragma once` 。

## The #define Guard

All header files should have `#define` guards to prevent multiple inclusion. The format of the symbol name should be *`<PROJECT>_<PATH>_<FILE>_H_`* .

To guarantee uniqueness, they should be based on the full path in a project's source tree. For example, the file `foo/src/bar/baz.h` in project `foo` should have the following guard:

```cpp
// header.h
#ifndef FOO_BAR_BAZ_H_
#define FOO_BAR_BAZ_H_
// Code placed here is included only once per translation unit
#endif  // FOO_BAR_BAZ_H_
```

## #pragma once

The use of `#pragma once` can reduce build times, as the compiler won't open and read the file again after the first `#include` of the file in the translation unit. It's called the *multiple-include optimization*. It has an effect similar to the *include guard* idiom, which uses preprocessor macro definitions to prevent multiple inclusions of the contents of the file. It also helps to prevent violations of the *one definition rule*: the requirement that all templates, types, functions, and objects have no more than one definition in your code.

We recommend the `#pragma once` directive for new code because it doesn't pollute the global namespace with a preprocessor symbol. It requires less typing, it's less distracting, and it can't cause *symbol collisions*. Symbol collisions are errors caused when different header files use the same preprocessor symbol as the guard value. **It isn't part of the C++ Standard, but it's implemented portably by several common compilers.** 

There's no advantage to use of both the include guard idiom and `#pragma once` in the same file.

```cpp
// header.h
#pragma once
// Code placed here is included only once per translation unit
```



## References

- [SF.8: Use #include guards for all .h files (isocpp.github.io)](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rs-guards)
- [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html#The__define_Guard)
- [pragma once | Microsoft Docs](https://docs.microsoft.com/en-us/cpp/preprocessor/once?view=msvc-160)

