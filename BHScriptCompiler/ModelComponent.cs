using System;
using System.Collections.Generic;

namespace BH
{
    public class ModelComponent : BHComponent
    {
        public void Initialise()
        {
            mModel.InitialiseModel();
        }
        
        public void Shutdown()
        {
            mModel.ShutdownModel();
        }
        
        public void Draw( float dt )
        {
            TransformComponent tc = mObject.GetComponent<TransformComponent>();
          
            CPlusPlusInterface.DrawObject( tc.mPosition, tc.mScale, 
                                           tc.mRotation, mModel.Model, 
                                           mMaterial.Material );
        }
        
        public BHModel mModel = new BHModel( "Up" );
        public BHMaterial mMaterial = new BHMaterial( "Plastic" );
    }
}