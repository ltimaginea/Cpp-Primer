# std::unique_ptr Deep Copy

虽然 `std::unique_ptr` 删除了 copy constructor 和 copy assignment operator ，但其实我们可以借助移动操作变通地对 `std::unique_ptr` 进行拷贝。

deep copy 示例如下：

```cpp
std::unique_ptr<std::string> up1(std::make_unique<std::string>("Good morning"));

// copy construct!
std::unique_ptr<std::string> up2(std::make_unique<std::string>(*up1));
// safe copy construct!
std::unique_ptr<std::string> up3(up1 ? std::make_unique<std::string>(*up1) : nullptr);
// copy assignment!
up2 = std::make_unique<std::string>(*up1);
// safe copy assignment!
up3 = up1 ? std::make_unique<std::string>(*up1) : nullptr;
```

其它的例证：

- Google tensorflow : [unique_ptr - in copy assignment operator](https://github.com/tensorflow/tensorflow/blob/3d30ef6f49285b9e08a7b3492a902c67678a94d7/tensorflow/compiler/xla/service/spmd/spmd_partitioner_util.cc#L726-L735)
- Microsoft terminal : [unique_ptr - in copy constructor](https://github.com/microsoft/terminal/blob/fb597ed304ec6eef245405c9652e9b8a029b821f/src/cascadia/TerminalApp/Pane.LayoutSizeNode.cpp#L17-L25)
- 《Effective Modern C++》，Item22，其中的 class Widget 的 copy constructor 和 copy assignment operator 的实现，见 [EffectiveModernCppChinese/item22.md · GitHub](https://github.com/kelthuzadx/EffectiveModernCppChinese/blob/master/4.SmartPointers/item22.md) 

