# Virtual Destructor



## Copy Control



## [Discussion: Make base class destructors public and virtual, or protected and non-virtual (C++ Core Guidelines)](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Sd-dtor)

In rarer cases, such as policy classes, the class is used as a base class for convenience, not for polymorphic behavior. It is recommended to make those destructors protected and non-virtual: 

eg: [boost/core/noncopyable.hpp - 1.78.0](https://www.boost.org/doc/libs/1_78_0/boost/core/noncopyable.hpp) and [noncopyable - 1.78.0 (boost.org)](https://www.boost.org/doc/libs/1_78_0/libs/core/doc/html/core/noncopyable.html) 

```cpp
namespace noncopyable_  // protection from unintended ADL
{
#ifndef BOOST_NONCOPYABLE_BASE_TOKEN_DEFINED
#define BOOST_NONCOPYABLE_BASE_TOKEN_DEFINED

// noncopyable derives from base_token to enable Type Traits to detect
// whether a type derives from noncopyable without needing the definition
// of noncopyable itself.
//
// The definition of base_token is macro-guarded so that Type Trais can
// define it locally without including this header, to avoid a dependency
// on Core.

  struct base_token {};

#endif // #ifndef BOOST_NONCOPYABLE_BASE_TOKEN_DEFINED

  class noncopyable: base_token
  {
  protected:
#if !defined(BOOST_NO_CXX11_DEFAULTED_FUNCTIONS) && !defined(BOOST_NO_CXX11_NON_PUBLIC_DEFAULTED_FUNCTIONS)
      BOOST_CONSTEXPR noncopyable() = default;
      ~noncopyable() = default;
#else
      noncopyable() {}
      ~noncopyable() {}
#endif
#if !defined(BOOST_NO_CXX11_DELETED_FUNCTIONS)
      noncopyable( const noncopyable& ) = delete;
      noncopyable& operator=( const noncopyable& ) = delete;
#else
  private:  // emphasize the following members are private
      noncopyable( const noncopyable& );
      noncopyable& operator=( const noncopyable& );
#endif
  };
}
```



## Example



## References

- [C.127: A class with a virtual function should have a virtual or protected destructor (C++ Core Guidelines)](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-dtor)

- [C.35: A base class destructor should be either public and virtual, or protected and non-virtual (C++ Core Guidelines)](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rc-dtor-virtual)

- [Discussion: Make base class destructors public and virtual, or protected and non-virtual (C++ Core Guidelines)](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Sd-dtor)

- [boost/core/noncopyable.hpp - 1.78.0](https://www.boost.org/doc/libs/1_78_0/boost/core/noncopyable.hpp)

- [noncopyable - 1.78.0 (boost.org)](https://www.boost.org/doc/libs/1_78_0/libs/core/doc/html/core/noncopyable.html)

  ```cpp
  // noncopyable usage example
  #include <boost/core/noncopyable.hpp>
  
  class X: private boost::noncopyable	// note: private inheritance
  {
  };
  ```

  
