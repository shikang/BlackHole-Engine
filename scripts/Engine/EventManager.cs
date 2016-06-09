using System;
using System.Collections.Generic;
using System.Text;

namespace BH
{
    // How to use EventArgs: https://msdn.microsoft.com/en-us/library/8627sbea.aspx
    public class BHEventManager
    {
        static public BHEventManager Instance = null;
        private Dictionary< string, Delegate > mEvents = new Dictionary< string, Delegate >();

        public delegate void KeyDelegate( int keycode );
        private event KeyDelegate mKeyTriggeredEvent;
        private event KeyDelegate mKeyReleasedEvent;
        private event KeyDelegate mKeyPressedEvent;

        public delegate void MouseDelegate( int x,int y, int mousecode );
        private event MouseDelegate mMouseTriggeredEvent;
        private event MouseDelegate mMouseReleasedEvent;
        private event MouseDelegate mMousePressedEvent;

        public BHEventManager ()
        {
            Instance = this;
        }

        // Add element -> Delegate mapping if it doesn't exist or 
        // combines a delegate to an existing EventKey
        public void Add( string eventName, Delegate handler )
        {
            Delegate d;
            mEvents.TryGetValue( eventName, out d );
            mEvents[eventName] = Delegate.Combine( d, handler );
        }

        // Removes a delegate from an EventKey (if it exists) and 
        // Removes element -> Delegate mapping the last delegate is removed
        public void Remove( string eventName, Delegate handler ) 
        {
           // Call TryGetValue to ensure that an exception is not thrown if
           // attempting to remove a delegate from an EventKey not in the set
           Delegate d;
           if ( mEvents.TryGetValue( eventName, out d ) )
           {
              d = Delegate.Remove( d, handler );

              // If a delegate remains, set the new head else remove the EventKey
              if ( d != null ) 
                  mEvents[eventName] = d;
              else 
                  mEvents.Remove( eventName );
           }
        }

        // Raises the event for the indicated EventKey
        public void Raise( string eventName, Object sender, EventArgs e )
        {
            // Don't throw an exception if the EventKey is not in the set
            Delegate d;
            mEvents.TryGetValue( eventName, out d );

            if ( d != null )
            {
                // Because the dictionary can contain several different delegate types,
                // it is impossible to construct a type-safe call to the delegate at 
                // compile time. So, I call the System.Delegate type’s DynamicInvoke 
                // method, passing it the callback method’s parameters as an array of 
                // objects. Internally, DynamicInvoke will check the type safety of the 
                // parameters with the callback method being called and call the method.
                // If there is a type mismatch, then DynamicInvoke will throw an exception.
                d.DynamicInvoke( new Object[] { sender, e } );
            }
        }

        public void AddKeyTriggeredEvent( KeyDelegate d )
        {
            mKeyTriggeredEvent += d;
        }

        public void RemoveKeyTriggeredEvent( KeyDelegate d )
        {
            mKeyTriggeredEvent -= d;
        }

        public void RaiseKeyTriggeredEvent( int keycode )
        {
            if( mKeyTriggeredEvent != null )
                mKeyTriggeredEvent( keycode );
        }

        public void AddKeyReleasedEvent( KeyDelegate d )
        {
            mKeyReleasedEvent += d;
        }

        public void RemoveKeyReleasedEvent( KeyDelegate d )
        {
            mKeyReleasedEvent -= d;
        }

        public void RaiseKeyReleasedEvent( int keycode )
        {
            if( mKeyReleasedEvent != null )
                mKeyReleasedEvent( keycode );
        }

        public void AddKeyPressedEvent( KeyDelegate d )
        {
            mKeyPressedEvent += d;
        }

        public void RemoveKeyPressedEvent( KeyDelegate d )
        {
            mKeyPressedEvent -= d;
        }

        public void RaiseKeyPressedEvent( int keycode )
        {
            if( mKeyPressedEvent != null )
                mKeyPressedEvent( keycode );
        }

        public void AddMouseTriggeredEvent( MouseDelegate d )
        {
            mMouseTriggeredEvent += d;
        }

        public void RemoveMouseTriggeredEvent( MouseDelegate d )
        {
            mMouseTriggeredEvent -= d;
        }

        public void RaiseMouseTriggeredEvent( int x,int y, int mousecode )
        {
            if( mMouseTriggeredEvent != null )
                mMouseTriggeredEvent( x, y, mousecode );
        }

        public void AddMouseReleasedEvent( MouseDelegate d )
        {
            mMouseReleasedEvent += d;
        }

        public void RemoveMouseReleasedEvent( MouseDelegate d )
        {
            mMouseReleasedEvent -= d;
        }

        public void RaiseMouseReleasedEvent( int x, int y, int mousecode )
        {
            if( mMouseReleasedEvent != null )
                mMouseReleasedEvent( x, y, mousecode );
        }

        public void AddMousePressedEvent( MouseDelegate d )
        {
            mMousePressedEvent += d;
        }

        public void RemoveMousePressedEvent( MouseDelegate d )
        {
            mMousePressedEvent -= d;
        }

        public void RaiseMousePressedEvent( int x, int y, int mousecode )
        {
            if( mMousePressedEvent != null )
                mMousePressedEvent( x, y, mousecode );
        }
    }
}
