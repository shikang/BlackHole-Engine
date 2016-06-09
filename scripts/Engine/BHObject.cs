using System;
using System.Collections.Generic;
using System.Text;

namespace BH
{
    public class BHObject
    {
        static public Dictionary< string, BHObject > mObjects = new Dictionary< string, BHObject >();
        private Dictionary< string, object > mComponents = new Dictionary< string, object >();
        private string mName;

        public BHObject ( string name )
        {
            mObjects.Add( name, this );
            mName = name;
        }

        public T GetComponent< T >()
        {
            if ( !mComponents.ContainsKey( typeof( T ).Name ) )
            {
                return default( T );
            }
        
            return ( T )mComponents[ typeof( T ).Name ];
        }
        
        public void AddComponent< T >( object component )
        {
            if( !mComponents.ContainsKey( typeof( T ).Name ) )
                mComponents.Add( typeof( T ).Name, component );
        }

        public void AddComponent( string type, object component )
        {
            if( !mComponents.ContainsKey( type ) )
                mComponents.Add( type, component );
        }
        
        public void RemoveComponent< T >()
        {
            if( mComponents.ContainsKey( typeof( T ).Name ) )
                mComponents.Remove( typeof( T ).Name );
        }

        public void RemoveComponent( string type )
        {
            if( mComponents.ContainsKey( type ) )
                mComponents.Remove( type );
        }

        public string GetName()
        {
            return mName;
        }
    }
}
