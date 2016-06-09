#define DEMO_SCRIPT
using System;
using System.Collections.Generic;

namespace BH
{
  public class LightManager
  {
    private delegate void OffLightDelegate( Object sender, EventArgs e );
    
    public void OffLight( Object sender, EventArgs e )
    {
      mLightOff = true;
    }
    
    public void Initialise()
    {
      Delegate d = Delegate .CreateDelegate( typeof( OffLightDelegate ),
                                             this,
                                             "OffLight" );
      BHEventManager.Instance.Add( "OffLight", d );
      Resize();
    }
    
    public void Shutdown()
    {
      Delegate d = Delegate .CreateDelegate( typeof( OffLightDelegate ),
                                             this,
                                             "OffLight" );
      BHEventManager.Instance.Remove( "OffLight", d );
    }
    
    private void Resize()
    {
      mPositions.Clear();
      mColors.Clear();
      
      Random random = new Random();
      for( int i = 0; i < mLightCount; ++i )
      {
        double angle = (double)i * 2.0 * Math.PI / 20;
        float x = -7.0f * (float)Math.Cos( angle );
        float y = 4.0f + (float)( i / 20 );
        float z = -7.0f * (float)Math.Sin( angle );
        float r = (float)random.NextDouble();
        float g = (float)random.NextDouble();
        float b = (float)random.NextDouble();
        mPositions.Add( new BHVector3f( x, y, z ) );
        mColors.Add( new BHVector4f( r, g, b, 1.0f ) );
      }
    }
    
    public void Draw( float dt )
    {
      if( mLightCount != mPositions.Count )
        Resize();
      
      #if DEMO_SCRIPT
      
        // Clamp time value to 0 to 1
        mTime += 0.5f * dt;
        mTime = ( mTime >= 1.0f ) ? mTime - 1.0f : mTime;
        
        double angle = (double)mTime * 2.0 * Math.PI;
        float xOffset = (float)Math.Cos( angle ) * mPathRadius;
        float yOffset = (float)Math.Sin( angle ) * mPathRadius;
        BHVector3f offset = new BHVector3f( xOffset, yOffset, 0.0f );
        
      #else 
      
        BHVector3f offset = new BHVector3f( 0.0f, 0.0f, 0.0f );
        
      #endif
      
      // Draw local lights
      for( int i = 0; i < mLightCount; ++i )
      {
        BHVector3f pos = mPositions[i] + offset;
        CPlusPlusInterface.DrawLight( pos,
                                      mColors[i],
                                      10.0f );
      }
      
      // Draw window light
      if( !mLightOff )
        CPlusPlusInterface.DrawLight( new BHVector3f( 10.0f, 10.0f, 0.0f ),
                                      new BHVector4f( 1.0f, 1.0f, 1.0f, 1.0f ),
                                      new BHVector3f( -1.0f, -1.0f, 0.0f ),
                                      new BHVector3f( 10.0f, 10.0f, 20.0f ) );
    }
    
    public int mLightCount = 20;
    private List<BHVector3f> mPositions = new List<BHVector3f>();
    private List<BHVector4f> mColors = new List<BHVector4f>();
    private bool mLightOff = false;
    
    #if DEMO_SCRIPT
      public float mPathRadius = 1.0f;
      private float mTime = 0.0f;
    #endif
  }
}