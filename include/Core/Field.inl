#include "Core/ArrayFieldContainer.h"
#include "Core/VectorFieldContainer.h"
#include "Core/SetFieldContainer.h"
#include "Core/MapFieldContainer.h"

namespace BH
{
	template< typename ENTITY_TYPE, typename FIELD_TYPE >
	inline Field::Field( const CName & name, const BH::Type * type, FIELD_TYPE ENTITY_TYPE::*field, const FieldAttributes & attrs, const Char * group, const Char * desc )
	: Type( type )
	, IsPointer( std::is_pointer<FIELD_TYPE>::value )
	, Offset( offsetof( ENTITY_TYPE, *field ) )
	, Name( name )
	, Attributes( attrs )
	, Group( group )
	, Desc( desc )
	{
		Container = std::shared_ptr<FieldContainer>( detail::CreateContainer( ( ( ENTITY_TYPE * )0 )->*field ) );
	}

	inline Field::Field( const CName & name, const BH::Type * type, size_t offset, bool isPointer, const FieldAttributes & attrs, std::shared_ptr<FieldContainer> container, const Char * group, const Char * desc )
	: Type( type )
	, IsPointer( isPointer )
	, Offset( offset )
	, Name( name )
	, Attributes( attrs )
	, Group( group )
	, Desc( desc )
	, Container( container )
	{
	}

	template< typename FIELD_TYPE >
	inline Field::Field( const CName & name, const BH::Type * type, size_t offset, bool isPointer, FIELD_TYPE & f, const FieldAttributes & attrs, const Char * group, const Char * desc )
	: Type( type )
	, IsPointer( isPointer )
	, Offset( offset )
	, Name( name )
	, Attributes( attrs )
	, Group( group )
	, Desc( desc )
	{
		Container = std::shared_ptr<FieldContainer>( detail::CreateContainer( f ) );
	}
}