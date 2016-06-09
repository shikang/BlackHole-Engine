using System;
using System.Collections.Generic;

namespace BH
{
  public class LightSwitch : BHComponent
  {
    public void Initialise()
    {
    }
    
    public void Shutdown()
    {    
    }
    
    public void Update( float dt )
    {
      // We should create another component call player behaviour 
      // and set this callback in intialise instead in update
      if( !mSetCallback )
      {
        StationaryBoxCollider sc = mObject.GetComponent<StationaryBoxCollider>();
        sc.SetOnCollision( this.OnCollision );
        mSetCallback = true;
      }
      
      if( mRotate )
      {
        BHVector3f direction = new BHVector3f( 1.0f, 1.0f, 0.0f );
        TransformComponent tc = mObject.GetComponent<TransformComponent>();
          
        tc.mRotation += ( dt * mRoateSpeed * direction );
        
        if( tc.mRotation.x >= 360.0f )
          tc.mRotation -= 360.0f * direction;
      }
    }
    
    public void OnCollision( string colliderTag )
    {
      if( colliderTag == "Player" )
      {
        BHEventManager.Instance.Raise( "OffLight", this, EventArgs.Empty );
        mRotate = false;
      }
    }
    
    public float mRoateSpeed = 180.0f;
    private bool mRotate = true;
    private bool mSetCallback = false;
  }
}