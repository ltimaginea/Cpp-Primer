# interface inheritance and implementation inheritance



1. 接口继承：基类是抽象基类，其成员函数都是纯虚函数。代码示例:

```cpp
// interface inheritance
class Shape1
{
public:
	virtual void Move(int, int) = 0;
	virtual ~Shape1() = default;
};

class Circle1 : public Shape1
{
public:
	void Move(int, int) override { /*...*/ }
};
```

2. 实现继承：基类是非抽象基类，其成员函数都是有完整实现的非纯虚函数或者非虚函数。代码示例:

```cpp
// implementation inheritance
class Shape2
{
public:
	void Rotate(int angle) { DefaultRotate(angle); }
	virtual ~Shape2() = default;
protected:
	virtual void DefaultRotate(int) { /*...*/ }
};

class Circle2 : public Shape2
{
	// ...
};
```

3. 混合的接口和实现继承：基类是抽象基类，其成员函数是既有纯虚函数又有完整实现的非纯虚函数或者非虚函数。代码示例:

```cpp
// mixed interface and implementation inheritance
class Shape3
{
public:
	void Rotate(int angle) { DefaultRotate(angle); }
	virtual void Move(int, int) = 0;
	virtual ~Shape3() = default;
protected:
	virtual void DefaultRotate(int) { /*...*/ }
};

class Circle3 : public Shape3
{
public:
	void Move(int, int) override { /*...*/ }
	// ...
};
```



## References

- *Effective C++* Item31, Item34
- [C.129: When designing a class hierarchy, distinguish between implementation inheritance and interface inheritance (C++ Core Guidelines)](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-kind)
- [I.25: Prefer empty abstract classes as interfaces to class hierarchies (C++ Core Guidelines)](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Ri-abstract)

