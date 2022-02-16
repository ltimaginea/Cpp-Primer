# uniform initialization

Prefer the `{}`-initializer syntax. The rules for `{}` initialization are simpler, more general, less ambiguous, and safer than for other forms of initialization.

Use `=` only when you are sure that there can be no narrowing conversions. For built-in arithmetic types, **use `=` only with `auto`.**

Avoid `()` initialization, which allows parsing ambiguities.

## References

- [ES.23: Prefer the {}-initializer syntax (C++ Core Guidelines)](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Res-list)
- [Item 7: Distinguish between () and {} when creating objects · EffectiveModernCppChinese · GitHub](https://github.com/kelthuzadx/EffectiveModernCppChinese/blob/master/3.MovingToModernCpp/item7.md)
- [Why am I getting an error after declaring a Foo object via Foo x(Bar())? (isocpp.org)](https://isocpp.org/wiki/faq/ctors#fn-decl-vs-obj-instantiation)
- [Is there any difference between List x; and List x();? (isocpp.org)](https://isocpp.org/wiki/faq/ctors#empty-parens-in-object-decl)

