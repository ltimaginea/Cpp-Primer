#include <iostream>
#include <memory>
#include <new>
#include <string>

class Except0
{
 public:
  ~Except0() { std::cout << "Except0's destructor is called!" << std::endl; }

 private:
  std::string str_;
};

class Except1
{
 public:
  ~Except1() { std::cout << "Except1's destructor is called!" << std::endl; }

 private:
  std::string str_;
};

class Except2
{
 public:
  ~Except2() { std::cout << "Except2's destructor is called!" << std::endl; }

 private:
  std::string str_;
};

class Except3
{
 public:
  ~Except3() { std::cout << "Except3's destructor is called!" << std::endl; }

 private:
  std::string str_;
};

class Except4
{
 public:
  ~Except4() { std::cout << "Except4's destructor is called!" << std::endl; }

 private:
  std::string str_;
};

void Test3()
{
  Except4 ex4;
  // throw an exception: std::out_of_range
  std::string().at(1);
  Except3 ex3;
}

void Test2()
{
  Except2 ex2;
  Test3();
}

void Test1()
{
  Except1 ex1;
  Test2();
}

int main()
{
  std::cout << "Test start" << std::endl;
  try
  {
    Except0 ex0;
    Test1();
  }
  catch (const std::bad_alloc& err)
  {
    std::cout << err.what() << std::endl;
  }
  /*catch (const std::exception& err)
  {
    std::cout << err.what() << std::endl;
  }
  catch (...)
  {
    std::cout << "unknown exceptions" << std::endl;
  }*/
  std::cout << "Test end" << std::endl;
  return 0;
}
