// Hashed string object.
namespace BH
{
	inline Hash::Hash()
	: mText("")
	, mHash(0)
	{
	}

	inline Hash::Hash(const Char* name)
	: mText(name ? name : "")
	, mHash(name ? Generate(name) : 0)
	{
	}

	inline Hash::Hash(const String& name)
	: mText(name)
	, mHash( Generate( name ) )
	{
	}

	inline bool Hash::operator != (const Hash & rhs) const 
	{
		return mHash != rhs.GetHash();
	}

	inline bool Hash::operator == (const Hash & rhs) const 
	{
		return mHash == rhs.GetHash();
	}

	inline bool Hash::operator == (const HashValue & rhs) const 
	{
		return mHash == rhs;
	}

	inline bool Hash::operator < (const Hash & rhs) const 
	{
		return mHash < rhs.GetHash();
	}

	inline bool Hash::operator >(const Hash & rhs) const 
	{
		return mHash > rhs.GetHash();
	}

	inline HashValue Hash::Generate(const Char* string) 
	{
		return Generate( String(string) );
	}

	inline HashValue Hash::Generate(const String& string) 
	{
		if (string == "")
			return 0;

		return HashFunctor()(string);
	}
}