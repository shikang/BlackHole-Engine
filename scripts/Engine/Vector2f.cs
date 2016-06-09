using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace BH
{
    public class BHVector2f : IDisposable
    {
        #region PInvokes

        [DllImport( "BHScriptBridge.dll" )]
        static private extern IntPtr CreateVector2f();

        [DllImport( "BHScriptBridge.dll" )]
        static private extern IntPtr CreateValVector2f( float x, float y );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void DestroyVector2f( IntPtr pVector );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void AssignVector2f( IntPtr dest, IntPtr src );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void PlusAssignVector2f( IntPtr dest, IntPtr src );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void MinusAssignVector2f( IntPtr dest, IntPtr src );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void ScaleAssignVector2f( IntPtr dest, float s );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void MulAssignVector2f( IntPtr dest, IntPtr src );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void DivAssignVector2f( IntPtr dest, float s );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void UnaryVector2f( IntPtr dest, IntPtr src );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern bool EqualityVector2f( IntPtr lhs, IntPtr rhs );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern bool NotEqualityVector2f( IntPtr lhs, IntPtr rhs );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern void PlusVector2f( IntPtr dest, IntPtr lhs, IntPtr rhs );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern void MinusVector2f( IntPtr dest, IntPtr lhs, IntPtr rhs );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern void MulVector2f( IntPtr dest, IntPtr lhs, IntPtr rhs );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern void ScaleVector2f( IntPtr dest, IntPtr lhs, float rhs );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern void DivVector2f( IntPtr dest, IntPtr lhs, float rhs );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern float ScalarTripleProductVector2f( IntPtr v1, IntPtr v2 );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern float GetXVector2f( IntPtr v );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern float GetYVector2f( IntPtr v );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void SetXVector2f( IntPtr v, float val );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern void SetYVector2f( IntPtr v, float val );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern bool InBoundsVector2f( IntPtr v, IntPtr bounds );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern void NormalizeVector2f( IntPtr v );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern float LengthVector2f( IntPtr v );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern float LengthSqVector2f( IntPtr v );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern float DotVector2f( IntPtr v1, IntPtr v2 );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void ClampVector2f( IntPtr v, IntPtr min, IntPtr max );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern float DistanceVector2f( IntPtr v1, IntPtr v2 );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern float DistanceSquaredVector2f( IntPtr v1, IntPtr v2 );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void MinVector2f( IntPtr v, IntPtr v1, IntPtr v2 );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void MaxVector2f( IntPtr v, IntPtr v1, IntPtr v2 );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void LerpVector2f( IntPtr v, IntPtr v1, IntPtr v2, float t );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void SmoothStepVector2f( IntPtr v, IntPtr v1, IntPtr v2, float t );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void BarycentricVector2f( IntPtr v, IntPtr v1, IntPtr v2, IntPtr v3, float f, float g );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void CatmullRomVector2f( IntPtr v, IntPtr v1, IntPtr v2, IntPtr v3, IntPtr v4, float t );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void HermiteVector2f( IntPtr v, IntPtr v1, IntPtr t1, IntPtr v2, IntPtr t2, float t );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void ReflectVector2f( IntPtr v, IntPtr ivec, IntPtr nvec );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void RefractVector2f( IntPtr v, IntPtr ivec, IntPtr nvec, float refractionIndex );

        #endregion PInvokes

        #region Members

        private IntPtr mVector; //!< Variable to hold the C++ class's this pointer
        public float x = 0.0f;
        public float y = 0.0f;

        #endregion Members

        private void CopyValues()
        {
            x = GetXVector2f( mVector );
            y = GetYVector2f( mVector );
        }

        private void SetValues()
        {
            SetXVector2f( mVector, x );
            SetYVector2f( mVector, y );
        }

        public BHVector2f()
        {
            mVector = CreateVector2f();
        }

        public BHVector2f( float X, float Y )
        {
            mVector = CreateValVector2f( X, Y );
            x = X;
            y = Y;
        }

        private BHVector2f( IntPtr v )
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
                DestroyVector2f( mVector );
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
        ~BHVector2f()
        {
            Dispose( false );
        }

        #region Wrapper methods

        public static BHVector2f operator -( BHVector2f v ) 
        {
            v.SetValues();
            BHVector2f r = new BHVector2f();
            UnaryVector2f( r.mVector, v.mVector );
            r.CopyValues();
            return r;
        }

        public static BHVector2f operator + ( BHVector2f v1, BHVector2f v2 ) 
        {
            v1.SetValues();
            v2.SetValues();
            BHVector2f r = new BHVector2f();
            PlusVector2f( r.mVector, v1.mVector, v2.mVector );
            r.CopyValues();
            return r;
        }

        public static BHVector2f operator - ( BHVector2f v1, BHVector2f v2 ) 
        {
            v1.SetValues();
            v2.SetValues();
            BHVector2f r = new BHVector2f();
            MinusVector2f( r.mVector, v1.mVector, v2.mVector );
            r.CopyValues();
            return r;
        }

        public static BHVector2f operator * ( BHVector2f v, float s ) 
        {
            v.SetValues();
            BHVector2f r = new BHVector2f();
            ScaleVector2f( r.mVector, v.mVector, s );
            r.CopyValues();
            return r;
        }

        public static BHVector2f operator * ( float s, BHVector2f v ) 
        {
            v.SetValues();
            BHVector2f r = new BHVector2f();
            ScaleVector2f( r.mVector, v.mVector, s );
            r.CopyValues();
            return r;
        }

        public static BHVector2f operator * ( BHVector2f v1, BHVector2f v2 ) 
        {
            v1.SetValues();
            v2.SetValues();
            BHVector2f r = new BHVector2f();
            MulVector2f( r.mVector, v1.mVector, v2.mVector );
            r.CopyValues();
            return r;
        }

        public static BHVector2f operator / ( BHVector2f v, float s ) 
        {
            v.SetValues();
            BHVector2f r = new BHVector2f();
            DivVector2f( r.mVector, v.mVector, s );
            r.CopyValues();
            return r;
        }

        public float ScalarTripleProduct( BHVector2f v )
        {
            v.SetValues();
            SetValues();
            return ScalarTripleProductVector2f( mVector, v.mVector );
        }

        public bool InBounds( BHVector2f bounds )
        {
            SetValues();
            return InBoundsVector2f( mVector, bounds.mVector );
        }

        public void Normalize()
        {
            SetValues();
            NormalizeVector2f( mVector );
            CopyValues();
        }

        public float Length()
        {
            SetValues();
            return LengthVector2f( mVector );
        }

        public float LengthSq()
        { 
            SetValues();
            return LengthSqVector2f( mVector );
        }

        public float Dot( BHVector2f v )
        {
            v.SetValues();
            return DotVector2f( mVector, v.mVector );
        }

        public static void Clamp( BHVector2f v, BHVector2f min, BHVector2f max )
        {
            v.SetValues();
            min.SetValues();
            max.SetValues();
            ClampVector2f( v.mVector, min.mVector, max.mVector );
            v.CopyValues();
        }

        public static float Distance( BHVector2f v1, BHVector2f v2 )
        {
            v1.SetValues();
            v2.SetValues();
            return DistanceVector2f( v1.mVector, v2.mVector );
        }

        public static float DistanceSquared( BHVector2f v1, BHVector2f v2 )
        { 
            v1.SetValues();
            v2.SetValues();
            return DistanceSquaredVector2f( v1.mVector, v2.mVector );
        }

        public static BHVector2f Min( BHVector2f v1, BHVector2f v2 )
        { 
            v1.SetValues();
            v2.SetValues();
            BHVector2f r = new BHVector2f();
            MinVector2f( r.mVector, v1.mVector, v2.mVector );
            r.CopyValues();
            return r;
        }

        public static BHVector2f Max( BHVector2f v1, BHVector2f v2 )
        { 
            v1.SetValues();
            v2.SetValues();
            BHVector2f r = new BHVector2f();
            MaxVector2f( r.mVector, v1.mVector, v2.mVector );
            r.CopyValues();
            return r;
        }

        public static BHVector2f Lerp( BHVector2f v1, BHVector2f v2, float t )
        { 
            v1.SetValues();
            v2.SetValues();
            BHVector2f r = new BHVector2f();
            LerpVector2f( r.mVector, v1.mVector, v2.mVector, t );
            r.CopyValues();
            return r;
        }

        public static BHVector2f SmoothStep( BHVector2f v1, BHVector2f v2, float t )
        { 
            v1.SetValues();
            v2.SetValues();
            BHVector2f r = new BHVector2f();
            SmoothStepVector2f( r.mVector, v1.mVector, v2.mVector, t );
            r.CopyValues();
            return r;
        }

        public static BHVector2f Barycentric( BHVector2f v1, BHVector2f v2, BHVector2f v3, float f, float g )
        {
            v1.SetValues();
            v2.SetValues();
            v3.SetValues();
            BHVector2f r = new BHVector2f();
            BarycentricVector2f( r.mVector, v1.mVector, v2.mVector, v3.mVector, f, g );
            r.CopyValues();
            return r;
        }

        public static BHVector2f CatmullRom( BHVector2f v1, BHVector2f v2, BHVector2f v3, BHVector2f v4, float t )
        {
            v1.SetValues();
            v2.SetValues();
            v3.SetValues();
            v4.SetValues();
            BHVector2f r = new BHVector2f();
            CatmullRomVector2f( r.mVector, v1.mVector, v2.mVector, v3.mVector, v4.mVector, t );
            r.CopyValues();
            return r;
        }

        public static BHVector2f Hermite( BHVector2f v1, BHVector2f t1, BHVector2f v2, BHVector2f t2, float t )
        { 
            v1.SetValues();
            v2.SetValues();
            t1.SetValues();
            t2.SetValues();
            BHVector2f r = new BHVector2f();
            HermiteVector2f( r.mVector, v1.mVector, t1.mVector, v2.mVector, t2.mVector, t );
            r.CopyValues();
            return r;
        }

        public static BHVector2f Reflect( BHVector2f ivec, BHVector2f nvec )
        {
            ivec.SetValues();
            nvec.SetValues();
            BHVector2f r = new BHVector2f();
            ReflectVector2f( r.mVector, ivec.mVector, nvec.mVector );
            r.CopyValues();
            return r;
        }

        public static BHVector2f Refract( BHVector2f ivec, BHVector2f nvec, float refractionIndex )
        {
            ivec.SetValues();
            nvec.SetValues();
            BHVector2f r = new BHVector2f();
            RefractVector2f( r.mVector, ivec.mVector, nvec.mVector, refractionIndex );
            r.CopyValues();
            return r;
        }
        
        #endregion Wrapper methods
    }
}
