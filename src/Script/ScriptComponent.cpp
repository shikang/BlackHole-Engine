// Precompiled Header
#include "Script/ScriptComponentStdafx.h"

#include "Script/ScriptComponent.h"
#include "Script/Script.h"
#include "Script/ScriptInterOp.h"

#include "Core/Hash.h"
#include "Core/TypeDB.h"
#include "Core/Assert.h"
#include "Core/Object.h"

namespace
{
	static BH::Type * ScriptComponentType = nullptr;
	static BH::CName ReflectionDummyName( "Data" );
}

namespace BH
{
	Object ** ScriptType::ChangingObject = nullptr;

	void ScriptType::SetChangingObject( Object ** changingObj )
	{
		ChangingObject = changingObj;
	}

	Entity::OPTIONS ScriptType::OnPreChanged() 
	{ 
		return Entity::O_NORMAL; 
	}

	void ScriptType::OnChanged() 
	{
		if ( mParent && mType != "[InvalidType]" && !mParent->mInitialised )
		{
			// Release data
			mParent->OnChanged();

			if( !mParent->mObject && ChangingObject )
				mParent->mObject = *ChangingObject;

			mParent->Initialise();
			mParent->OnPreChanged();
		}
	}

	Entity::OPTIONS ScriptType::OnPreSaved() const 
	{ 
		return Entity::O_NORMAL; 
	}

	void ScriptType::OnSaved() const 
	{
	}

	TypeDB * ScriptComponent::Types = nullptr;
	std::function< void(ScriptComponent *) > ScriptComponent::InitialiseCallback;
	std::function< void(ScriptComponent *) > ScriptComponent::ShutdownCallback;

	ScriptComponent::ScriptComponent()
	: mData( nullptr )
	, mDummyType( nullptr )
	, mScript( "[InvalidType]" )
	, mInitialised( false )
	, mRefreshTimer( 0.0f )
	{
		BH_ASSERT( Types, "ScriptComponent::Types not assigned!" );

		if ( !ScriptComponentType )
			ScriptComponentType = const_cast< Type * >( Types->Get<ScriptComponent>() );

		mScript.mParent = this;
	}

	ScriptComponent::~ScriptComponent()
	{
		Shutdown();
	}

	void ScriptComponent::SetTypeDB( TypeDB * types )
	{
		Types = types;
	}

	void ScriptComponent::SetInitialiseCallback( const std::function< void(ScriptComponent *) > & callback )
	{
		InitialiseCallback = callback;
	}

	void ScriptComponent::SetShutdownCallback( const std::function< void(ScriptComponent *) > & callback )
	{
		ShutdownCallback = callback;
	}

	void ScriptComponent::Initialise()
	{
		mScript.mParent = this;
		InitialiseScript();
	}

	void ScriptComponent::InitialiseScript()
	{
		if ( mScript.mType == "[InvalidType]" || mInitialised )
			return;

		mDummyType = Types->Get( CName( mScript.mType.c_str() ) );
		mScriptObjectName = mObject->GetName() + mScript.mType;
		mScriptObject = Script::CreateScriptObject( mScript.mType.c_str(), mScriptObjectName.c_str() );
		mScriptObject.CallMethod( "SetObject", mObject->GetName().c_str(), mScript.mType.c_str() );
		mScriptObject.CallMethod( "Initialise" );
		mInitialised = true;

		if ( InitialiseCallback )
			InitialiseCallback( this );
	}

	void ScriptComponent::Update( f32 dt )
	{
		if ( mScript.mType == "[InvalidType]" || !mInitialised )
			return;
		
		mScriptObject.CallMethod( "Update", dt );
		//RefreshScriptValue( dt );
	}

	void ScriptComponent::Draw( f32 dt )
	{
		if ( mScript.mType == "[InvalidType]" || !mInitialised )
			return;
		
		mScriptObject.CallMethod( "Draw", dt );
		RefreshScriptValue( dt );
	}

