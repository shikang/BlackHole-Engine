// Precompiled Header
#include "Core/StdAfx.h"

#include "Core/LogManager.h"
#include "Core/Type.h"
#include "Core/TypeDB.h"
#include "Core/Field.h"
#include "Core/TypeStringifier.h"

#include "Core/XMLDeserialiser.h"

namespace BH
{
	// Constructor
	XMLDeserialiser::XMLDeserialiser()
	: Deserialiser()
	, mDoc()
	, mFileName()
	{
	}

	// Destructor
	XMLDeserialiser::~XMLDeserialiser()
	{
	}

	bool XMLDeserialiser::IsStreamEnded() const
	{
		void * s = mStream;
		if ( mStream )
			return GetElem( "", reinterpret_cast<SerialiseElement*>( s ) ) == nullptr;
		else
			return true;
	}

	// Traverse the stream specified by the path.
	XMLDeserialiser::StreamPtr XMLDeserialiser::TraverseStream( const BH::Char* path )
	{
		mFileName = path;

		if ( !LoadFile( path, mDoc ) )
		{
			LOGERROR( "XML Deserialiser failed to load file: (" << path << ")" );
			return nullptr;
		}

		mStream = GetElem( "", mDoc );
		return mStream;
	}

	// Actual deserialise method.
	XMLDeserialiser::StreamPtr XMLDeserialiser::DoDeserialise( StreamPtr s,						//!< Stream Pointer
															   const BH::Type* type,			//!< Type of object
															   void *& obj,						//!< Object
															   const BH::FieldAttributes* attr,	//!< Attributes
															   const BH::Field* field,			//!< Field
															   bool inferType					//!< If true, get type through deserialisation
															   )
	{
		if ( !type )
		{
			LOGERROR( "Type not specified." );
			return s;
		}

		CName type_name;
		SerialiseElement * current = GetType( s, type_name );
		SerialiseElement * next = GetNextElem( current );

		// No more next elem
		if ( !current )
			return nullptr;

		if ( inferType )
		{
			const Type* t = ENTITY_MANAGER.Types.Get( type_name );

			// No type found
			if ( !t )
			{
				LOGERROR( "Unable to determine type for " << type->Name.Text << "." );
				type = nullptr;
			}
			// Type gotten is not derived from current type
			else if ( !t->IsType( type->Name.Text ) )
			{
				LOGERROR( t->Name.Text << " is not a type of " << type->Name.Text );
				type = nullptr;
			}
			else
			{
				type = t;
			}
		}

		// If obj is null, create (Meaning, field is a pointer)
		if ( type && !obj )
			obj = ENTITY_MANAGER.New( type );

		if ( !obj )
		{
			LOGERROR( "Unable to create type: " << type->Name.Text );
			return nullptr;
		}


		Entity::OPTIONS opt = Entity::O_NORMAL;
		if ( type->IsEntity )
		{
			opt = reinterpret_cast<EntityPtr>( obj )->OnPreChanged();

			if ( opt == Entity::O_SKIP )
				return next;
		}

		// If built in type or deserialiser for type is specified, we are done for the type
		if ( GetValue( current, type, obj, attr, field ) )
			return next;

		// If field not serialise, ignore
		SerialiseElement * field_current = GetElem( "", current );

		std::stack<const Type::Fields*> fieldStack = type->GetAllFields();

		while ( !fieldStack.empty() && ( opt != Entity::O_IGNORE_FIELDS ) )
		{
			for ( const auto & field : ( *fieldStack.top() ) )
			{
				// No available data for this field
				if ( !field_current )
					return next;

				// Serialize container.
				if ( field.Attributes.Transient != 0 )
					continue;

				// Get a pointer to the data member.
				void * dataPtr = field.IsPointer ? *reinterpret_cast<void **>( field.GetPtr( obj ) ) : field.GetPtr( obj );

				// Try deserialise if container
				if ( field.Container )
				{
					const Type* keyType = field.Container->KeyType;
					const Type* valType = field.Container->ValueType;
					BH::u32 size = 0;

					if ( BH::FieldWriteIterator* it = NEW_FIELD_WRITEIT( field.Container, dataPtr ) )
					{
						field_current = GetContainer( field_current, size );
						//SerialiseElement * cont_current = GetElem("", field_current);
						void * cont_current = GetElem( "", field_current );

						bool keyInferType = field.Container->IsKeyPointer && keyType && keyType->IsEntity;
						bool valInferType = field.Container->IsValuePointer && valType->IsEntity;

						//for ( SerialiseElement * cont_current = GetElem("", field_current); cont_current; cont_current = GetNextElem(cont_current) )
						while ( cont_current )
						{
							void * newKey = nullptr,
								 * newVal = nullptr;

							if ( keyType )
							{
								SerialiseElement * key_node = GetElem( "", reinterpret_cast<SerialiseElement*>( cont_current ) );
								cont_current = DoDeserialise( cont_current, keyType, newKey, &field.Attributes, &field, keyInferType );
								DoDeserialise( key_node, valType, newVal, &field.Attributes, &field, valInferType );
							}
							else
							{
								// @hack: Field belongs to object and type is component
								//if ( type->Name == ENTITY_MANAGER.Types.Get<Object>()->Name && 
								//	 valType->Name == ENTITY_MANAGER.Types.Get<ObjectComponent>()->Name )
								//	 ;

								cont_current = DoDeserialise( cont_current, valType, newVal, &field.Attributes, &field, valInferType );
							}

							if ( newKey && newVal )
							{
								it->Add( field.Container->IsKeyPointer ? reinterpret_cast<void*>( &newKey ) : newKey,
									     field.Container->IsValuePointer ? reinterpret_cast<void*>( &newVal ) : newVal );
							}
							else
							{
								it->Add( field.Container->IsValuePointer ? reinterpret_cast<void*>( &newVal ) : newVal );
							}

							if ( newKey && !field.Container->IsKeyPointer )
							{
								if ( keyType->IsEntity )
									ENTITY_MANAGER.Delete( reinterpret_cast<EntityPtr>( newKey ) );
								else
									keyType->Delete( newKey );
							}

							if ( newVal && !field.Container->IsValuePointer )
							{
								if ( valType->IsEntity )
									ENTITY_MANAGER.Delete( reinterpret_cast<EntityPtr>( newVal ) );
								else
									valType->Delete( newVal );
							}
						}
						DEL_FIELD_IT( field.Container, it );
					}

					field_current = GetNextElem( field_current );
				}
				else
				{
					// Otherwise just deserialize as per normal.
					if ( field.IsPointer )
					{
						if ( *reinterpret_cast<void **>( field.GetPtr( obj ) ) == nullptr )
						{
							void * newObj = nullptr;
							field_current = reinterpret_cast<SerialiseElement *>( DoDeserialise( field_current, field.Type, newObj, &field.Attributes, &field, field.Type->IsEntity ) );
							*reinterpret_cast<void **>( field.GetPtr( obj ) ) = newObj;
						}
						else
						{
							field_current = reinterpret_cast<SerialiseElement *>( DoDeserialise( field_current, field.Type, *reinterpret_cast<void **>( field.GetPtr( obj ) ), &field.Attributes, &field, field.Type->IsEntity ) );
						}
					}
					else
					{
						field_current = reinterpret_cast<SerialiseElement *>( DoDeserialise( field_current, field.Type, dataPtr, &field.Attributes, &field, false ) );
					}
				}
			}
			fieldStack.pop();
		}

		if ( type->IsEntity )
			reinterpret_cast<EntityPtr>( obj )->OnChanged();

		return next;
	}

	// Write value to current node
	bool XMLDeserialiser::GetValue( SerialiseElement * s, const BH::Type * type, void* obj, const BH::FieldAttributes * attr, const BH::Field * field )
	{
		if ( TypeStringifier::FmString fmstr = TYPE_STRINGIFIER.GetFromString( type ) )
		{
			BH::String value;
			BH::String name = ( field ) ? field->Name.Text : "Value";

			// If successfully gotten the value
			if ( GetAttribute( name, s, value ) )
			{
				fmstr( value, type, obj, attr );

				// Handled the value
				return true;
			}
		}

		return false;
	}

	SerialiseElement * XMLDeserialiser::GetType( StreamPtr s, BH::CName & type_name )
	{
		if ( s )
			type_name = GetElemName( reinterpret_cast<SerialiseElement *>( s ) );

		return reinterpret_cast<SerialiseElement *>( s );
	}

	// Write the container size to current noded
	SerialiseElement * XMLDeserialiser::GetContainer( SerialiseElement * s, BH::u32 & size )
	{
		GetAttribute( "Size", s, size );
		return s;
	}
}