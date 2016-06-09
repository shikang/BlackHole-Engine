#ifndef BH_FUNCTIONS_FRAMWORK_H
#define BH_FUNCTIONS_FRAMWORK_H

// Disable warning
#pragma warning( disable:4251 )

// Wrapper macros for get set functions

// -------------Getter framework----------------

/*
    Using Getter method macro for defining an getter method.

    Usage:

    class GetterTest
    {
    public:
        GETTER( int, Member );

        GETTER_PTR( int, PtrMember )
        {
            std::cout << "Return pointer." << std::endl;
            return p_Mem;
        }
        GETTER_REF( int, RefMember )
        {
            std::cout << "Return by reference." << std::endl;
            return member;
        }
        GETTER_CREF( int, CRefMember )
        {
            std::cout << "Return by const reference." << std::endl;
            return member;
        }
    private:
        int member;
        int * p_Mem;
    }

    // External declaration.
    GETTER_DEF( GetterTest, int, Member )
    {
        std::cout << "Return by value." << std::endl;
        return _member;
    }
    
    GetterTest test;
    test.GetMember();
    test.GetPtrMember();
    test.GetRefMember();
    test.GetCRefMember();
 */

// Macro for standard getter
#define GETTER( type, getter_name )\
	inline const type Get##getter_name() const

// Macro for standard getter of pointer member
#define GETTER_PTR( type, getter_name )\
	inline type * Get##getter_name()

// Macro for standard getter of const pointer member
#define GETTER_CPTR( type, getter_name )\
	inline const type * Get##getter_name() const

// Macro for standard getter of reference member
#define GETTER_REF( type, getter_name )\
	inline type & Get##getter_name()

// Macro for standard getter of const reference member
#define GETTER_CREF( type, getter_name )\
	inline const type & Get##getter_name() const

// Macro for standard getter (Defination header of function outside class body)
#define GETTER_DEF( class_type, type, getter_name )\
	const type class_type::Get##getter_name() const

// Macro for standard getter of pointer member (Defination header of function outside class body)
#define GETTER_PTR_DEF( class_type, type, getter_name )\
	inline type * class_type::Get##getter_name()

// Macro for standard getter of const pointer member (Defination header of function outside class body)
#define GETTER_CPTR_DEF( class_type, type, getter_name )\
	inline const type * class_type::Get##getter_name() const

// Macro for standard getter of reference member (Defination header of function outside class body)
#define GETTER_REF_DEF( class_type, type, getter_name )\
	inline type & class_type::Get##getter_name()

// Macro for standard getter of const reference member (Use it to define the function outside class body)
#define GETTER_CREF_DEF( class_type, type, getter_name )\
	inline const type & class_type::Get##getter_name() const

// -----------Default Getter BH-------------
/*
    Getter method macro for defining a default accessor/getter method.

    Usage:

    class GetterTest
    {
    public:
        GETTER_DEFAULT( int, Member, _member );
        GETTER_DEFAULT_REF( int, RefMember, _member );
        GETTER_DEFAULT_CREF( int, CRefMember, _member );
    
    private:
        int _member;
    }
    
    GetterTest test;
    test.GetMember();
    test.GetRefMember();
    test.GetCRefMember();
*/

// Macro for default standard getter
#define GETTER_DEFAULT( type, getter_name, member )\
	    GETTER( type, getter_name ) { return member; }

// Macro for default standard getter of pointer member
#define GETTER_DEFAULT_PTR( type, getter_name, member )\
	    GETTER_PTR( type, getter_name ) { return member; }

// Macro for default standard getter of const pointer member
#define GETTER_DEFAULT_CPTR( type, getter_name, member )\
	    GETTER_CPTR( type, getter_name ) { return member; }

// Macro for default standard getter of reference member
#define GETTER_DEFAULT_REF( type, getter_name, member )\
	    GETTER_REF( type, getter_name ) { return member; }

// Macro for default standard getter of const reference member
#define GETTER_DEFAULT_CREF( type, getter_name, member )\
	    GETTER_CREF( type, getter_name ) { return member; }


