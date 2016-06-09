using System;
using System.Collections.Generic;

namespace BH
{
  public class StationaryBoxCollider : BHComponent
  {
    public delegate void StationaryOnCollision( string colliderTag );
    
    public void Initialise()
    {
    }
    
    public void Shutdown()
    {
      if( mAdd )
      {
        CPlusPlusInterface.RemoveStationaryBoxCollider( mObject.GetName() );
      }
      
      SetOnCollision( null );
    }
    
    public void Update( float dt )
    {
      if( !mAdd )
      {
        TransformComponent tc = mObject.GetComponent<TransformComponent>();
        MeshComponent mc = mObject.GetComponent<MeshComponent>();
        CPlusPlusInterface.AddStationaryBoxCollider( mObject.GetName(),
                                                     tc.mPosition,
                                                     tc.mScale,
                                                     tc.mRotation,
                                                     mc.mMesh.Mesh,
                                                     mPassable,
                                                     mTag );
        mAdd = true;
      }
      else if( mReset )
      {
        TransformComponent tc = mObject.GetComponent<TransformComponent>();
        MeshComponent mc = mObject.GetComponent<MeshComponent>();
        CPlusPlusInterface.UpdateStationaryBoxCollider( mObject.GetName(),
                                                        tc.mPosition,
                                                        tc.mScale,
                                                        tc.mRotation,
                                                        mc.mMesh.Mesh,
                                                        mPassable,
                                                        mTag );
        mReset = false;
      }
    }
  
    public void ResetBox()
    {
      mReset = true;
    }
    
    public void SetOnCollision( StationaryOnCollision c )
    {
      mOnCollision = c;
    }
    
    public void CallOnCollision( string colliderTag )
    {
      if( mOnCollision != null )
        mOnCollision( colliderTag );
    }
    
    private bool mAdd = false;
    private bool mReset = false;
    private StationaryOnCollision mOnCollision = null;
    public bool mPassable = false;
    public string mTag = "NoTag";
    
  }
}