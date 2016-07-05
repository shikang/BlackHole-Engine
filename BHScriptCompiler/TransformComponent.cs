using System;
using System.Collections.Generic;

namespace BH
{
  public class TransformComponent : BHComponent
  {
    public void Initialise()
    {
      mPosition.x = 1.0f;
      mPosition.y = 1.0f;
      mPosition.z = 1.0f;
    }
    
    public void Shutdown()
    {
    }
    
    public void Update( float dt )
    {
    }
    
    public void Snapback( float x, float y, float z )
    {
      //mPosition.x -= x;
      //mPosition.y -= y;
      //mPosition.z -= z;
      
      //Console.WriteLine( "Snap back: {0}, {1}, {2}", x, y, z );
      //Console.WriteLine( "Before Position: {0}, {1}, {2}", mPosition.x, mPosition.y, mPosition.z );
      mPosition += new BHVector3f( x, y, z );
      //Console.WriteLine( "After Position: {0}, {1}, {2}", mPosition.x, mPosition.y, mPosition.z );
    }
    
    public BHVector3f mPosition = new BHVector3f();
    public BHVector3f mScale = new BHVector3f( 1.0f, 1.0f, 1.0f );
    public BHVector3f mRotation = new BHVector3f();
  
  }
}