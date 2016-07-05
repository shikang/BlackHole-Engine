using System;
using System.Collections.Generic;

namespace BH
{
  public class ModelComponent : BHComponent
  {
    public void Initialise()
    {
    }
    
    public void Shutdown()
    {
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