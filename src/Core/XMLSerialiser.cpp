// Precompiled Header
#include "Core/StdAfx.h"

#include "Core/XMLSerialiser.h"
#include "Core/XMLDeserialiser.h"

#include "Core/LogManager.h"
#include "Core/Type.h"
#include "Core/TypeDB.h"
#include "Core/Field.h"
#include "Core/TypeStringifier.h"

using namespace BH;

// Constructor
XMLSerialiser::XMLSerialiser()
: Serialiser()
, mDoc()
, mFileName()
{
}

// Destructor
XMLSerialiser::~XMLSerialiser()
{
	if ( mFileName != "" )
	{
		if ( !SaveFile( mFileName, mDoc ) )
		{
			LOGERROR( "XML Serialiser failed to save file: (" << mFileName << ")" );
		}
	}
}

// Traverse the stream specified by the path.
XMLSerialiser::StreamPtr XMLSerialiser::TraverseStream( const BH::Char* path )
{
	mFileName = path;
	mStream = &mDoc;

	XMLDeserialiser::LoadFile( path, mDoc );

	return &mDoc;
}

// Actual serialise method.
XMLSerialiser::StreamPtr XMLSerialiser::DoSerialise( StreamPtr s,						//!< Stream Pointer
													 const BH::Type* type,				//!< Type of the object
													 const void* obj,					//!< Object
													 const BH::FieldAttributes* attr,	//!< Attributes
													 const BH::Field* field				//!< Field
													)
{
	Entity::OPTIONS opt = Entity::O_NORMAL;

	if ( type->IsEntity ) 
	{
		opt = reinterpret_cast<ConstEntityPtr>( obj )->OnPreSaved();

		if ( opt == Entity::O_SKIP )
			return s;
	}

	// Type
	SerialiseElement * current = SetType( s, type );

	// If built in type or serialiser for type is specified, we are done for the type
	if ( SetValue( current, type, obj, attr, field ) )
		return s;

	// Get all fields from current type
	std::stack<const Type::Fields*> fieldStack = type->GetAllFields();

	while ( !fieldStack.empty() && ( opt != Entity::O_IGNORE_FIELDS ) )
	{
		for ( const auto & field : ( *fieldStack.top() ) )
		{
			// If field not serialisable
			if ( field.Attributes.Transient != 0 )
				continue;

			// Get a pointer to the data member.
			const void* dataPtr = field.IsPointer ? *reinterpret_cast<const void **>( field.GetPtr( obj ) ) : field.GetPtr( obj );

			// Try to serialize if container
			if ( field.Container ) 
			{ 
				const Type* keyType = field.Container->KeyType;
				const Type* valType = field.Container->ValueType;

				bool keyEntity = keyType ? keyType->IsEntity : false;
				bool valEntity = valType->IsEntity;

				const void * keyPtr = nullptr,
						   * valPtr = nullptr;

				const Type * keyEntryType = nullptr,
						   * valEntryType = nullptr;

				// Work on the container.
				{
					BH::FieldReadIterator* it = NEW_FIELD_READIT(field.Container, dataPtr);

					const Type* ftype = ( field.Type->IsEntity && &reinterpret_cast<ConstEntityPtr>( dataPtr )->GetType() )
										? &reinterpret_cast<ConstEntityPtr>( dataPtr )->GetType()
										: field.Type;
					SerialiseElement * cont_current = SetType( current, ftype );
					cont_current = SetContainer( cont_current, it->Count() );
					
					//bool keyOutType = field.Container->IsKeyPointer && keyType && keyType->IsEntity;
					//bool valOutType = field.Container->IsValuePointer && valType->IsEntity;

					for ( ; it->IsValid(); it->IncNext() ) 
					{
						if ( keyType ) 
						{
							// Get key pointer (Data)
							keyPtr = field.Container->IsKeyPointer 
									 ? *reinterpret_cast<const void * const *>( it->GetKey() ) 
									 : it->GetKey();

							// Get key type
							keyEntryType = ( keyEntity && &reinterpret_cast<ConstEntityPtr>( keyPtr )->GetType() ) 
										   ? &reinterpret_cast<ConstEntityPtr>( keyPtr )->GetType() 
										   : keyType;
						}

						// Get value pointer (Data)
						valPtr = field.Container->IsValuePointer ? *reinterpret_cast<const void * const *>( it->GetValue() ) : it->GetValue();
						
						// Get value type
						valEntryType = ( valEntity && &reinterpret_cast<ConstEntityPtr>( valPtr )->GetType() ) 
									   ? &reinterpret_cast<ConstEntityPtr>( valPtr )->GetType()
									   : valType;

						if ( keyType ) 
						{
							void * keynode = DoSerialise( cont_current, keyEntryType, keyPtr, &field.Attributes, &field );
							DoSerialise( keynode, valEntryType, valPtr, &field.Attributes, &field );
						}
						else
						{
							DoSerialise( cont_current, valEntryType, valPtr, &field.Attributes, &field );
						}

					} 
					
					DEL_FIELD_IT( field.Container, it );
				}
			}
			else 
			{
				const Type* ftype = ( field.Type->IsEntity && &reinterpret_cast<ConstEntityPtr>( dataPtr )->GetType()  )
									? &reinterpret_cast<ConstEntityPtr>( dataPtr )->GetType() 
									: field.Type;

				DoSerialise( current, ftype, dataPtr, &field.Attributes, &field );
			}
		}

		fieldStack.pop();
	}

	if ( type->IsEntity )
		reinterpret_cast<ConstEntityPtr>( obj )->OnSaved();

	return s;
}

bool XMLSerialiser::SetValue( SerialiseElement* s, const BH::Type* type, const void* obj, const BH::FieldAttributes* attr, const BH::Field * field ) 
{
	if ( TypeStringifier::ToString tostr = TYPE_STRINGIFIER.GetToString( type ) ) 
	{
		BH::String value;
		tostr( value, type, obj, attr );

		BH::String name = ( field ) ? field->Name.Text : "Value";
		SetAttribute( name, value, s );

		// Handled the value
		return true;
	}

	return false;
}

SerialiseElement * XMLSerialiser::SetType( StreamPtr s, const BH::Type* type )
{
	// If current node is doc
	if ( s == &mDoc )
		return PushElem( type->Name.Text, mDoc );
	// If current node is a element in doc
	else
		return PushElem( type->Name.Text, reinterpret_cast<SerialiseElement *>( s ), mDoc );
}

SerialiseElement * XMLSerialiser::SetContainer( SerialiseElement * s, BH::u32 size )
{
	SetAttribute( "Size", size, s );
	return s;
}