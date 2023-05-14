#include "polymorphism.h"

// базовый класс
VIRTUAL_CLASS( Base )
	int a;
END( Base )
// методы
DECLARE_METHOD( Base, Both )
DECLARE_METHOD( Base, OnlyBase )

// класс-наследник
VIRTUAL_CLASS_DERIVED( Derived, Base )
	int b;
END_DERIVE( Derived, Base )
// методы
DECLARE_METHOD( Derived, Both )
DECLARE_METHOD( Derived, OnlyDerived )


int main()
{
	Base base; /* как-то создали базовый класс */
	base.a = 0; // работаем как со структурой
	Derived derived; /* ... как-то создали наследник */
    derived.a = 123;
    derived.b = 456;
	
	// полиморфизм
    Base* reallyDerived = reinterpret_cast<Base*>(&derived);

	VIRTUAL_CALL(&base, Both); // печатает “Base::Both”
	VIRTUAL_CALL(reallyDerived, Both); // печатает “Derived::Both”
	VIRTUAL_CALL(reallyDerived, OnlyBase);  // печатает “Base::OnlyBase”
	VIRTUAL_CALL(reallyDerived, OnlyDerived); // печатает Derived::OnlyDerived”
    VIRTUAL_CALL(&base, OnlyDerived); // кидает exception
}