	void ScriptComponent::RefreshScriptValueByField( const Field * field )
	{
		// Refresh built in type only (Normally they are the more important one)
		if ( field->Type->Name == TypeCName<String>() )
		{
			detail::ScriptInterOp::SetValueToScript<String>( mData, field, &mScriptObject );
		}
		else if ( field->Type->Name == TypeCName<bool>() )
		{
			detail::ScriptInterOp::SetValueToScript<bool>( mData, field, &mScriptObject );
		}
		else if ( field->Type->Name == TypeCName<s8>() )
		{
			detail::ScriptInterOp::SetValueToScript<s8>( mData, field, &mScriptObject );
		}
		else if ( field->Type->Name == TypeCName<u8>() )
		{
			detail::ScriptInterOp::SetValueToScript<u8>( mData, field, &mScriptObject );
		}
		else if ( field->Type->Name == TypeCName<s16>() )
		{
			detail::ScriptInterOp::SetValueToScript<s16>( mData, field, &mScriptObject );
		}
		else if ( field->Type->Name == TypeCName<u16>() )
		{
			detail::ScriptInterOp::SetValueToScript<u16>( mData, field, &mScriptObject );
		}
		else if ( field->Type->Name == TypeCName<s32>() )
		{
			detail::ScriptInterOp::SetValueToScript<s32>( mData, field, &mScriptObject );
		}
		else if ( field->Type->Name == TypeCName<u32>() )
		{
			detail::ScriptInterOp::SetValueToScript<u32>( mData, field, &mScriptObject );
		}
		else if ( field->Type->Name == TypeCName<s64>() )
		{
			detail::ScriptInterOp::SetValueToScript<s64>( mData, field, &mScriptObject );
		}
		else if ( field->Type->Name == TypeCName<u64>() )
		{
			detail::ScriptInterOp::SetValueToScript<u64>( mData, field, &mScriptObject );
		}
		else if ( field->Type->Name == TypeCName<f32>() )
		{
			detail::ScriptInterOp::SetValueToScript<f32>( mData, field, &mScriptObject );
		}
		else if ( field->Type->Name == TypeCName<f64>() )
		{
			detail::ScriptInterOp::SetValueToScript<f64>( mData, field, &mScriptObject );
		}
		else if ( field->Container )
		{
			if ( field->Container->ValueType->Name == TypeCName<String>() )
			{
				detail::ScriptInterOp::SetValueToScript< std::vector<String> >( mData, field, &mScriptObject );
			}
			else if ( field->Container->ValueType->Name == TypeCName<s8>() )
			{
				detail::ScriptInterOp::SetValueToScript< std::vector<s8> >( mData, field, &mScriptObject );
			}
			else if ( field->Container->ValueType->Name == TypeCName<u8>() )
			{
				detail::ScriptInterOp::SetValueToScript< std::vector<u8> >( mData, field, &mScriptObject );
			}
			else if ( field->Container->ValueType->Name == TypeCName<s16>() )
			{
				detail::ScriptInterOp::SetValueToScript< std::vector<s16> >( mData, field, &mScriptObject );
			}
			else if ( field->Container->ValueType->Name == TypeCName<u16>() )
			{
				detail::ScriptInterOp::SetValueToScript< std::vector<u16> >( mData, field, &mScriptObject );
			}
			else if ( field->Container->ValueType->Name == TypeCName<s32>() )
			{
				detail::ScriptInterOp::SetValueToScript< std::vector<s32> >( mData, field, &mScriptObject );
			}
			else if ( field->Container->ValueType->Name == TypeCName<u32>() )
			{
				detail::ScriptInterOp::SetValueToScript< std::vector<u32> >( mData, field, &mScriptObject );
			}
			else if ( field->Container->ValueType->Name == TypeCName<s64>() )
			{
				detail::ScriptInterOp::SetValueToScript< std::vector<s64> >( mData, field, &mScriptObject );
			}
			else if ( field->Container->ValueType->Name == TypeCName<u64>() )
			{
				detail::ScriptInterOp::SetValueToScript< std::vector<u64> >( mData, field, &mScriptObject );
			}
			else if ( field->Container->ValueType->Name == TypeCName<f32>() )
			{
				detail::ScriptInterOp::SetValueToScript< std::vector<f32> >( mData, field, &mScriptObject );
			}
			else if ( field->Container->ValueType->Name == TypeCName<f64>() )
			{
				detail::ScriptInterOp::SetValueToScript< std::vector<f64> >( mData, field, &mScriptObject );
			}
			else
			{
				detail::ScriptInterOp::SetValueToScript< std::vector<Char *> >( mData, field, &mScriptObject );
			}
		}
		else
		{
			std::vector<const BH::Char *> fieldNames { field->Name.Text };
			detail::ScriptInterOp::SetScriptTypeValueFromScript( mData, field, fieldNames, &mScriptObject );
		}
	}

