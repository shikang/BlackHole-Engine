#include "Editor/EditorColourPicker.h"
#include "Editor/EditorUIManager.h"

#include"Game/GameConfig.h"

#if GAME_EDITOR_MODE

// Dependicies
#include "CEGUI/System.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/InputEvent.h"
#include "CEGUI/CEGUI.h"
#include "CEGUI/widgets/Combobox.h"
#include "CEGUI/widgets/ListboxTextItem.h"
#include "CEGUI/Event.h"

namespace BH
{
	EditorColourPicker::EditorColourPicker( const String & name, const String & label, Color * colour, CEGUI::Window * root, bool readOnly )
	: mHLayout( static_cast<CEGUI::HorizontalLayoutContainer *>( CEGUI::WindowManager::getSingleton().createWindow( "HorizontalLayoutContainer" ) ) )
	, mPicker( static_cast<CEGUI::ColourPicker *>( CEGUI::WindowManager::getSingleton().createWindow( "Vanilla/ColourPicker", (name + " ColourPicker").c_str() ) ) )
	, mLabel( CEGUI::WindowManager::getSingleton().createWindow( "Vanilla/StaticText", name.c_str() ) )
	, mRoot( root )
	, mColour( colour )
	, mVector3f( nullptr )
	, mVector4f( nullptr )
	, mReadOnly( readOnly )
	{
		// H Layout
		//mHLayout->setSize( CEGUI::USize( cegui_reldim( 1.0f ), cegui_absdim( 40.0f ) ) );
		mHLayout->setMouseInputPropagationEnabled( true );
		mHLayout->setMargin( CEGUI::UBox( CEGUI::UDim( 0.0f, 0.0f ), CEGUI::UDim( 0.0f, 0.0f ), 
										  CEGUI::UDim( 0.0f, 0.0f ), CEGUI::UDim( 0.0f, 0.0f ) ) );
		root->addChild( mHLayout );

		// Label
		mHLayout->addChild( mLabel );
		mLabel->setProperty( "FrameEnabled", "false" );
		mLabel->setProperty( "BackgroundEnabled", "false" );
		mLabel->setSize( CEGUI::USize( cegui_reldim( 1.0f - EditorUIManager::StandardSize.x - 2.0f * EditorUIManager::IndentGap ), cegui_reldim( EditorUIManager::StandardSize.y ) ) );
		mLabel->setMargin( CEGUI::UBox( cegui_reldim( 0.005f ), cegui_reldim( 0.0f ), cegui_reldim( 0.0f ), cegui_reldim( 0.0f ) ) );
		mLabel->setText( label.c_str() );

		// Editbox
		mHLayout->addChild( mPicker );
		mPicker->setSize( CEGUI::USize( cegui_reldim( EditorUIManager::StandardSize.x ), cegui_reldim( EditorUIManager::StandardSize.y ) ) );
		mPicker->setMargin( CEGUI::UBox( cegui_reldim( 0.005f ), cegui_reldim( 0.0f ), cegui_reldim( 0.0f ), cegui_reldim( 0.0f ) ) );
		mPicker->setColour( CEGUI::Colour( colour->r, colour->g, colour->b, colour->a ) );
		mPicker->subscribeEvent( CEGUI::ColourPicker::EventAcceptedColour,
								 CEGUI::Event::Subscriber( &EditorColourPicker::SetValueEvent,
														   this ) );
	}

	EditorColourPicker::EditorColourPicker( const String & name, const String & label, Vector3f * colour, CEGUI::Window * root, bool readOnly )
	: mHLayout( static_cast<CEGUI::HorizontalLayoutContainer *>( CEGUI::WindowManager::getSingleton().createWindow( "HorizontalLayoutContainer" ) ) )
	, mPicker( static_cast<CEGUI::ColourPicker *>( CEGUI::WindowManager::getSingleton().createWindow( "Vanilla/ColourPicker", (name + " ColourPicker").c_str() ) ) )
	, mLabel( CEGUI::WindowManager::getSingleton().createWindow( "Vanilla/StaticText", name.c_str() ) )
	, mRoot( root )
	, mColour( nullptr )
	, mVector3f( colour )
	, mVector4f( nullptr )
	, mReadOnly( readOnly )
	{
		// H Layout
		//mHLayout->setSize( CEGUI::USize( cegui_reldim( 1.0f ), cegui_absdim( 40.0f ) ) );
		mHLayout->setMouseInputPropagationEnabled( true );
		mHLayout->setMargin( CEGUI::UBox( CEGUI::UDim( 0.0f, 0.0f ), CEGUI::UDim( 0.0f, 0.0f ), 
										  CEGUI::UDim( 0.0f, 0.0f ), CEGUI::UDim( 0.0f, 0.0f ) ) );
		root->addChild( mHLayout );

		// Label
		mHLayout->addChild( mLabel );
		mLabel->setProperty( "FrameEnabled", "false" );
		mLabel->setProperty( "BackgroundEnabled", "false" );
		mLabel->setSize( CEGUI::USize( cegui_reldim( 1.0f - EditorUIManager::StandardSize.x - 2.0f * EditorUIManager::IndentGap ), cegui_reldim( EditorUIManager::StandardSize.y ) ) );
		mLabel->setMargin( CEGUI::UBox( cegui_reldim( 0.005f ), cegui_reldim( 0.0f ), cegui_reldim( 0.0f ), cegui_reldim( 0.0f ) ) );
		mLabel->setText( label.c_str() );

		// Editbox
		mHLayout->addChild( mPicker );
		mPicker->setSize( CEGUI::USize( cegui_reldim( EditorUIManager::StandardSize.x ), cegui_reldim( EditorUIManager::StandardSize.y ) ) );
		mPicker->setMargin( CEGUI::UBox( cegui_reldim( 0.005f ), cegui_reldim( 0.0f ), cegui_reldim( 0.0f ), cegui_reldim( 0.0f ) ) );
		mPicker->setColour( CEGUI::Colour( colour->x, colour->y, colour->z, 1.0f ) );
		mPicker->subscribeEvent( CEGUI::ColourPicker::EventAcceptedColour,
								 CEGUI::Event::Subscriber( &EditorColourPicker::SetValueEvent,
														   this ) );
	}

