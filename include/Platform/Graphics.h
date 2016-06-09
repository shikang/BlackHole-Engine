#ifndef BH_GRAPHICS_H
#define BH_GRAPHICS_H

#include "Platform/GraphicsPipeline.h"
#include "Platform/Mesh.h"
#include "Platform/Shader.h"
#include "Platform/PlatformWindow.h"

namespace BH
{
	class Graphics
	{
	public:
		// Singleton
		static Graphics sInstance;

	public:
		// Destructor
		~Graphics();

		// Initialise
		bool Initialise( const PlatformWindow & window, f32 near_dist, f32 far_dist  );

		// Shutdown
		void Shutdown();

	public:
		// Getters
		// Get Graphics pipeline
		GraphicsPipeline * GetPipeline();

	private:
		// Constructor
		Graphics();

	private:
		GraphicsPipeline mPipeline;

		friend class Shader;
		friend class Mesh;
	};
}

#define GRAPHICS\
	Graphics::sInstance

#endif