//#define ENABLE_CSCOMPILER_LOG

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;
using Microsoft.CSharp;

namespace BHScriptCompiler
{
    // Type info gotten from script, stored in database
    public class CSharpType
    {
    // Member functions
        // Get the name of the type
        public string GetTypeName()
        {
            return mTypeName;
        }
    
        // Constructor
        public CSharpType( Type type )
        {
#if ENABLE_CSCOMPILER_LOG
            Console.WriteLine();
            Console.WriteLine( "Script Type: " + type.Name );
#endif
            // Extract name
            mTypeName = type.Name;

            // Extract methods
            MethodInfo[] mis = type.GetMethods();

            for ( int j = 0; j < mis.Length; ++j )
            {
                MethodInfo mi = mis[j];

                // If non public methods, we ignore
                if( !mi.IsPublic || mi.Name == "ToString" || mi.Name == "Equals" || mi.Name == "GetHashCode" || mi.Name == "GetType" )
                    continue;

                if( !mMethods.ContainsKey( mi.Name ) )
                    mMethods.Add( mi.Name, mi );

#if ENABLE_CSCOMPILER_LOG
                Console.WriteLine( type.Name + " Method " + j + ": " + mi.Name );
#endif
            }

            // Extract all reflecting / public, non reference field
            FieldInfo[] fis = type.GetFields();

            for (int j = 0; j < fis.Length; ++j)
            {
                FieldInfo fi = fis[j];

                // If private, static, protected, const, we ignore.(We only reflect public type
                if( !fi.IsPublic || fi.IsStatic || fi.IsFamily || fi.IsLiteral )
                    continue;

                mFields.Add( fi );

#if ENABLE_CSCOMPILER_LOG
                Console.WriteLine( type.Name + " Field " + j + ": " + fi.Name + " - " + fi.FieldType.Name );

                if( fi.FieldType.IsGenericType )
                {
                    Type[] gTypes = fi.FieldType.GetGenericArguments();
                    Console.Write("\t");
                    for ( int k = 0; k < gTypes.Length; ++k )
                        Console.Write( " " + gTypes[k].Name );
                    Console.WriteLine();
                }
#endif
            }
    
            // Extract default constructor
            mDefaultConstructor = type.GetConstructor( new Type[0] );
        }
    
        // Create object of this type
        public object Create()
        {
            return mDefaultConstructor.Invoke( new Type[0] );
        }
    
        // Call method using name
        public void CallMethod( object obj, string methodName, object[] parameters )
        {
            if ( !mMethods.ContainsKey( methodName ) )
            {
#if ENABLE_CSCOMPILER_LOG
                Console.WriteLine( methodName + " does not exist in class: " + mTypeName );
#endif
                return;
            }
    
#if ENABLE_CSCOMPILER_LOG
            Console.WriteLine( "Calling Class Method: " + mTypeName + "::" + methodName );
#endif

            try
            { 
                mMethods[methodName].Invoke( obj, parameters );
            }
            catch( Exception e )
            {
                Console.WriteLine( "{2} {0}: {1}", methodName, e.Message, mTypeName );
            }
        }
    
        // Get parameters info from a method
        public ParameterInfo[] GetParamtersInfo( string methodName )
        {
            if( !mMethods.ContainsKey( methodName ) )
                return new ParameterInfo[0];

            return mMethods[methodName].GetParameters();
        }

        // Get all fields
        public List<FieldInfo> GetFields()
        {
            return mFields;
        }

        // Set field value
        public void SetValueToField( object obj, string fieldName, object value )
        {
            foreach( var i in mFields )
            {
                if( i.Name == fieldName )
                {
                    i.SetValue( obj, value );
                    break;
                }
            }
        }

        // Get field value
        public object GetValueFromField( object obj, string fieldName )
        {
            foreach( var i in mFields )
            {
                if( i.Name == fieldName )
                {
                    return i.GetValue( obj );
                }
            }

            return null;
        }

        public FieldInfo GetFieldType( string fieldName )
        {
            foreach( var i in mFields )
            {
                if( i.Name == fieldName )
                {
                    return i;
                }
            }

            return null;
        }

        public bool IsFieldContainer( string fieldName )
        {
            foreach( var i in mFields )
            {
                if( i.Name == fieldName )
                {
                    return i.FieldType.IsGenericType;
                }
            }

            return false;
        }
    
    // Member variables
        private string mTypeName = "InvalidTypeName";                                           //!< Name of the type
        private Dictionary<string, MethodInfo> mMethods = new Dictionary<string, MethodInfo>(); //!< Map of methods, only class would have entries
        private List<FieldInfo> mFields = new List<FieldInfo>();                                //!< List all public field
        private ConstructorInfo mDefaultConstructor;                                            //!< Default constructor of the type to construct the type
    }
}
