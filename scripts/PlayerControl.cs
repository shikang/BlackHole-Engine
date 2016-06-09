using System;
using System.Collections.Generic;

namespace BH
{
  public class PlayerControl : BHComponent
  {
    public void Initialise()
    {
      BHEventManager.Instance.AddKeyTriggeredEvent( this.KeyTriggered );
      BHEventManager.Instance.AddKeyReleasedEvent( this.KeyReleased );
    }
    
    public void Shutdown()
    {    
      BHEventManager.Instance.RemoveKeyTriggeredEvent( this.KeyTriggered );
      BHEventManager.Instance.RemoveKeyReleasedEvent( this.KeyReleased );
    }
    
    public void Update( float dt )
    {
      TransformComponent tc = mObject.GetComponent<TransformComponent>();
      
      BHVector3f direction = new BHVector3f( mLeft ? -1.0f : ( mRight ? 1.0f : 0.0f ),
                                             0.0f,
                                             mDown ? -1.0f : ( mUp ? 1.0f : 0.0f ) );
      
      direction.Normalize();
      tc.mPosition += dt * mSpeed * direction;
    }
    
    public void KeyTriggered( int keycode )
    {
      mUp = mUp ? true : ( (int)KeyCode.Up == keycode );
      mDown = mDown ? true : ( (int)KeyCode.Down == keycode );
      mLeft = mLeft ? true : ( (int)KeyCode.Left == keycode );
      mRight = mRight ? true : ( (int)KeyCode.Right == keycode );
    }
    
    public void KeyReleased( int keycode )
    {
      mUp = mUp ? !( (int)KeyCode.Up == keycode ) : false;
      mDown = mDown ? !( (int)KeyCode.Down == keycode ) : false;
      mLeft = mLeft ? !( (int)KeyCode.Left == keycode ) : false;
      mRight = mRight ? !( (int)KeyCode.Right == keycode ) : false;
    }
    
    private bool mAdd = false;
    private bool mUp = false;
    private bool mDown = false;
    private bool mLeft = false;
    private bool mRight = false;
    public float mSpeed = 5.0f;
  }
}