// -----------Auto Getter BH-----------------
// Macro for auto standard getter
#define GETTER_AUTO( type, getter_name )\
	    GETTER_DEFAULT( type, getter_name, m##getter_name )

// Macro for auto standard getter of pointer member
#define GETTER_AUTO_PTR( type, getter_name )\
	    GETTER_DEFAULT_PTR( type, getter_name, m##getter_name )

// Macro for auto standard getter of const pointer member
#define GETTER_AUTO_CPTR( type, getter_name )\
	    GETTER_DEFAULT_CPTR( type, getter_name, m##getter_name )

// Macro for auto standard getter of reference member
#define GETTER_AUTO_REF( type, getter_name )\
	    GETTER_DEFAULT_REF( type, getter_name, m##getter_name )

// Macro for auto standard getter of const reference member
#define GETTER_AUTO_CREF( type, getter_name )\
	    GETTER_DEFAULT_CREF( type, getter_name, m##getter_name )

// ---------Setter BH--------------
// Macro for standard setter
#define SETTER( type, setter_name )\
	    inline void Set##setter_name( const type & value )

// Macro for standard setter with no inline keyword
#define SETTER_NO_INLINE( type, setter_name )\
	    void Set##setter_name( const type & value )

// Macro for standard setter (Defination header of function outside class body)
#define SETTER_DEF( class_type, type, setter_name )\
	    void class_type::Set##setter_name( const type & value )

// Macro for default standard setter
#define SETTER_DEFAULT( type, setter_name, member )\
	    SETTER( type, setter_name ) { member = value; }

// Macro for auto standard setter
#define SETTER_AUTO( type, setter_name )\
	    SETTER_DEFAULT( type, setter_name, m##setter_name )

// ----------- Getter + Setter BH ------------
// Default getter and setter methods.
#define GETSET_DEFAULT( type, getset_name, member )\
	    GETTER_DEFAULT( type, getset_name, member )\
	    SETTER_DEFAULT( type, getset_name, member )

// Default reference getter and setter methods.
#define GETSET_DEFAULT_REF( type, getset_name, member )\
	    GETTER_DEFAULT_REF( type, getset_name, member )\
	    SETTER_DEFAULT( type, getset_name, member )

// Default const reference getter and setter methods.
#define GETSET_DEFAULT_CREF( type, getset_name, member )\
	    GETTER_DEFAULT_CREF( type, getset_name, member )\
	    SETTER_DEFAULT( type, getset_name, member )

// Auto getter and setter methods.
#define GETSET_AUTO( type, getset_name )\
	    GETTER_AUTO( type, getset_name )\
	    SETTER_AUTO( type, getset_name )

// Auto reference getter and setter methods.
#define GETSET_AUTO_REF( type, getset_name )\
	    GETTER_AUTO_REF( type, getset_name )\
	    SETTER_AUTO( type, getset_name )

// Auto const reference getter and setter methods.
#define GETSET_AUTO_CREF( type, getset_name )\
	    GETTER_AUTO_CREF( type, getset_name )\
	    SETTER_AUTO( type, getset_name )

// Helper Macro for template class definition
#define CLASS_TEMPLATE_PARAM(classtype, ...)\
	classtype<__VA_ARGS__>

// Macro for function declaration
#define AUTO_DECLARE_FN(name, returntype, ...)\
	returntype name(__VA_ARGS__)

// Macro for function definition
#define AUTO_DEFINE_FN(name, classtype, returntype, ...)\
	returntype classtype::name(__VA_ARGS__)

// Macro for abstract function declaration
#define AUTO_DECLARE_EMPTY_ABSTRACT_FN(name, returntype, ...)\
	returntype name(__VA_ARGS__) = 0

// Macro for function declaration with scope
#define AUTO_DECLARE_EMPTY_FN(name, returntype, ...)\
	returntype name(__VA_ARGS__) {}

// Safe Access Ptr check before dereference
#define SAFE_ACCESS_PTR(ptr) \
	if (!ptr) {} else ptr

#endif