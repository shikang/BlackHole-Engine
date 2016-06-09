using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace BH
{
    public class BHVector4f : IDisposable
    {
        #region PInvokes

        [DllImport( "BHScriptBridge.dll" )]
        static private extern IntPtr CreateVector4f();

        [DllImport( "BHScriptBridge.dll" )]
        static private extern IntPtr CreateValVector4f( float x, float y, float z, float w );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void DestroyVector4f( IntPtr pVector );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void AssignVector4f( IntPtr dest, IntPtr src );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void PlusAssignVector4f( IntPtr dest, IntPtr src );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void MinusAssignVector4f( IntPtr dest, IntPtr src );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void ScaleAssignVector4f( IntPtr dest, float s );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void MulAssignVector4f( IntPtr dest, IntPtr src );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void DivAssignVector4f( IntPtr dest, float s );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void UnaryVector4f( IntPtr dest, IntPtr src );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern bool EqualityVector4f( IntPtr lhs, IntPtr rhs );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern bool NotEqualityVector4f( IntPtr lhs, IntPtr rhs );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern void PlusVector4f( IntPtr dest, IntPtr lhs, IntPtr rhs );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern void MinusVector4f( IntPtr dest, IntPtr lhs, IntPtr rhs );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern void MulVector4f( IntPtr dest, IntPtr lhs, IntPtr rhs );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern void ScaleVector4f( IntPtr dest, IntPtr lhs, float rhs );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern void DivVector4f( IntPtr dest, IntPtr lhs, float rhs );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern float GetXVector4f( IntPtr v );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern float GetYVector4f( IntPtr v );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern float GetZVector4f( IntPtr v );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern float GetWVector4f( IntPtr v );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void SetXVector4f( IntPtr v, float val );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern void SetYVector4f( IntPtr v, float val );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void SetZVector4f( IntPtr v, float val );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void SetWVector4f( IntPtr v, float val );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern bool InBoundsVector4f( IntPtr v, IntPtr bounds );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern void NormalizeVector4f( IntPtr v );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern float LengthVector4f( IntPtr v );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern float LengthSqVector4f( IntPtr v );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern float DotVector4f( IntPtr v1, IntPtr v2 );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void ClampVector4f( IntPtr v, IntPtr min, IntPtr max );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern float DistanceVector4f( IntPtr v1, IntPtr v2 );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern float DistanceSquaredVector4f( IntPtr v1, IntPtr v2 );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void MinVector4f( IntPtr v, IntPtr v1, IntPtr v2 );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void MaxVector4f( IntPtr v, IntPtr v1, IntPtr v2 );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void LerpVector4f( IntPtr v, IntPtr v1, IntPtr v2, float t );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void SmoothStepVector4f( IntPtr v, IntPtr v1, IntPtr v2, float t );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void BarycentricVector4f( IntPtr v, IntPtr v1, IntPtr v2, IntPtr v3, float f, float g );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void CatmullRomVector4f( IntPtr v, IntPtr v1, IntPtr v2, IntPtr v3, IntPtr v4, float t );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void HermiteVector4f( IntPtr v, IntPtr v1, IntPtr t1, IntPtr v2, IntPtr t2, float t );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void ReflectVector4f( IntPtr v, IntPtr ivec, IntPtr nvec );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void RefractVector4f( IntPtr v, IntPtr ivec, IntPtr nvec, float refractionIndex );

        #endregion PInvokes

        #region Members

        private IntPtr mVector; //!< Variable to hold the C++ class's this pointer
        public float x = 0.0f;
        public float y = 0.0f;
        public float z = 0.0f;
        public float w = 0.0f;

        #endregion Members

        private void CopyValues()
        {
            x = GetXVector4f( mVector );
            y = GetYVector4f( mVector );
            z = GetZVector4f( mVector );
            w = GetZVector4f( mVector );
        }

        private void SetValues()
        {
            SetXVector4f( mVector, x );
            SetYVector4f( mVector, y );
            SetZVector4f( mVector, z );
            SetWVector4f( mVector, w );
        }

        public BHVector4f()
        {
            mVector = CreateVector4f();
        }

        public BHVector4f( float X, float Y, float Z, float W )
        {
            mVector = CreateValVector4f( X, Y, Z, W );
            x = X;
            y = Y;
            z = Z;
        }

        private BHVector4f( IntPtr v )
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
                DestroyVector4f( mVector );
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
        ~BHVector4f()
        {
            Dispose( false );
        }

        #region Wrapper methods
        
        public static BHVector4f operator - ( BHVector4f v ) 
        {
            v.SetValues();
            BHVector4f r = new BHVector4f();
            UnaryVector4f( r.mVector, v.mVector );
            r.CopyValues();
            return r;
        }

        public static BHVector4f operator + ( BHVector4f v1, BHVector4f v2 ) 
        {
            v1.SetValues();
            v2.SetValues();
            BHVector4f r = new BHVector4f();
            PlusVector4f( r.mVector, v1.mVector, v2.mVector );
            r.CopyValues();
            return r;
        }

        public static BHVector4f operator - ( BHVector4f v1, BHVector4f v2 ) 
        {
            v1.SetValues();
            v2.SetValues();
            BHVector4f r = new BHVector4f();
            MinusVector4f( r.mVector, v1.mVector, v2.mVector );
            r.CopyValues();
            return r;
        }

        public static BHVector4f operator * ( BHVector4f v, float s ) 
        {
            v.SetValues();
            BHVector4f r = new BHVector4f();
            ScaleVector4f( r.mVector, v.mVector, s );
            r.CopyValues();
            return r;
        }

        public static BHVector4f operator * ( float s, BHVector4f v ) 
        {
            v.SetValues();
            BHVector4f r = new BHVector4f();
            ScaleVector4f( r.mVector, v.mVector, s );
            r.CopyValues();
            return r;
        }

        public static BHVector4f operator * ( BHVector4f v1, BHVector4f v2 ) 
        {
            v1.SetValues();
            v2.SetValues();
            BHVector4f r = new BHVector4f();
            MulVector4f( r.mVector, v1.mVector, v2.mVector );
            r.CopyValues();
            return r;
        }

        public static BHVector4f operator / ( BHVector4f v, float s ) 
        {
            v.SetValues();
            BHVector4f r = new BHVector4f();
            DivVector4f( r.mVector, v.mVector, s );
            r.CopyValues();
            return r;
        }

        public bool InBounds( BHVector4f bounds )
        {
            SetValues();
            return InBoundsVector4f( mVector, bounds.mVector );
        }

        public void Normalize()
        {
            SetValues();
            NormalizeVector4f( mVector );
            CopyValues();
        }

        public float Length()
        {
            SetValues();
            return LengthVector4f( mVector );
        }

        public float LengthSq()
        { 
            SetValues();
            return LengthSqVector4f( mVector );
        }

        public float Dot( BHVector4f v )
        {
            v.SetValues();
            return DotVector4f( mVector, v.mVector );
        }

        public static void Clamp( BHVector4f v, BHVector4f min, BHVector4f max )
        {
            v.SetValues();
            min.SetValues();
            max.SetValues();
            ClampVector4f( v.mVector, min.mVector, max.mVector );
            v.CopyValues();
        }

        public static float Distance( BHVector4f v1, BHVector4f v2 )
        {
            v1.SetValues();
            v2.SetValues();
            return DistanceVector4f( v1.mVector, v2.mVector );
        }

        public static float DistanceSquared( BHVector4f v1, BHVector4f v2 )
        { 
            v1.SetValues();
            v2.SetValues();
            return DistanceSquaredVector4f( v1.mVector, v2.mVector );
        }

        public static BHVector4f Min( BHVector4f v1, BHVector4f v2 )
        { 
            v1.SetValues();
            v2.SetValues();
            BHVector4f r = new BHVector4f();
            MinVector4f( r.mVector, v1.mVector, v2.mVector );
            r.CopyValues();
            return r;
        }

        public static BHVector4f Max( BHVector4f v1, BHVector4f v2 )
        { 
            v1.SetValues();
            v2.SetValues();
            BHVector4f r = new BHVector4f();
            MaxVector4f( r.mVector, v1.mVector, v2.mVector );
            r.CopyValues();
            return r;
        }

        public static BHVector4f Lerp( BHVector4f v1, BHVector4f v2, float t )
        { 
            v1.SetValues();
            v2.SetValues();
            BHVector4f r = new BHVector4f();
            LerpVector4f( r.mVector, v1.mVector, v2.mVector, t );
            r.CopyValues();
            return r;
        }

        public static BHVector4f SmoothStep( BHVector4f v1, BHVector4f v2, float t )
        { 
            v1.SetValues();
            v2.SetValues();
            BHVector4f r = new BHVector4f();
            SmoothStepVector4f( r.mVector, v1.mVector, v2.mVector, t );
            r.CopyValues();
            return r;
        }

        public static BHVector4f Barycentric( BHVector4f v1, BHVector4f v2, BHVector4f v3, float f, float g )
        {
            v1.SetValues();
            v2.SetValues();
            v3.SetValues();
            BHVector4f r = new BHVector4f();
            BarycentricVector4f( r.mVector, v1.mVector, v2.mVector, v3.mVector, f, g );
            r.CopyValues();
            return r;
        }

        public static BHVector4f CatmullRom( BHVector4f v1, BHVector4f v2, BHVector4f v3, BHVector4f v4, float t )
        {
            v1.SetValues();
            v2.SetValues();
            v3.SetValues();
            v4.SetValues();
            BHVector4f r = new BHVector4f();
            CatmullRomVector4f( r.mVector, v1.mVector, v2.mVector, v3.mVector, v4.mVector, t );
            r.CopyValues();
            return r;
        }

        public static BHVector4f Hermite( BHVector4f v1, BHVector4f t1, BHVector4f v2, BHVector4f t2, float t )
        { 
            v1.SetValues();
            v2.SetValues();
            t1.SetValues();
            t2.SetValues();
            BHVector4f r = new BHVector4f();
            HermiteVector4f( r.mVector, v1.mVector, t1.mVector, v2.mVector, t2.mVector, t );
            r.CopyValues();
            return r;
        }

        public static BHVector4f Reflect( BHVector4f ivec, BHVector4f nvec )
        {
            ivec.SetValues();
            nvec.SetValues();
            BHVector4f r = new BHVector4f();
            ReflectVector4f( r.mVector, ivec.mVector, nvec.mVector );
            r.CopyValues();
            return r;
        }

        public static BHVector4f Refract( BHVector4f ivec, BHVector4f nvec, float refractionIndex )
        {
            ivec.SetValues();
            nvec.SetValues();
            BHVector4f r = new BHVector4f();
            RefractVector4f( r.mVector, ivec.mVector, nvec.mVector, refractionIndex );
            r.CopyValues();
            return r;
        }
        
        #endregion Wrapper methods
    }
}
