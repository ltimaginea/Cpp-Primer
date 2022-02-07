#include <iostream>

class B
{
public:
	virtual ~B() = default;

	void f_publ() {  }
protected:
	void f_prot() {  }
private:
	void f_priv() {  }	// None of the derived classes can access anything that is private in B.
};

// In D_publ, the public parts of B are public and the protected parts of B are protected (D_publ is-a-kind-of-a B).
class D_publ : public B {  };

// In D_prot, the public and protected parts of B are protected.
class D_prot : protected B {  };

// In D_priv, the public and protected parts of B are private.
class D_priv : private B {  };

class D_D_publ : public D_publ
{
public:
	void f()
	{
		f_publ();	// Ok
		f_prot();	// Ok
	}
};

class D_D_prot : protected D_prot
{
public:
	void f()
	{
		f_publ();	// Ok
		f_prot();	// Ok
	}
};

//class D_D_priv : private D_priv
class D_D_priv : public D_priv
{
public:
	void f()
	{
		f_publ();	// Error: inaccessible. In D_priv, the public and protected parts of B are private.
		f_prot();	// Error: inaccessible. In D_priv, the public and protected parts of B are private.
	}
};

int main()
{
	D_D_publ d_d_publ;
	D_D_prot d_d_prot;
	D_D_priv d_d_priv;

	d_d_publ.f_publ();	// Ok
	d_d_prot.f_publ();	// Error: inaccessible
	d_d_priv.f_publ();	// Error: inaccessible

	return 0;
}