	void ScriptComponent::RefreshScriptValue( f32 dt )
	{
		// If on editor ( refresh built in type value )
		if ( mData )
		{
			if ( mDummyType )
			{
				mRefreshTimer += dt;

				if ( mRefreshTimer < 0.5f )
					return;

				mRefreshTimer = 0.0f;

				std::stack<const Type::Fields *> fields = mDummyType->GetAllFields();

				if ( !fields.empty() )
				{
					// There should not be a base type, therefore stack size should be 1
					for ( const auto & field : *fields.top() )
					{
						RefreshScriptValueByField( &field );
					}
				}
			}
		}
	}

	void ScriptComponent::Shutdown()
	{
		if ( mScript.mType == "[InvalidType]" || !mInitialised )
			return;

		mScriptObject.CallMethod( "Shutdown" );
		Script::DestroyScriptObject( mScriptObject );

		if ( ShutdownCallback )
			ShutdownCallback( this );

		mInitialised = false;
	}

	void ScriptComponent::ResetObject()
	{
		if ( mScript.mType == "[InvalidType]" || !mInitialised )
			return;

		mScriptObject.CallMethod( "SetObject", mObject->GetName().c_str(), mScript.mType.c_str() );
	}

	void ScriptComponent::GetValuesFromScript() const
	{
		std::stack<const Type::Fields *> fields = mDummyType->GetAllFields();
		// There should not be a base type, therefore stack size should be 1
		if ( !fields.empty() )
		{
			for ( const auto & field : *fields.top() )
			{
				GetScriptValueByField( &field, mData, &mScriptObject );
			}
		}
	}

	bool ScriptComponent::IsScriptType( const String & scriptType )
	{
		return Script::IsScriptType( scriptType.c_str() );
	}

	void ScriptComponent::SetScriptType( const String & type )
	{
		mScript.mType = type;
	}

	std::vector<String> ScriptComponent::GetAllScriptTypes()
	{
		std::vector<String> scriptTypes = Script::GetAllScriptTypes();
		std::vector<String> engineScriptTypes = Script::GetAllEngineScriptTypes();

		for ( const auto & i : engineScriptTypes )
		{
			std::vector<String>::iterator it = std::find( scriptTypes.begin(), scriptTypes.end(), i );
			if ( it != scriptTypes.end() )
				scriptTypes.erase( it );
		}

		return scriptTypes;
	}

	void * ScriptComponent::GetDefaultConstructedScriptData( const String & scriptType, const String & uniqueName )
	{
		if( !Script::IsScriptType( scriptType.c_str() ) )
			return nullptr;

		const Type * scriptReflection = Types->Get( CName( scriptType.c_str() ) );

		if ( !scriptReflection )
			return nullptr;

		ScriptObject so = Script::CreateScriptObject( scriptType.c_str(), uniqueName.c_str() );
		Char * data = reinterpret_cast< Char * >( scriptReflection->New() );

		std::stack<const Type::Fields *> fields = scriptReflection->GetAllFields();
		// There should not be a base type, therefore stack size should be 1
		for ( const auto & field : *fields.top() )
		{
			GetScriptValueByField( &field, data, &so );
		}

		Script::DestroyScriptObject( so );
		return data;
	}

