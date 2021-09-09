# Copy and Move

## [Copy and Move (C++ Core Guidelines)](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#ccopy-copy-and-move)

### C.60: Make copy assignment non-`virtual`, take the parameter by `const&`, and return by non-`const&` 

A copy assignment operator should (implicitly or explicitly) invoke all base and member copy assignment operators. 



### C.63: Make move assignment non-`virtual`, take the parameter by `&&`, and return by non-`const&` 

A move assignment operator should (implicitly or explicitly) invoke all base and member move assignment operators.



### C.64: A move operation should move and leave its source in a valid state

That is the generally assumed semantics. After `y = std::move(x)` the value of `y` should be the value `x` had and `x` should be in a valid state. 

Ideally, that moved-from should be the default value of the type. Ensure that unless there is an exceptionally good reason not to. However, not all types have a default value and for some types establishing the default value can be expensive. The standard requires only that the moved-from object can be destroyed. Often, we can easily and cheaply do better: The standard library assumes that it is possible to assign to a moved-from object. Always leave the moved-from object in some (necessarily specified) valid state. 

Unless there is an exceptionally strong reason not to, make `x = std::move(y); y = z;` work with the conventional semantics.



### C.65: Make move assignment safe for self-assignment

There is no known general way of avoiding an `if (this == &a) return *this;` test for a move assignment and still get a correct answer (i.e., after `x = x` the value of `x` is unchanged). 

The ISO standard guarantees only a “valid but unspecified” state for the standard-library containers. 



### C.66: Make move operations `noexcept`

A throwing move violates most people’s reasonable assumptions. A non-throwing move will be used more efficiently by standard-library and language facilities. 

A move operation should be marked `noexcept`. 



### C.80: Use `=default` if you have to be explicit about using the default semantics

The compiler is more likely to get the default semantics right and you cannot implement these functions better than the compiler. Writing out the bodies of the copy and move operations is verbose, tedious, and error-prone. A compiler does it better. 

The `= default` is the best and simplest way of doing that.



### C.81: Use `=delete` when you want to disable default behavior (without wanting an alternative)

In a few cases, a default operation is not desirable. Use `=delete` when you want to disable default behavior. 

Note that deleted functions should be public.

