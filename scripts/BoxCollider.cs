using System;
using System.Collections.Generic;

namespace BH
{
  public class BoxCollider : BHComponent
  {
    public delegate void OnCollision( string colliderTag );
    
    public void Initialise()
    {
    }
    
    public void Shutdown()
    {
      if( mAdd )
      {
        CPlusPlusInterface.RemoveMovingBoxCollider( mObject.GetName() );
      }
      
      SetOnCollision( null );
    }
    
    public void Update( float dt )
    {
      if( !mAdd )
      {
        TransformComponent tc = mObject.GetComponent<TransformComponent>();
        ModelComponent mc = mObject.GetComponent<ModelComponent>();
        CPlusPlusInterface.AddMovingBoxCollider( mObject.GetName(),
                                                 tc.mPosition,
                                                 tc.mScale,
                                                 tc.mRotation,
                                                 mc.mModel.Model,
                                                 mPassable,
                                                 mTag );
        mAdd = true;
      }
      else
      {
        TransformComponent tc = mObject.GetComponent<TransformComponent>();
        ModelComponent mc = mObject.GetComponent<ModelComponent>();
        CPlusPlusInterface.UpdateMovingBoxCollider( mObject.GetName(),
                                                    tc.mPosition,
                                                    tc.mScale,
                                                    tc.mRotation,
                                                    mc.mModel.Model,
                                                    mPassable,
                                                    mTag );
      }
    }
    
    public void SetOnCollision( OnCollision c )
    {
      mOnCollision = c;
    }
    
    public void CallOnCollision( string colliderTag )
    {
      if( mOnCollision != null )
        mOnCollision( colliderTag );
    }
    
    private bool mAdd = false;
    private OnCollision mOnCollision = null;
    public bool mPassable = false;
    public string mTag = "NoTag";
    
  }
}