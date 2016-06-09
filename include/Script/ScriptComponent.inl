namespace BH
{
	// Call script function
	template< typename... Args >
	void ScriptComponent::CallScriptFunction( const String & functionName, Args... args )
	{
		mScriptObject.CallMethod( functionName.c_str(), args... );
	}
}