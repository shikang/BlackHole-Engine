using System;
using System.Text;
using System.Runtime.InteropServices;

namespace BH
{
    public class CPlusPlusInterface
    {
        // Start profiling
        [DllImport( "BHScriptBridge.dll" )]
        public static extern void StartProfiler( string profiler_name );

        // Stop profiling
        [DllImport( "BHScriptBridge.dll" )]
        public static extern void StopProfiler( string profiler_name );

        // Draw instance
        [DllImport( "BHScriptBridge.dll" )]
        private static extern void DrawInstance( float posX,
                                                 float posY,
                                                 float posZ,
                                                 float scaleX,
                                                 float scaleY,
                                                 float scaleZ,
                                                 float rotX,
                                                 float rotY,
                                                 float rotZ,
                                                 string model,
                                                 string material );

        // Draw global light
        [DllImport( "BHScriptBridge.dll" )]
        private static extern void DrawGlobalLight( float posX,
                                                    float posY,
                                                    float posZ,
                                                    float colorR,
                                                    float colorG,
                                                    float colorB,
                                                    float colorA );

        // Draw global light
        [DllImport( "BHScriptBridge.dll" )]
        private static extern void DrawDirectionalLight( float posX,
                                                         float posY,
                                                         float posZ,
                                                         float colorR,
                                                         float colorG,
                                                         float colorB,
                                                         float colorA,
                                                         float dirX,
                                                         float dirY,
                                                         float dirZ,
                                                         float width,
                                                         float height,
                                                         float depth );

        // Draw local light
        [DllImport( "BHScriptBridge.dll" )]
        private static extern void DrawLocalLight( float posX,
                                                   float posY,
                                                   float posZ,
                                                   float colorR,
                                                   float colorG,
                                                   float colorB,
                                                   float colorA,
                                                   float radius );

        // Add Static OBB
        [DllImport( "BHScriptBridge.dll" )]
	    private static extern void AddStaticOBB( string name,
	    										 float posX,	
	    										 float posY,
	    										 float posZ,
	    										 float scaleX,
	    										 float scaleY,
	    										 float scaleZ,
	    										 float rotX,
	    										 float rotY,
	    										 float rotZ,
	    										 string model,
                                                 bool passable,
                                                 string tag );

	    // Update Static OBB
	    [DllImport( "BHScriptBridge.dll" )]
	    private static extern void UpdateStaticOBB( string name,
	    											float posX,	
	    											float posY,
	    											float posZ,
	    											float scaleX,
	    											float scaleY,
	    											float scaleZ,
	    											float rotX,
	    											float rotY,
	    											float rotZ,
	    											string model,
                                                    bool passable,
                                                    string tag );

	    // Remove Static OBB
	    [DllImport( "BHScriptBridge.dll" )]
	    private static extern void RemoveStaticOBB( string name );

	    // Add Moving OBB
	    [DllImport( "BHScriptBridge.dll" )]
	    private static extern void AddMovingOBB( string name,
	    										 float posX,	
	    										 float posY,
	    										 float posZ,
	    										 float scaleX,
	    										 float scaleY,
	    										 float scaleZ,
	    										 float rotX,
	    										 float rotY,
	    										 float rotZ,
	    										 string model,
                                                 bool passable,
                                                 string tag );

	    // Update Moving OBB
	    [DllImport( "BHScriptBridge.dll" )]
	    private static extern void UpdateMovingOBB( string name,
	    											float posX,	
	    											float posY,
	    											float posZ,
	    											float scaleX,
	    											float scaleY,
	    											float scaleZ,
	    											float rotX,
	    											float rotY,
	    											float rotZ,
	    											string model,
                                                    bool passable,
                                                    string tag );

	    // Remove Moving OBB
	    [DllImport( "BHScriptBridge.dll" )]
	    private static extern void RemoveMovingOBB( string name );

        // ------------------ Wrapper functions ------------------
        public static void DrawObject( BHVector3f position,
                                       BHVector3f scale,
                                       BHVector3f rotation,
                                       string model,
                                       string material )
        {
            DrawInstance( position.x,
                          position.y,
                          position.z,
                          scale.x,
                          scale.y,
                          scale.z,
                          rotation.x,
                          rotation.y,
                          rotation.z,
                          model,
                          material );
        }

        public static void DrawLight( BHVector3f position,
                                      BHVector4f color )
        {
            DrawGlobalLight( position.x,
                             position.y,
                             position.z,
                             color.x,
                             color.y,
                             color.z,
                             color.w );
        }

        public static void DrawLight( BHVector3f position,
                                      BHVector4f color,
                                      BHVector3f directional,
                                      BHVector3f dimension )
        {
            DrawDirectionalLight( position.x,
                                  position.y,
                                  position.z,
                                  color.x,
                                  color.y,
                                  color.z,
                                  color.w,
                                  directional.x,
                                  directional.y,
                                  directional.z,
                                  dimension.x,
                                  dimension.y,
                                  dimension.z );
        }

        public static void DrawLight( BHVector3f position,
                                      BHVector4f color,
                                      float radius )
        {
            DrawLocalLight( position.x,
                            position.y,
                            position.z,
                            color.x,
                            color.y,
                            color.z,
                            color.w,
                            radius);
        }

        public static void AddStationaryBoxCollider( string name,
	    										     BHVector3f position,
	    										     BHVector3f scale,
	    										     BHVector3f rotation,
	    										     string model,
                                                     bool passable,
                                                     string tag )
        {
            AddStaticOBB( name,
                          position.x,
                          position.y,
                          position.z,
                          scale.x,
                          scale.y,
                          scale.z,
                          rotation.x,
                          rotation.y,
                          rotation.z,
                          model,
                          passable,
                          tag );
        }

        public static void UpdateStationaryBoxCollider( string name,
	    										        BHVector3f position,
	    										        BHVector3f scale,
	    										        BHVector3f rotation,
	    										        string model,
                                                        bool passable,
                                                        string tag )
        {
            UpdateStaticOBB( name,
                             position.x,
                             position.y,
                             position.z,
                             scale.x,
                             scale.y,
                             scale.z,
                             rotation.x,
                             rotation.y,
                             rotation.z,
                             model,
                             passable,
                             tag );
        }

        public static void RemoveStationaryBoxCollider( string name )
        {
            RemoveStaticOBB( name );
        }

        public static void AddMovingBoxCollider( string name,
	    										 BHVector3f position,
	    										 BHVector3f scale,
	    										 BHVector3f rotation,
	    										 string model, 
                                                 bool passable,
                                                 string tag )
        {
            AddMovingOBB( name,
                          position.x,
                          position.y,
                          position.z,
                          scale.x,
                          scale.y,
                          scale.z,
                          rotation.x,
                          rotation.y,
                          rotation.z,
                          model,
                          passable,
                          tag );
        }

        public static void UpdateMovingBoxCollider( string name,
	    										    BHVector3f position,
	    										    BHVector3f scale,
	    										    BHVector3f rotation,
	    										    string model,
                                                    bool passable,
                                                    string tag )
        {
            UpdateMovingOBB( name,
                             position.x,
                             position.y,
                             position.z,
                             scale.x,
                             scale.y,
                             scale.z,
                             rotation.x,
                             rotation.y,
                             rotation.z,
                             model,
                             passable,
                             tag );
        }

        public static void RemoveMovingBoxCollider( string name )
        {
            RemoveMovingOBB( name );
        }
    }
}