	void ScriptComponent::SetValuesToScript() const
	{
		std::stack<const Type::Fields *> fields = mDummyType->GetAllFields();

		if ( !fields.empty() )
		{
			// There should not be a base type, therefore stack size should be 1
			for ( const auto & field : *fields.top() )
			{
				if ( field.Type->Name == TypeCName<String>() )
				{
					detail::ScriptInterOp::SetValueToScript<String>( mData, &field, &mScriptObject );
				}
				else if ( field.Type->Name == TypeCName<bool>() )
				{
					detail::ScriptInterOp::SetValueToScript<bool>( mData, &field, &mScriptObject );
				}
				else if ( field.Type->Name == TypeCName<s8>() )
				{
					detail::ScriptInterOp::SetValueToScript<s8>( mData, &field, &mScriptObject );
				}
				else if ( field.Type->Name == TypeCName<u8>() )
				{
					detail::ScriptInterOp::SetValueToScript<u8>( mData, &field, &mScriptObject );
				}
				else if ( field.Type->Name == TypeCName<s16>() )
				{
					detail::ScriptInterOp::SetValueToScript<s16>( mData, &field, &mScriptObject );
				}
				else if ( field.Type->Name == TypeCName<u16>() )
				{
					detail::ScriptInterOp::SetValueToScript<u16>( mData, &field, &mScriptObject );
				}
				else if ( field.Type->Name == TypeCName<s32>() )
				{
					detail::ScriptInterOp::SetValueToScript<s32>( mData, &field, &mScriptObject );
				}
				else if ( field.Type->Name == TypeCName<u32>() )
				{
					detail::ScriptInterOp::SetValueToScript<u32>( mData, &field, &mScriptObject );
				}
				else if ( field.Type->Name == TypeCName<s64>() )
				{
					detail::ScriptInterOp::SetValueToScript<s64>( mData, &field, &mScriptObject );
				}
				else if ( field.Type->Name == TypeCName<u64>() )
				{
					detail::ScriptInterOp::SetValueToScript<u64>( mData, &field, &mScriptObject );
				}
				else if ( field.Type->Name == TypeCName<f32>() )
				{
					detail::ScriptInterOp::SetValueToScript<f32>( mData, &field, &mScriptObject );
				}
				else if ( field.Type->Name == TypeCName<f64>() )
				{
					detail::ScriptInterOp::SetValueToScript<f64>( mData, &field, &mScriptObject );
				}
				else if ( field.Container )
				{
					if ( field.Container->ValueType->Name == TypeCName<String>() )
					{
						detail::ScriptInterOp::SetValueToScript< std::vector<String> >( mData, &field, &mScriptObject );
					}
					else if ( field.Container->ValueType->Name == TypeCName<s8>() )
					{
						detail::ScriptInterOp::SetValueToScript< std::vector<s8> >( mData, &field, &mScriptObject );
					}
					else if ( field.Container->ValueType->Name == TypeCName<u8>() )
					{
						detail::ScriptInterOp::SetValueToScript< std::vector<u8> >( mData, &field, &mScriptObject );
					}
					else if ( field.Container->ValueType->Name == TypeCName<s16>() )
					{
						detail::ScriptInterOp::SetValueToScript< std::vector<s16> >( mData, &field, &mScriptObject );
					}
					else if ( field.Container->ValueType->Name == TypeCName<u16>() )
					{
						detail::ScriptInterOp::SetValueToScript< std::vector<u16> >( mData, &field, &mScriptObject );
					}
					else if ( field.Container->ValueType->Name == TypeCName<s32>() )
					{
						detail::ScriptInterOp::SetValueToScript< std::vector<s32> >( mData, &field, &mScriptObject );
					}
					else if ( field.Container->ValueType->Name == TypeCName<u32>() )
					{
						detail::ScriptInterOp::SetValueToScript< std::vector<u32> >( mData, &field, &mScriptObject );
					}
					else if ( field.Container->ValueType->Name == TypeCName<s64>() )
					{
						detail::ScriptInterOp::SetValueToScript< std::vector<s64> >( mData, &field, &mScriptObject );
					}
					else if ( field.Container->ValueType->Name == TypeCName<u64>() )
					{
						detail::ScriptInterOp::SetValueToScript< std::vector<u64> >( mData, &field, &mScriptObject );
					}
					else if ( field.Container->ValueType->Name == TypeCName<f32>() )
					{
						detail::ScriptInterOp::SetValueToScript< std::vector<f32> >( mData, &field, &mScriptObject );
					}
					else if ( field.Container->ValueType->Name == TypeCName<f64>() )
					{
						detail::ScriptInterOp::SetValueToScript< std::vector<f64> >( mData, &field, &mScriptObject );
					}
					else
					{
						detail::ScriptInterOp::SetValueToScript< std::vector<Char *> >( mData, &field, &mScriptObject );
					}
				}
				else
				{
					std::vector<const BH::Char *> fieldNames { field.Name.Text };
					detail::ScriptInterOp::SetScriptTypeValueFromScript( mData, &field, fieldNames, &mScriptObject );
					//void ** data = reinterpret_cast< void ** >( field.GetPtr( mData ) );
					//*data = field.Type->New();
				}
			}
		}
	}

