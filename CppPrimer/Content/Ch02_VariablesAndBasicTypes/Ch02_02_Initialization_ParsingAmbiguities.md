# initialization parsing ambiguities

## parsing ambiguities 1

### [Is there any difference between List x; and List x();? (isocpp.org)](https://isocpp.org/wiki/faq/ctors#empty-parens-in-object-decl)

A *big* difference!

Suppose that `List` is the name of some class. Then function `f()` declares a local `List` object called `x`:

```cpp
void f()
{
  List x;     // Local object named x (of class List)
  // ...
}
```

But function `g()` declares a function called `x()` that returns a `List`:

```cpp
void g()
{
  List x();   // Function named x (that returns a List)
  // ...
}
```



## parsing ambiguities 2

### [Why am I getting an error after declaring a Foo object via Foo x(Bar())? (isocpp.org)](https://isocpp.org/wiki/faq/ctors#fn-decl-vs-obj-instantiation)



## References

- [Is there any difference between List x; and List x();? (isocpp.org)](https://isocpp.org/wiki/faq/ctors#empty-parens-in-object-decl)
- [Why am I getting an error after declaring a Foo object via Foo x(Bar())? (isocpp.org)](https://isocpp.org/wiki/faq/ctors#fn-decl-vs-obj-instantiation)
- [ES.23: Prefer the {}-initializer syntax (C++ Core Guidelines)](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Res-list)
- [Item 7: Distinguish between () and {} when creating objects · EffectiveModernCppChinese · GitHub](https://github.com/kelthuzadx/EffectiveModernCppChinese/blob/master/3.MovingToModernCpp/item7.md)

