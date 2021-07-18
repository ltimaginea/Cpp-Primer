# *format input and output*

每个iostream对象维护一个格式状态来控制IO如何格式化的细节。格式状态控制格式化的某些方面，如整型值是几进制、浮点值的精度、一个输出元素的宽度等。 

标准库定义了一组操纵符（manipulator）来修改流的格式状态。一个操纵符是一个函数或是一个对象，会影响流的状态，并能用作输入或输出运算符的运算对象。类似输入和输出运算符，操纵符也返回它所处理的流对象，因此我们可以在一条语句中组合操纵符和数据。

大多数改变格式状态的操纵符都是设置/复原成对的；一个操纵符用来将格式状态设置为一个新值，而另一个用来将其复原，恢复为正常的默认格式。当操纵符改变流的格式状态时，通常改变后的状态对所有后续IO都生效。因此，通常最好在不再需要特殊格式时尽快将流恢复到默认状态。

## 控制布尔值的格式

默认情况下，一个true值输出为整数1，而false输出为0。我们可以通过对流使用 `boolalpha` 操纵符来使布尔值输出为字符串，使用 `noboolalpha` 来恢复到默认状态。

```cpp
bool is = false;
cout << boolalpha << is << noboolalpha << endl;
```

## 指定整数值的进制

启用或禁用非负数输出中的正号 `+` 的显示。

```cpp
int32_t sale = 343797;
cout << showpos << sale << noshowpos << endl;
```

默认情况下，整型值的输入输出使用十进制。我们可以使用操纵符 `hex` 、 `oct` 和 `dec` 将其改为十六进制、八进制或是改回十进制。操纵符hex、oct和dec只影响整型运算对象，浮点值的表示形式不受影响。

使用 `showbase` 操纵符显示进制前缀，使用 `uppercase` 操纵符来输出大写的 X 并将十六进制数字 a-f 以大写输出。

进制前缀规范：前缀 0x 表示十六进制；前缀 0 表示八进制；无前缀字符串表示十进制。

```cpp
cout << showbase << uppercase << hex;
cout << UINT32_MAX << endl;
cout << noshowbase << nouppercase << dec;
```

## 控制浮点数格式

默认情况下，浮点值按六位数字精度打印；如果浮点值没有小数部分，则不打印小数点；根据浮点数的值选择打印成定点十进制或科学记数法形式。标准库会选择一种可读性更好的格式：非常大和非常小的值打印为科学记数法形式，其他值打印为定点十进制形式。

### 指定打印精度

默认情况下，精度会控制打印的数字的总数。当打印时，浮点值按当前精度四舍五入而非截断。

我们可以通过调用IO对象的 `precision` 成员或使用 `setprecision` 操纵符来改变精度。precision成员是重载的。一个版本接受一个int值，将精度设置为此值，并返回旧精度值。另一个版本不接受参数，返回当前精度值。setprecision操纵符接受一个参数，用来设置精度。

操纵符setprecision和其他接受参数的操纵符都定义在头文件iomanip中。

```cpp
// 返回当前精度值
auto prec = cout.precision();
cout << prec << endl;
cout << sqrt(2.0) << endl;
cout.precision(12);
cout << sqrt(2.0) << endl;
cout << setprecision(7);
cout << sqrt(2.0) << endl;
cout.precision(prec);
```



### 指定浮点数记数法

操纵符 `scientific` 改变流的状态来使用科学记数法，操纵符 `fixed` 改变流的状态来使用定点十进制，在C++11中，也可以通过 `hexfloat` 强制浮点数使用十六进制格式。C++11还提供另一个名为 `defaultfloat` 的操纵符，它将流恢复到默认状态——根据要打印的值选择记数法。 

这些操纵符也会改变流的精度的默认含义。在执行scientific、fixed或hexfloat后，精度值控制的是小数点后面的数字位数，而默认情况下精度值指定的是数字的总位数——既包括小数点之后的数字也包括小数点之前的数字。

默认情况下，十六进制数字和科学记数法中的 e 都打印成小写形式。我们可以用uppercase操纵符打印这些字母的大写形式。

```cpp
cout << 100 * sqrt(2.0) << endl;
cout << fixed << 100 * sqrt(2.0) << endl;
cout << scientific << 100 * sqrt(2.0) << endl;
cout << hexfloat << 100 * sqrt(2.0) << endl;
cout << defaultfloat << 100 * sqrt(2.0) << endl;
```

### 打印小数点

默认情况下，当一个浮点值的小数部分为0时，不显示小数点。`showpoint` 操纵符强制打印小数点。操纵符 `noshowpoint` 恢复默认行为。

```cpp
cout << 10.0 << endl;
cout << showpoint << 10.0 << noshowpoint << endl;
```



## 输出补白

当按列打印数据时，我们常常需要非常精细地控制数据格式。标准库提供了一些操纵符帮助我们完成所需的控制： `setw` 指定下一个数字或字符串值的最小空间。 `left` 表示左对齐输出。 `right` 表示右对齐输出，右对齐是默认格式。 `internal` 控制负数的符号的位置，它左对齐符号，右对齐值，用空格填满所有中间空间。 `setfill` 允许指定一个字符代替默认的空格来补白输出。 

setw类似endl，不改变输出流的内部状态。它只决定下一个输出的大小。

```cpp
int i = -16;
double d = 3.14159;
cout << i << endl;
cout << d << endl;
cout << setw(12) << i << endl;
cout << setw(12) << d << endl;

cout << left;
cout << setw(12) << i << endl;
cout << setw(12) << d << endl;

cout << right;
cout << setw(12) << i << endl;
cout << setw(12) << d << endl;

cout << internal;
cout << setw(12) << i << endl;
cout << setw(12) << d << endl;

cout << setfill('#');
cout << setw(12) << i << endl;
cout << setw(12) << d << endl;
cout << setfill(' ');
```



## 控制输入格式

默认情况下，输入运算符会忽略空白符（空格符、制表符、换行符、换纸符和回车符）。

操纵符 `noskipws` 会令输入运算符读取空白符，而不是跳过它们。为了恢复默认行为，我们可以使用 `skipws` 操纵符。

```cpp
char ch;
// 设置读取空白符
cin >> noskipws;
while (cin >> ch)
	cout << ch;
// 恢复到默认状态，忽略读取空白符
cin >> skipws;
```



![](https://github.com/ltimaginea/Cpp-Primer/blob/main/CppPrimer/Images/Chapter17/Ch17_05_FormatInputAndOutput.jpg)



