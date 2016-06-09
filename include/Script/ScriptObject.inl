namespace BH
{
	template< typename... Args >
	void ScriptObject::CallMethod( const Char * method, Args... args )
	{
		AddParams<Args...>()( args... );
		detail::ScriptMethod::CallMethod( this, method );
	}
}