	Entity::OPTIONS ScriptComponent::OnPreChanged()
	{
		Field * field = const_cast< Field * >( ScriptComponentType->GetField( ReflectionDummyName ) );

		//if ( mScript.mType != "[InvalidType]" && !mInitialised )
		//	InitialiseScript();

		mScript.mParent = this;
		mRefreshTimer = 0.0f;

		if ( mDummyType )
		{
			field->Type = mDummyType;

			if ( !mData )
			{
				mData = reinterpret_cast< Char * >( mDummyType->New() );

				// Get values from script
				GetValuesFromScript();
			}
		}
		else
		{
			field->Type = Types->Get<Char>();

			if ( !mData )
				//mData = new Char();
				mData = reinterpret_cast< Char * >( Types->Get<Char>()->New() );
		}

		return Entity::O_NORMAL;
	}

	void ScriptComponent::OnChanged()
	{
		Field * field = const_cast< Field * >( ScriptComponentType->GetField( ReflectionDummyName ) );
		field->Type = Types->Get<Char>();

		if ( mData )
		{
			if ( mDummyType )
			{
				// Set values to script
				SetValuesToScript();

				mDummyType->Delete( mData );
			}
			else
			{
				//delete mData;
				Types->Get<Char>()->Delete( mData );
			}

			mData = nullptr;
		}
	}

	Entity::OPTIONS ScriptComponent::OnPreSaved() const
	{
		Field * field = const_cast< Field * >( ScriptComponentType->GetField( ReflectionDummyName ) );

		//if ( mScript.mType != "[InvalidType]" && !mInitialised )
		//	InitialiseScript();

		if ( mDummyType )
		{
			field->Type = mDummyType;

			if ( !mData )
			{
				mData = reinterpret_cast< Char * >( mDummyType->New() );

				// Get values from script
				GetValuesFromScript();
			}
		}
		else
		{
			field->Type = Types->Get<Char>();

			if ( !mData )
				//mData = new Char();
				mData = reinterpret_cast< Char * >( Types->Get<Char>()->New() );
		}

		return Entity::O_NORMAL;
	}

	void ScriptComponent::OnSaved() const
	{
		Field * field = const_cast< Field * >( ScriptComponentType->GetField( ReflectionDummyName ) );
		field->Type = Types->Get<Char>();

		if ( mData )
		{
			if ( mDummyType )
			{
				// Set values to script
				SetValuesToScript();

				mDummyType->Delete( mData );
			}
			else
			{
				//delete mData;
				Types->Get<Char>()->Delete( mData );
			}

			mData = nullptr;
		}
	}

