#include "Editor/EditorUIManager.h"

#if GAME_EDITOR_MODE

namespace BH
{
	template< typename T >
	EditorVariableBox<T>::EditorVariableBox( const String & name, const String & label, T * var, CEGUI::Window * root, bool readOnly )
	: EditorEditBox( name, label, root, readOnly )
	, mVar( var ) 
	, mRefresh( true )
	{ 
		SetValidateString();
		SetValue( *var );
		mEditBox->subscribeEvent( CEGUI::Editbox::EventTextAccepted, 
								  CEGUI::Event::Subscriber( &EditorVariableBox<T>::SetValueEvent,
															this ) );

		mEditBox->subscribeEvent( CEGUI::Window::EventDeactivated, 
								  CEGUI::Event::Subscriber( &EditorVariableBox<T>::SetValueEvent,
															this ) );

		mEditBox->subscribeEvent( CEGUI::Window::EventActivated, 
								  CEGUI::Event::Subscriber( &EditorVariableBox<T>::OffRefreshEvent,
															this ) );

		if ( !readOnly )
			EditorUIManager::Instance().AddRefreshEvent( Delegate<void()>::FromMethod<E_METHOD( EditorVariableBox<T>, Refresh )>( this ) );
	}

	template< typename T >
	EditorVariableBox<T>::~EditorVariableBox() 
	{
		//Refresh();

		mEditBox->removeEvent( CEGUI::Editbox::EventTextAccepted );
		mEditBox->removeEvent( CEGUI::Window::EventDeactivated );
		mEditBox->removeEvent( CEGUI::Window::EventActivated );

		EditorUIManager::Instance().RemoveRefreshEvent( Delegate<void()>::FromMethod<E_METHOD( EditorVariableBox<T>, Refresh )>( this ) );
	}

	template< typename T >
	bool EditorVariableBox<T>::SetValueEvent( const CEGUI::EventArgs & )
	{
		SetValue();
		mRefresh = true;
		return true;
	}

	template< typename T >
	bool EditorVariableBox<T>::OffRefreshEvent( const CEGUI::EventArgs & )
	{
		mRefresh = false;
		return true;
	}

	template< typename T >
	void EditorVariableBox<T>::Refresh()
	{
		if ( !mRefresh || mReadOnly )
			return;

		SetValue( *mVar );

		if ( mRefreshCallback )
			mRefreshCallback( mVar );
	}

	template< typename T >
	inline void EditorVariableBox<T>::SetValidateString()
	{
		mEditBox->setValidationString( ".*" );
	}

	// Integer types ( There is a need to inline these functions, because we are specialising template without parameters! )
	template<>
	inline void EditorVariableBox<s8>::SetValidateString()
	{
		mEditBox->setValidationString( "\\d*" );
	}

	template<>
	inline void EditorVariableBox<u8>::SetValidateString()
	{
		mEditBox->setValidationString( "\\d*" );
	}

	template<>
	inline void EditorVariableBox<s16>::SetValidateString()
	{
		mEditBox->setValidationString( "\\d*" );
	}

	template<>
	inline void EditorVariableBox<u16>::SetValidateString()
	{
		mEditBox->setValidationString( "\\d*" );
	}

	template<>
	inline void EditorVariableBox<s32>::SetValidateString()
	{
		mEditBox->setValidationString( "\\d*" );
	}

	template<>
	inline void EditorVariableBox<u32>::SetValidateString()
	{
		mEditBox->setValidationString( "\\d*" );
	}

	template<>
	inline void EditorVariableBox<s64>::SetValidateString()
	{
		mEditBox->setValidationString( "\\d*" );
	}

	template<>
	inline void EditorVariableBox<u64>::SetValidateString()
	{
		mEditBox->setValidationString( "\\d*" );
	}

	// Integer types
	template<>
	inline void EditorVariableBox<Char>::SetValue()
	{
		*mVar = mEditBox->getText().c_str()[0];
	}

	template<>
	inline void EditorVariableBox<s8>::SetValue()
	{
		try
		{
			*mVar = static_cast<s8>( std::stoi( mEditBox->getText().c_str() ) );
		}
		catch (...)
		{
			*mVar = 0;
			mEditBox->setText( "0" );
		}
	}

	template<>
	inline void EditorVariableBox<u8>::SetValue()
	{
		try
		{
			*mVar = static_cast<u8>( std::stoi( mEditBox->getText().c_str() ) );
		}
		catch (...)
		{
			*mVar = 0;
			mEditBox->setText( "0" );
		}
	}

	template<>
	inline void EditorVariableBox<s16>::SetValue()
	{
		try
		{
			*mVar = static_cast<s16>( std::stoi( mEditBox->getText().c_str() ) );
		}
		catch (...)
		{
			*mVar = 0;
			mEditBox->setText( "0" );
		}
	}

	template<>
	inline void EditorVariableBox<u16>::SetValue()
	{
		try
		{
			*mVar = static_cast<u16>( std::stoi( mEditBox->getText().c_str() ) );
		}
		catch (...)
		{
			*mVar = 0;
			mEditBox->setText( "0" );
		}
	}

	template<>
	inline void EditorVariableBox<s32>::SetValue()
	{
		try
		{
			*mVar = std::stoi( mEditBox->getText().c_str() );
		}
		catch (...)
		{
			*mVar = 0;
			mEditBox->setText( "0" );
		}
	}

	template<>
	inline void EditorVariableBox<u32>::SetValue()
	{
		try
		{
			*mVar = static_cast<u32>( std::stoul( mEditBox->getText().c_str() ) );
		}
		catch (...)
		{
			*mVar = 0;
			mEditBox->setText( "0" );
		}
	}

	template<>
	inline void EditorVariableBox<s64>::SetValue()
	{
		try
		{
			*mVar = static_cast<s64>( std::stoll( mEditBox->getText().c_str() ) );
		}
		catch (...)
		{
			*mVar = 0;
			mEditBox->setText( "0" );
		}
	}

	template<>
	inline void EditorVariableBox<u64>::SetValue()
	{
		try
		{
			*mVar = static_cast<u64>( std::stoull( mEditBox->getText().c_str() ) );
		}
		catch (...)
		{
			*mVar = 0;
			mEditBox->setText( "0" );
		}
	}

	// Rest of valid type
	template<>
	inline void EditorVariableBox<f64>::SetValue()
	{
		try
		{
			*mVar = std::stod( mEditBox->getText().c_str() );
		}
		catch (...)
		{
			*mVar = 0;
			mEditBox->setText( "0" );
		}
	}

	template<>
	inline void EditorVariableBox<f32>::SetValue()
	{
		try
		{
			*mVar = std::stof( mEditBox->getText().c_str() );
		}
		catch (...)
		{
			*mVar = 0;
			mEditBox->setText( "0" );
		}
	}

	template<>
	inline void EditorVariableBox<String>::SetValue()
	{
		*mVar = mEditBox->getText().c_str();
	}

	template< typename T >
	void EditorVariableBox<T>::SetValue( const T & val )
	{
		*mVar = val;
		std::ostringstream is;
		is << *mVar;
		mEditBox->setText( is.str().c_str() );
	}
}

#endif