using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace BH
{
    public class BHVector3f : IDisposable
    {
        #region PInvokes

        [DllImport( "BHScriptBridge.dll" )]
        static private extern IntPtr CreateVector3f();

        [DllImport( "BHScriptBridge.dll" )]
        static private extern IntPtr CreateValVector3f( float x, float y, float z );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void DestroyVector3f( IntPtr pVector );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void AssignVector3f( IntPtr dest, IntPtr src );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void PlusAssignVector3f( IntPtr dest, IntPtr src );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void MinusAssignVector3f( IntPtr dest, IntPtr src );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void ScaleAssignVector3f( IntPtr dest, float s );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void MulAssignVector3f( IntPtr dest, IntPtr src );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void DivAssignVector3f( IntPtr dest, float s );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void UnaryVector3f( IntPtr dest, IntPtr src );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern bool EqualityVector3f( IntPtr lhs, IntPtr rhs );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern bool NotEqualityVector3f( IntPtr lhs, IntPtr rhs );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern void PlusVector3f( IntPtr dest, IntPtr lhs, IntPtr rhs );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern void MinusVector3f( IntPtr dest, IntPtr lhs, IntPtr rhs );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern void MulVector3f( IntPtr dest, IntPtr lhs, IntPtr rhs );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern void ScaleVector3f( IntPtr dest, IntPtr lhs, float rhs );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern void DivVector3f( IntPtr dest, IntPtr lhs, float rhs );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void CrossVector3f( IntPtr dest, IntPtr v1, IntPtr v2 );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern float GetXVector3f( IntPtr v );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern float GetYVector3f( IntPtr v );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern float GetZVector3f( IntPtr v );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void SetXVector3f( IntPtr v, float val );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern void SetYVector3f( IntPtr v, float val );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void SetZVector3f( IntPtr v, float val );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern bool InBoundsVector3f( IntPtr v, IntPtr bounds );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern void NormalizeVector3f( IntPtr v );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern float LengthVector3f( IntPtr v );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern float LengthSqVector3f( IntPtr v );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern float DotVector3f( IntPtr v1, IntPtr v2 );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void ClampVector3f( IntPtr v, IntPtr min, IntPtr max );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern float DistanceVector3f( IntPtr v1, IntPtr v2 );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern float DistanceSquaredVector3f( IntPtr v1, IntPtr v2 );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void MinVector3f( IntPtr v, IntPtr v1, IntPtr v2 );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void MaxVector3f( IntPtr v, IntPtr v1, IntPtr v2 );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void LerpVector3f( IntPtr v, IntPtr v1, IntPtr v2, float t );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void SmoothStepVector3f( IntPtr v, IntPtr v1, IntPtr v2, float t );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void BarycentricVector3f( IntPtr v, IntPtr v1, IntPtr v2, IntPtr v3, float f, float g );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void CatmullRomVector3f( IntPtr v, IntPtr v1, IntPtr v2, IntPtr v3, IntPtr v4, float t );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void HermiteVector3f( IntPtr v, IntPtr v1, IntPtr t1, IntPtr v2, IntPtr t2, float t );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void ReflectVector3f( IntPtr v, IntPtr ivec, IntPtr nvec );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void RefractVector3f( IntPtr v, IntPtr ivec, IntPtr nvec, float refractionIndex );

        #endregion PInvokes

        #region Members

        private IntPtr mVector; //!< Variable to hold the C++ class's this pointer
        public float x = 0.0f;
        public float y = 0.0f;
        public float z = 0.0f;

        #endregion Members

        public void CopyValues()
        {
            x = GetXVector3f( mVector );
            y = GetYVector3f( mVector );
            z = GetZVector3f( mVector );
        }

        public void SetValues()
        {
            SetXVector3f( mVector, x );
            SetYVector3f( mVector, y );
            SetZVector3f( mVector, z );
        }

        public BHVector3f()
        {
            mVector = CreateVector3f();
        }

        public BHVector3f( float X, float Y, float Z )
        {
            mVector = CreateValVector3f( X, Y, Z );
            x = X;
            y = Y;
            z = Z;
        }

        public BHVector3f( IntPtr v )
        {
            mVector = v;
            CopyValues();
        }

        public void Dispose()
        {
            Dispose( true );
        }

        protected virtual void Dispose( bool disposing )
        {
            if ( mVector != IntPtr.Zero )
            {
                // Call the DLL Export to dispose this class
                DestroyVector3f( mVector );
                mVector = IntPtr.Zero;
            }

            if( disposing )
            {
                // No need to call the finalizer since we've now cleaned
                // up the unmanaged memory
                GC.SuppressFinalize(this);
            }
        }

        // This finalizer is called when Garbage collection occurs, but only if
        // the IDisposable.Dispose method wasn't already called.
        ~BHVector3f()
        {
            Dispose( false );
        }

        #region Wrapper methods
        
        public static BHVector3f operator - ( BHVector3f v ) 
        {
            v.SetValues();
            BHVector3f r = new BHVector3f();
            UnaryVector3f( r.mVector, v.mVector );
            r.CopyValues();
            return r;
        }

        public static BHVector3f operator + ( BHVector3f v1, BHVector3f v2 ) 
        {
            v1.SetValues();
            v2.SetValues();
            BHVector3f r = new BHVector3f();
            PlusVector3f( r.mVector, v1.mVector, v2.mVector );
            r.CopyValues();
            return r;
        }

        public static BHVector3f operator - ( BHVector3f v1, BHVector3f v2 ) 
        {
            v1.SetValues();
            v2.SetValues();
            BHVector3f r = new BHVector3f();
            MinusVector3f( r.mVector, v1.mVector, v2.mVector );
            r.CopyValues();
            return r;
        }

        public static BHVector3f operator * ( BHVector3f v, float s ) 
        {
            v.SetValues();
            BHVector3f r = new BHVector3f();
            ScaleVector3f( r.mVector, v.mVector, s );
            r.CopyValues();
            return r;
        }

        public static BHVector3f operator * ( float s, BHVector3f v ) 
        {
            v.SetValues();
            BHVector3f r = new BHVector3f();
            ScaleVector3f( r.mVector, v.mVector, s );
            r.CopyValues();
            return r;
        }

        public static BHVector3f operator * ( BHVector3f v1, BHVector3f v2 ) 
        {
            v1.SetValues();
            v2.SetValues();
            BHVector3f r = new BHVector3f();
            MulVector3f( r.mVector, v1.mVector, v2.mVector );
            r.CopyValues();
            return r;
        }

        public static BHVector3f operator / ( BHVector3f v, float s ) 
        {
            v.SetValues();
            BHVector3f r = new BHVector3f();
            DivVector3f( r.mVector, v.mVector, s );
            r.CopyValues();
            return r;
        }

        public BHVector3f Cross( BHVector3f v )
        {
            v.SetValues();
            BHVector3f r = new BHVector3f();
            SetValues();
            CrossVector3f( r.mVector, mVector, v.mVector );
            r.CopyValues();
            return r;
        }

        public bool InBounds( BHVector3f bounds )
        {
            SetValues();
            return InBoundsVector3f( mVector, bounds.mVector );
        }

        public void Normalize()
        {
            SetValues();
            NormalizeVector3f( mVector );
            CopyValues();
        }

        public float Length()
        {
            SetValues();
            return LengthVector3f( mVector );
        }

        public float LengthSq()
        { 
            SetValues();
            return LengthSqVector3f( mVector );
        }

        public float Dot( BHVector3f v )
        {
            v.SetValues();
            return DotVector3f( mVector, v.mVector );
        }

        public static void Clamp( BHVector3f v, BHVector3f min, BHVector3f max )
        {
            v.SetValues();
            min.SetValues();
            max.SetValues();
            ClampVector3f( v.mVector, min.mVector, max.mVector );
            v.CopyValues();
        }

        public static float Distance( BHVector3f v1, BHVector3f v2 )
        {
            v1.SetValues();
            v2.SetValues();
            return DistanceVector3f( v1.mVector, v2.mVector );
        }

        public static float DistanceSquared( BHVector3f v1, BHVector3f v2 )
        { 
            v1.SetValues();
            v2.SetValues();
            return DistanceSquaredVector3f( v1.mVector, v2.mVector );
        }

        public static BHVector3f Min( BHVector3f v1, BHVector3f v2 )
        { 
            v1.SetValues();
            v2.SetValues();
            BHVector3f r = new BHVector3f();
            MinVector3f( r.mVector, v1.mVector, v2.mVector );
            r.CopyValues();
            return r;
        }

        public static BHVector3f Max( BHVector3f v1, BHVector3f v2 )
        { 
            v1.SetValues();
            v2.SetValues();
            BHVector3f r = new BHVector3f();
            MaxVector3f( r.mVector, v1.mVector, v2.mVector );
            r.CopyValues();
            return r;
        }

        public static BHVector3f Lerp( BHVector3f v1, BHVector3f v2, float t )
        { 
            v1.SetValues();
            v2.SetValues();
            BHVector3f r = new BHVector3f();
            LerpVector3f( r.mVector, v1.mVector, v2.mVector, t );
            r.CopyValues();
            return r;
        }

        public static BHVector3f SmoothStep( BHVector3f v1, BHVector3f v2, float t )
        { 
            v1.SetValues();
            v2.SetValues();
            BHVector3f r = new BHVector3f();
            SmoothStepVector3f( r.mVector, v1.mVector, v2.mVector, t );
            r.CopyValues();
            return r;
        }

        public static BHVector3f Barycentric( BHVector3f v1, BHVector3f v2, BHVector3f v3, float f, float g )
        {
            v1.SetValues();
            v2.SetValues();
            v3.SetValues();
            BHVector3f r = new BHVector3f();
            BarycentricVector3f( r.mVector, v1.mVector, v2.mVector, v3.mVector, f, g );
            r.CopyValues();
            return r;
        }

        public static BHVector3f CatmullRom( BHVector3f v1, BHVector3f v2, BHVector3f v3, BHVector3f v4, float t )
        {
            v1.SetValues();
            v2.SetValues();
            v3.SetValues();
            v4.SetValues();
            BHVector3f r = new BHVector3f();
            CatmullRomVector3f( r.mVector, v1.mVector, v2.mVector, v3.mVector, v4.mVector, t );
            r.CopyValues();
            return r;
        }

        public static BHVector3f Hermite( BHVector3f v1, BHVector3f t1, BHVector3f v2, BHVector3f t2, float t )
        { 
            v1.SetValues();
            v2.SetValues();
            t1.SetValues();
            t2.SetValues();
            BHVector3f r = new BHVector3f();
            HermiteVector3f( r.mVector, v1.mVector, t1.mVector, v2.mVector, t2.mVector, t );
            r.CopyValues();
            return r;
        }

        public static BHVector3f Reflect( BHVector3f ivec, BHVector3f nvec )
        {
            ivec.SetValues();
            nvec.SetValues();
            BHVector3f r = new BHVector3f();
            ReflectVector3f( r.mVector, ivec.mVector, nvec.mVector );
            r.CopyValues();
            return r;
        }

        public static BHVector3f Refract( BHVector3f ivec, BHVector3f nvec, float refractionIndex )
        {
            ivec.SetValues();
            nvec.SetValues();
            BHVector3f r = new BHVector3f();
            RefractVector3f( r.mVector, ivec.mVector, nvec.mVector, refractionIndex );
            r.CopyValues();
            return r;
        }
        
        #endregion Wrapper methods
    }
}
