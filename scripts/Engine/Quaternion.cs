using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace BH
{
    class BHQuaternion : IDisposable
    {
        #region PInvokes

        [DllImport( "BHScriptBridge.dll" )]
        static private extern IntPtr CreateQuaternion();

        [DllImport( "BHScriptBridge.dll" )]
        static private extern IntPtr CreateValVector3f( float x, float y, float z );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern IntPtr CreateValQuaternion( float x, float y, float z, float w );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern IntPtr CreateVectorQuaternion( IntPtr v, float scalar );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern IntPtr CreateCopyQuaternion( IntPtr q );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void DestroyVector3f( IntPtr pVector );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void DestroyQuaternion( IntPtr pQuat );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void AssignQuaternion( IntPtr dest, IntPtr src );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void PlusAssignQuaternion( IntPtr dest, IntPtr src );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void MinusAssignQuaternion( IntPtr dest, IntPtr src );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void MulAssignQuaternion( IntPtr dest, IntPtr src );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void DivAssignQuaternion( IntPtr dest, IntPtr src );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void UnaryQuaternion( IntPtr dest, IntPtr src );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern bool EqualityQuaternion( IntPtr lhs, IntPtr rhs );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern bool NotEqualityQuaternion( IntPtr lhs, IntPtr rhs );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern void PlusQuaternion( IntPtr dest, IntPtr lhs, IntPtr rhs );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern void MinusQuaternion( IntPtr dest, IntPtr lhs, IntPtr rhs );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern void MulQuaternion( IntPtr dest, IntPtr lhs, IntPtr rhs );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern void RotVecQuaternion( IntPtr dest, IntPtr lhs, IntPtr rhs );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern void DivQuaternion( IntPtr dest, IntPtr lhs, IntPtr rhs );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern float GetXQuaternion( IntPtr q );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern float GetYQuaternion( IntPtr q );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern float GetZQuaternion( IntPtr q );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern float GetWQuaternion( IntPtr q );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void SetXQuaternion( IntPtr q, float val );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern void SetYQuaternion( IntPtr q, float val );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void SetZQuaternion( IntPtr q, float val );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern void SetWQuaternion( IntPtr q, float val );
        
        [DllImport( "BHScriptBridge.dll" )]
        static private extern void NormalizeQuaternion( IntPtr q );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void ConjugateQuaternion( IntPtr q );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void InverseQuaternion( IntPtr q );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern float LengthQuaternion( IntPtr q );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern float LengthSqQuaternion( IntPtr q );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern float DotQuaternion( IntPtr q1, IntPtr q2 );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void CrossQuaternion( IntPtr dest, IntPtr q1, IntPtr q2 );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void AxisAngleQuaternion( IntPtr dest, IntPtr axis, float angle );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void YawPitchRollQuaternion( IntPtr dest, float yaw, float pitch, float roll );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void LerpQuaternion( IntPtr dest, IntPtr q1, IntPtr q2, float t );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void SlerpQuaternion( IntPtr dest, IntPtr q1, IntPtr q2, float t );

        [DllImport( "BHScriptBridge.dll" )]
        static private extern void ConcatenateQuaternion( IntPtr dest, IntPtr q1, IntPtr q2 );

        #endregion PInvokes

        #region Members

        private IntPtr mQuaternion; //!< Variable to hold the C++ class's this pointer
        public float x = 0.0f;
        public float y = 0.0f;
        public float z = 0.0f;
        public float w = 0.0f;

        #endregion Members

        private void CopyValues()
        {
            x = GetXQuaternion( mQuaternion );
            y = GetYQuaternion( mQuaternion );
            z = GetZQuaternion( mQuaternion );
            w = GetWQuaternion( mQuaternion );
        }

        private void SetValues()
        {
            SetXQuaternion( mQuaternion, x );
            SetYQuaternion( mQuaternion, y );
            SetZQuaternion( mQuaternion, z );
            SetWQuaternion( mQuaternion, w );
        }

        public BHQuaternion()
        {
            mQuaternion = CreateQuaternion();
        }

        public BHQuaternion( float X, float Y, float Z, float W )
        {
            mQuaternion = CreateValQuaternion( X, Y, Z, W );
            x = X;
            y = Y;
            z = Z;
            w = W;
        }

        private BHQuaternion( BHVector3f v, float scalar )
        {
            IntPtr vec = CreateValVector3f( v.x, v.y, v.z );

            mQuaternion = CreateVectorQuaternion( vec, scalar );
            DestroyVector3f( vec );

            CopyValues();
        }

        public void Dispose()
        {
            Dispose( true );
        }

        protected virtual void Dispose( bool disposing )
        {
            if ( mQuaternion != IntPtr.Zero )
            {
                // Call the DLL Export to dispose this class
                DestroyQuaternion( mQuaternion );
                mQuaternion = IntPtr.Zero;
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
        ~BHQuaternion()
        {
            Dispose( false );
        }

        #region Wrapper methods

        public static BHQuaternion operator - ( BHQuaternion q ) 
        {
            q.SetValues();
            BHQuaternion r = new BHQuaternion();
            UnaryQuaternion( r.mQuaternion, q.mQuaternion );
            r.CopyValues();
            return r;
        }

        public static BHQuaternion operator + ( BHQuaternion q1, BHQuaternion q2 ) 
        {
            q1.SetValues();
            q2.SetValues();
            BHQuaternion r = new BHQuaternion();
            PlusQuaternion( r.mQuaternion, q1.mQuaternion, q2.mQuaternion );
            r.CopyValues();
            return r;
        }

        public static BHQuaternion operator - ( BHQuaternion q1, BHQuaternion q2 ) 
        {
            q1.SetValues();
            q2.SetValues();
            BHQuaternion r = new BHQuaternion();
            MinusQuaternion( r.mQuaternion, q1.mQuaternion, q2.mQuaternion );
            r.CopyValues();
            return r;
        }

        public static BHQuaternion operator * ( BHQuaternion q1, BHQuaternion q2 ) 
        {
            q1.SetValues();
            q2.SetValues();
            BHQuaternion r = new BHQuaternion();
            MulQuaternion( r.mQuaternion, q1.mQuaternion, q2.mQuaternion );
            r.CopyValues();
            return r;
        }

        public static BHVector3f operator * ( BHQuaternion q, BHVector3f v ) 
        {
            q.SetValues();

            IntPtr result = CreateValVector3f( 0.0f, 0.0f, 0.0f );
            IntPtr vec = CreateValVector3f( v.x, v.y, v.z );

            RotVecQuaternion( result, q.mQuaternion, vec );
            BHVector3f r = new BHVector3f( result );
            DestroyVector3f( vec );
            return r;
        }

        public static BHQuaternion operator / ( BHQuaternion lhs, BHQuaternion rhs ) 
        {
            lhs.SetValues();
            rhs.SetValues();
            BHQuaternion r = new BHQuaternion();
            DivQuaternion( r.mQuaternion, lhs.mQuaternion, rhs.mQuaternion );
            r.CopyValues();
            return r;
        }

        public void Normalize()
        {
            SetValues();
            NormalizeQuaternion( mQuaternion );
            CopyValues();
        }

        public void Conjugate()
        {
            SetValues();
            ConjugateQuaternion( mQuaternion );
            CopyValues();
        }

        public void Inverse()
        {
            SetValues();
            InverseQuaternion( mQuaternion );
            CopyValues();
        }

        public float Length()
        {
            SetValues();
            return LengthQuaternion( mQuaternion );
        }

        public float LengthSq()
        { 
            SetValues();
            return LengthSqQuaternion( mQuaternion );
        }

        public float Dot( BHQuaternion q )
        {
            q.SetValues();
            return DotQuaternion( mQuaternion, q.mQuaternion );
        }

        public static BHQuaternion Cross( BHQuaternion q1, BHQuaternion q2 )
        { 
            q1.SetValues();
            q2.SetValues();
            BHQuaternion r = new BHQuaternion();
            CrossQuaternion( r.mQuaternion, q1.mQuaternion, q2.mQuaternion );
            r.CopyValues();
            return r;
        }

        public static BHQuaternion AxisAngle( BHVector3f axis, float angle )
        {
            BHQuaternion r = new BHQuaternion();

            IntPtr a = CreateValVector3f( axis.x, axis.y, axis.z );
            AxisAngleQuaternion( r.mQuaternion, a, angle );
            DestroyVector3f( a );

            r.CopyValues();
            return r;
        }

        public static BHQuaternion YawPitchRoll( float yaw, float pitch, float roll )
        {
            BHQuaternion r = new BHQuaternion();
            YawPitchRollQuaternion( r.mQuaternion, yaw, pitch, roll );
            return r;
        }

        public static BHQuaternion Lerp( BHQuaternion q1, BHQuaternion q2, float t )
        { 
            q1.SetValues();
            q2.SetValues();
            BHQuaternion r = new BHQuaternion();
            LerpQuaternion( r.mQuaternion, q1.mQuaternion, q2.mQuaternion, t );
            r.CopyValues();
            return r;
        }

        public static BHQuaternion Slerp( BHQuaternion q1, BHQuaternion q2, float t )
        { 
            q1.SetValues();
            q2.SetValues();
            BHQuaternion r = new BHQuaternion();
            SlerpQuaternion( r.mQuaternion, q1.mQuaternion, q2.mQuaternion, t );
            r.CopyValues();
            return r;
        }

        public static BHQuaternion Concatenate( BHQuaternion q1, BHQuaternion q2 )
        { 
            q1.SetValues();
            q2.SetValues();
            BHQuaternion r = new BHQuaternion();
            ConcatenateQuaternion( r.mQuaternion, q1.mQuaternion, q2.mQuaternion );
            r.CopyValues();
            return r;
        }
        
        #endregion Wrapper methods
    }
}
