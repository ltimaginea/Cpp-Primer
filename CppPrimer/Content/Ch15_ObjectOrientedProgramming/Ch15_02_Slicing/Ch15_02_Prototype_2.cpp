#include <iostream>
#include <vector>
#include <array>
#include <iterator>
#include <memory>
#include <utility>
#include <typeinfo>
#include <cassert>

class Shape
{
public:
	std::unique_ptr<Shape> Clone() const
	{
		std::unique_ptr<Shape> result = DoClone();
		assert(typeid(*result) == typeid(*this) && "Every derived class must correctly override DoClone.");
		return result;
	}

	virtual void Draw() = 0;

	virtual ~Shape() = default;

protected:
	Shape() = default;

	// Make the polymorphic base class copy and move operations protected to prevent slicing, 
	// and so that only the derived class can invoke them in its own copy and move operations.
	Shape(const Shape&) = default;
	Shape(Shape&&) = default;
	Shape& operator=(const Shape&) = default;
	Shape& operator=(Shape&&) = default;

private:
	virtual std::unique_ptr<Shape> DoClone() const = 0;
};

class Circle : public Shape
{
public:
	void Draw() override
	{
		std::cout << "Draw a circle." << std::endl;
	}

private:
	std::unique_ptr<Shape> DoClone() const override
	{
		return std::make_unique<Circle>(*this);
	}
};

class Square : public Shape
{
public:
	void Draw() override
	{
		std::cout << "Draw a square." << std::endl;
	}

private:
	std::unique_ptr<Shape> DoClone() const override
	{
		return std::make_unique<Square>(*this);
	}
};

// a simple demo implementation of polymorphic_value
class Picture
{
public:
	explicit Picture(std::unique_ptr<Shape> shape) : shape_(std::move(shape)) {  }

	explicit Picture(const Shape& shape) : shape_(shape.Clone()) {  }

	Picture(const Picture& rhs) : shape_(rhs.shape_->Clone()) {  }

	Picture& operator=(const Picture& rhs)
	{
		if (this != &rhs)
		{
			shape_ = rhs.shape_->Clone();
		}
		return *this;
	}

	Picture(Picture&&) = default;
	Picture& operator=(Picture&&) = default;
	~Picture() = default;

	void ChangeShape(std::unique_ptr<Shape> shape)
	{
		shape_ = std::move(shape);
	}

	void ChangeShape(const Shape& shape)
	{
		shape_ = shape.Clone();
	}

	void Draw()
	{
		shape_->Draw();
	}

private:
	std::unique_ptr<Shape> shape_;
};

int main()
{
	Picture picture1(std::make_unique<Circle>());
	picture1.Draw();
	Picture picture2(std::make_unique<Square>());
	picture2.Draw();

	// OK: The copy constructor performs a polymorphic deep copy.
	Picture picture3(picture1);
	picture3.Draw();
	// OK: The copy assignment operator performs a polymorphic deep copy.
	picture3 = picture2;
	picture3.Draw();

	Picture picture4(std::move(picture1));
	picture4.Draw();
	picture4 = std::move(picture2);
	picture4.Draw();

	Picture picture5(std::make_unique<Circle>());
	picture5.Draw();
	picture5.ChangeShape(std::make_unique<Square>());
	picture5.Draw();

	Circle circle;
	Square square;
	// a polymorphic deep copy of a given object
	Picture picture6(circle);
	picture6.Draw();
	// a polymorphic deep copy of a given object
	picture6.ChangeShape(square);
	picture6.Draw();

	// a polymorphic deep copy of a temporary object
	Picture picture7(Circle{});
	picture7.Draw();
	// a polymorphic deep copy of a temporary object
	picture7.ChangeShape(Square{});
	picture7.Draw();

	std::vector<Picture> pictures1;
	pictures1.emplace_back(std::make_unique<Circle>());
	pictures1.emplace_back(std::make_unique<Square>());
	// a polymorphic deep copy of a temporary object
	pictures1.emplace_back(Circle{});
	// a polymorphic deep copy of a temporary object
	pictures1.emplace_back(Square{});
	for (auto& picture : pictures1)
	{
		picture.Draw();
	}

	std::vector<std::unique_ptr<Shape>> shapes1;
	shapes1.push_back(std::make_unique<Circle>());
	shapes1.push_back(std::make_unique<Square>());
	for (auto& shape : shapes1)
	{
		shape->Draw();
	}

	// The container (pictures2) is constructed with a polymorphic deep copy of 
	// each of the elements in the std::initializer_list.
	std::vector<Picture> pictures2{ picture3, picture4, picture5, picture6, picture7 };
	// OK: The copy constructor performs a polymorphic deep copy.
	std::vector<Picture> pictures3(pictures1);
	std::vector<Picture> pictures4(std::move(pictures1));
	// OK: The copy assignment operator performs a polymorphic deep copy.
	pictures3 = pictures2;
	pictures4 = std::move(pictures2);

	std::vector<std::unique_ptr<Shape>> shapes2;
	shapes2.push_back(std::make_unique<Circle>());
	shapes2.push_back(std::make_unique<Circle>());
	shapes2.push_back(std::make_unique<Circle>());

	auto init = std::to_array<std::unique_ptr<Shape>>({ std::make_unique<Circle>(), std::make_unique<Square>() });
	std::vector<std::unique_ptr<Shape>> shapes3(std::make_move_iterator(init.begin()), std::make_move_iterator(init.end()));

	// Error: The std::vector<std::unique_ptr<Shape>> is not a copy constructible type.
	//std::vector<std::unique_ptr<Shape>> shapes4(shapes1);
	std::vector<std::unique_ptr<Shape>> shapes5(std::move(shapes1));
	// Error: The std::vector<std::unique_ptr<Shape>> is not a copy assignable type.
	//shapes3 = shapes2;
	shapes5 = std::move(shapes2);

	return 0;
}
