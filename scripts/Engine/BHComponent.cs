using System;
using System.Text;

namespace BH
{
    public class BHComponent
    {
        public BHObject mObject;
        private string mType;

        public void SetObject( string name, string scriptType )
        {
            BHObject o;
            if( BHObject.mObjects.ContainsKey( name ) )
            {
                o = BHObject.mObjects[ name ];
            }
            else
            { 
                o = new BHObject( name );
            }
        
            o.AddComponent( scriptType, this );
            mType = scriptType;
            mObject = o;
        }

        ~BHComponent()
        {
            if( mObject != null )
            {
                mObject.RemoveComponent( mType );
            }
        }
    }
}
