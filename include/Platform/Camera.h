#ifndef BH_CAMERA_H
#define BH_CAMERA_H

#include "Platform/Vector3f.h"
#include "Platform/Matrix4.h"

namespace BH
{
	class Camera
	{
	public:
		// Constructor
		Camera();

		// Destructor
		~Camera();

		// Set Positiom
		void SetPosition( const Vector3f & position );

		// Set Target or Look At
		void SetTarget( const Vector3f & lookat );

		// Set up vector
		void SetUp( const Vector3f & up );

		// Get Position
		Vector3f GetPosition() const;

		// Get Look At
		Vector3f GetTarget() const;

		// Get Up
		Vector3f GetUp() const;

		// Get view matrix
		Matrix4 GetView() const;

		// Update view matrix
		void UpdateView();

	private:
		Matrix4 mView;		//!< View Matrix

		Vector3f mPosition;	//!< Camera Position
		Vector3f mTarget;	//!< Camera Look at
		Vector3f mUp;		//!< Camera Up

		bool mUpdate;		//!< Flag to check if update is needed
	};
}

#endif