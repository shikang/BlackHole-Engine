using System;
using System.Collections.Generic;

namespace BH
{
  public class MeshComponent : BHComponent
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
                                     tc.mRotation, mMesh.Mesh, 
                                     mMaterial.Material );
    }
    
    public BHMesh mMesh = new BHMesh( "Up" );
    public BHMaterial mMaterial = new BHMaterial( "Plastic" );
  }
}