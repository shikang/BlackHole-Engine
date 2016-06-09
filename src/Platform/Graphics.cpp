// Precompiled Header
#include "Platform/Stdafx.h"

#include "Platform/MessageBox.h"
#include "Platform/Graphics.h"

namespace BH
{
	Graphics Graphics::sInstance;

	Graphics::Graphics()
	{
	}

	Graphics::~Graphics()
	{
	}

	bool Graphics::Initialise( const PlatformWindow & window, f32 near_dist, f32 far_dist )
	{
		bool result = mPipeline.Initialise( window.GetWidth(), 
											window.GetHeight(), 
											window.mHwnd, 
											near_dist,
											far_dist,
											window.GetFullscreen() );

		if ( !result )
		{
			//MessageBox(window.mHwnd, "Could not initialize Direct3D.", "Error", MB_OK);
			CreateMessageBox( "Could not initialize Direct3D.", "Error" );
			return false;
		}

		return true;
	}

	void Graphics::Shutdown()
	{
		mPipeline.Shutdown();
	}

	GraphicsPipeline * Graphics::GetPipeline()
	{
		return &mPipeline;
	}
}