	void ScriptComponent::GetScriptValueByField( const Field * field, void * data, ScriptObject * scriptObject )
	{
		if ( field )
		{
			if ( field->Type->Name == TypeCName<String>() )
			{
				detail::ScriptInterOp::GetValueFromScript<String>( data, field, scriptObject );
			}
			else if ( field->Type->Name == TypeCName<bool>() )
			{
				detail::ScriptInterOp::GetValueFromScript<bool>( data, field, scriptObject );
			}
			else if ( field->Type->Name == TypeCName<s8>() )
			{
				detail::ScriptInterOp::GetValueFromScript<s8>( data, field, scriptObject );
			}
			else if ( field->Type->Name == TypeCName<u8>() )
			{
				detail::ScriptInterOp::GetValueFromScript<u8>( data, field, scriptObject );
			}
			else if ( field->Type->Name == TypeCName<s16>() )
			{
				detail::ScriptInterOp::GetValueFromScript<s16>( data, field, scriptObject );
			}
			else if ( field->Type->Name == TypeCName<u16>() )
			{
				detail::ScriptInterOp::GetValueFromScript<u16>( data, field, scriptObject );
			}
			else if ( field->Type->Name == TypeCName<s32>() )
			{
				detail::ScriptInterOp::GetValueFromScript<s32>( data, field, scriptObject );
			}
			else if ( field->Type->Name == TypeCName<u32>() )
			{
				detail::ScriptInterOp::GetValueFromScript<u32>( data, field, scriptObject );
			}
			else if ( field->Type->Name == TypeCName<s64>() )
			{
				detail::ScriptInterOp::GetValueFromScript<s64>( data, field, scriptObject );
			}
			else if ( field->Type->Name == TypeCName<u64>() )
			{
				detail::ScriptInterOp::GetValueFromScript<u64>( data, field, scriptObject );
			}
			else if ( field->Type->Name == TypeCName<f32>() )
			{
				detail::ScriptInterOp::GetValueFromScript<f32>( data, field, scriptObject );
			}
			else if ( field->Type->Name == TypeCName<f64>() )
			{
				detail::ScriptInterOp::GetValueFromScript<f64>( data, field, scriptObject );
			}
			else if ( field->Container )
			{
				if ( field->Container->ValueType->Name == TypeCName<String>() )
				{
					detail::ScriptInterOp::GetValueFromScript< std::vector<String> >( data, field, scriptObject );
				}
				else if ( field->Container->ValueType->Name == TypeCName<s8>() )
				{
					detail::ScriptInterOp::GetValueFromScript< std::vector<s8> >( data, field, scriptObject );
				}
				else if ( field->Container->ValueType->Name == TypeCName<u8>() )
				{
					detail::ScriptInterOp::GetValueFromScript< std::vector<u8> >( data, field, scriptObject );
				}
				else if ( field->Container->ValueType->Name == TypeCName<s16>() )
				{
					detail::ScriptInterOp::GetValueFromScript< std::vector<s16> >( data, field, scriptObject );
				}
				else if ( field->Container->ValueType->Name == TypeCName<u16>() )
				{
					detail::ScriptInterOp::GetValueFromScript< std::vector<u16> >( data, field, scriptObject );
				}
				else if ( field->Container->ValueType->Name == TypeCName<s32>() )
				{
					detail::ScriptInterOp::GetValueFromScript< std::vector<s32> >( data, field, scriptObject );
				}
				else if ( field->Container->ValueType->Name == TypeCName<u32>() )
				{
					detail::ScriptInterOp::GetValueFromScript< std::vector<u32> >( data, field, scriptObject );
				}
				else if ( field->Container->ValueType->Name == TypeCName<s64>() )
				{
					detail::ScriptInterOp::GetValueFromScript< std::vector<s64> >( data, field, scriptObject );
				}
				else if ( field->Container->ValueType->Name == TypeCName<u64>() )
				{
					detail::ScriptInterOp::GetValueFromScript< std::vector<u64> >( data, field, scriptObject );
				}
				else if ( field->Container->ValueType->Name == TypeCName<f32>() )
				{
					detail::ScriptInterOp::GetValueFromScript< std::vector<f32> >( data, field, scriptObject );
				}
				else if ( field->Container->ValueType->Name == TypeCName<f64>() )
				{
					detail::ScriptInterOp::GetValueFromScript< std::vector<f64> >( data, field, scriptObject );
				}
				else
				{
					detail::ScriptInterOp::GetValueFromScript< std::vector<Char *> >( data, field, scriptObject );
				}
			}
			else
			{
				std::vector<const BH::Char *> fieldNames { field->Name.Text };
				detail::ScriptInterOp::GetScriptTypeValueFromScript( data, field, fieldNames, scriptObject );
			}
		}
	}

	void * ScriptComponent::GetScriptValue()
	{
		if ( !mDummyType )
			return nullptr;

		if ( mData )
			return mData;

		Char * data = reinterpret_cast< Char * >( mDummyType->New() );
			
		std::stack<const Type::Fields *> fields = mDummyType->GetAllFields();
		// There should not be a base type, therefore stack size should be 1
		if ( !fields.empty() )
		{
			for ( const auto & field : *fields.top() )
			{
				GetScriptValueByField( &field, data, &mScriptObject );
			}
		}

		return data;
	}

	void * ScriptComponent::GetValueFromScriptValue( std::list< const Char * > & fieldNames, void * data )
	{
		const Type * type =  mDummyType;
		void * val = data;
		while ( !fieldNames.empty() )
		{
			const Field * field = type->GetField( fieldNames.front() );

			if ( !field )
				return nullptr;

			fieldNames.pop_front();
			val = field->GetPtr( val );
			if ( field->IsPointer )
				val = *reinterpret_cast< void ** >( val );

			type = field->Type;
		}

		return val;
	}

	void ScriptComponent::FreeScriptValue( void * data )
	{
		if ( mDummyType && mData != data )
			mDummyType->Delete( data );
	}

	String ScriptComponent::GetObjectName() const
	{
		return mScriptObjectName.substr( 0, mScriptObjectName.size() - mScript.mType.size() );
	}
}