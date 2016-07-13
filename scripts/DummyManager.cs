using System;
using System.Collections.Generic;

namespace BH
{
  public class Dummy
  {
    public BHVector3f mPosition = new BHVector3f( 0.0f, 0.0f, 0.0f );
    public BHMaterial mMaterial = new BHMaterial( "Plastic" );
  }
  
  public class DummyManager
  {   
    public void Draw( float dt )
    {                          
      foreach( var dummy in mDummies )
      {
        CPlusPlusInterface.DrawObject( dummy.mPosition, 
                                       mAllScale, 
                                       mAllRot, 
                                       "Sphere",
                                       dummy.mMaterial.Material,
                                       (int)BHModel.ANIMATION.INVALID );
      }
    }
    
    public List<Dummy> mDummies = new List<Dummy>();
    private BHVector3f mAllScale = new BHVector3f( 1.0f, 1.0f, 1.0f );
    private BHVector3f mAllRot = new BHVector3f( 0.0f, 0.0f, 0.0f );
  }
}