	EditorColourPicker::EditorColourPicker( const String & name, const String & label, Vector4f * colour, CEGUI::Window * root, bool readOnly )
	: mHLayout( static_cast<CEGUI::HorizontalLayoutContainer *>( CEGUI::WindowManager::getSingleton().createWindow( "HorizontalLayoutContainer" ) ) )
	, mPicker( static_cast<CEGUI::ColourPicker *>( CEGUI::WindowManager::getSingleton().createWindow( "Vanilla/ColourPicker", (name + " ColourPicker").c_str() ) ) )
	, mLabel( CEGUI::WindowManager::getSingleton().createWindow( "Vanilla/StaticText", name.c_str() ) )
	, mRoot( root )
	, mColour( nullptr )
	, mVector3f( nullptr )
	, mVector4f( colour )
	, mReadOnly( readOnly )
	{
		// H Layout
		//mHLayout->setSize( CEGUI::USize( cegui_reldim( 1.0f ), cegui_absdim( 40.0f ) ) );
		mHLayout->setMouseInputPropagationEnabled( true );
		mHLayout->setMargin( CEGUI::UBox( CEGUI::UDim( 0.0f, 0.0f ), CEGUI::UDim( 0.0f, 0.0f ), 
										  CEGUI::UDim( 0.0f, 0.0f ), CEGUI::UDim( 0.0f, 0.0f ) ) );
		root->addChild( mHLayout );

		// Label
		mHLayout->addChild( mLabel );
		mLabel->setProperty( "FrameEnabled", "false" );
		mLabel->setProperty( "BackgroundEnabled", "false" );
		mLabel->setSize( CEGUI::USize( cegui_reldim( 1.0f - EditorUIManager::StandardSize.x - 2.0f * EditorUIManager::IndentGap ), cegui_reldim( EditorUIManager::StandardSize.y ) ) );
		mLabel->setMargin( CEGUI::UBox( cegui_reldim( 0.005f ), cegui_reldim( 0.0f ), cegui_reldim( 0.0f ), cegui_reldim( 0.0f ) ) );
		mLabel->setText( label.c_str() );

		// Editbox
		mHLayout->addChild( mPicker );
		mPicker->setSize( CEGUI::USize( cegui_reldim( EditorUIManager::StandardSize.x ), cegui_reldim( EditorUIManager::StandardSize.y ) ) );
		mPicker->setMargin( CEGUI::UBox( cegui_reldim( 0.005f ), cegui_reldim( 0.0f ), cegui_reldim( 0.0f ), cegui_reldim( 0.0f ) ) );
		mPicker->setColour( CEGUI::Colour( colour->x, colour->y, colour->z, colour->w ) );
		mPicker->subscribeEvent( CEGUI::ColourPicker::EventAcceptedColour,
								 CEGUI::Event::Subscriber( &EditorColourPicker::SetValueEvent,
														   this ) );
	}

	EditorColourPicker::~EditorColourPicker()
	{
		mPicker->removeEvent( CEGUI::ColourPicker::EventAcceptedColour );

		mHLayout->removeChild( mPicker );
		mHLayout->removeChild( mLabel );
		mRoot->removeChild( mHLayout );

		CEGUI::WindowManager::getSingleton().destroyWindow( mPicker );
		CEGUI::WindowManager::getSingleton().destroyWindow( mLabel );
		CEGUI::WindowManager::getSingleton().destroyWindow( mHLayout );
	}

	bool EditorColourPicker::SetValueEvent( const CEGUI::EventArgs & )
	{
		if ( mReadOnly )
		{
			if ( mColour )
				mPicker->setColour( CEGUI::Colour( mColour->r, mColour->g, mColour->b, mColour->a ) );
			else if ( mVector3f )
				mPicker->setColour( CEGUI::Colour( mVector3f->x, mVector3f->y, mVector3f->z, 1.0f ) );
			else if ( mVector4f )
				mPicker->setColour( CEGUI::Colour( mVector4f->x, mVector4f->y, mVector4f->z, mVector4f->w ) );
		}
		else
		{
			CEGUI::Colour c = mPicker->getColour();

			if ( mColour )
			{
				mColour->r = c.getRed();
				mColour->g = c.getGreen();
				mColour->b = c.getBlue();
				mColour->a = c.getAlpha();
			}
			else if ( mVector3f )
			{
				mVector3f->x = c.getRed();
				mVector3f->y = c.getGreen();
				mVector3f->z = c.getBlue();
			}
			else if ( mVector4f )
			{
				mVector4f->x = c.getRed();
				mVector4f->y = c.getGreen();
				mVector4f->z = c.getBlue();
				mVector4f->w = c.getAlpha();
			}
		}

		return true;
	}
}
#endif