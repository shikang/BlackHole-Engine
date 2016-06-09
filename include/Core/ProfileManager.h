#ifndef BH_PROFILE_MANAGER_H
#define BH_PROFILE_MANAGER_H

#include "Core/CoreEssential.h"
#include "Core/XMLSerialiseType.h"
#include "Core/Profiler.h"
#include "Core/Singleton.h"

#include <unordered_map>

#if BH_PROFILING_ENABLED == BH_ENABLED
namespace BH
{
	class ProfileManager
	{
	public:
		DECLARE_SINGLETON(ProfileManager);

	private:
		struct ProfileNode
		{
			const char *	mFunctionName;
			String			mStrFunctionName;	//!< @hack: Because C# string get delete first
			ProfileNode *	mParent;
			ProfileNode *	mChildren;
			ProfileNode *	mSiblings;
			Profiler		mProfiler;

			ProfileNode() 
			: mFunctionName("")
			, mParent(nullptr)
			, mChildren(nullptr)
			, mSiblings(nullptr)
			, mProfiler() {}
		};

	public:
		// Destructor
		~ProfileManager();

		// Start profiler of this name
		void StartProfiler(const Char * name);

		// End profiler of this name
		void EndProfiler(const Char * name);

	private:
		// Constructor
		ProfileManager();

		// Find Node by name
		ProfileNode * FindCurrentNode(const Char * name);

		// Add Current Node
		ProfileNode * AddCurrentNode(const Char * name);

		// Report time taken for each function profiled
		void PrintProfileReport();

		// Print node
		void PrintNode( SerialiseDoc & doc,
						SerialiseElement * parent_elem,
						SerialiseElement * table_elem,
						ProfileNode * node,
						s64 total_cycles,
						bool child);

		// Clear all profiler
		void Clear();

		// Delete node
		void DeleteNode(ProfileNode * node);

		// HTML/XML functions
		void AddElement(SerialiseElement * table_elem, SerialiseDoc & doc, ProfileNode * node, s64 total_cycles);

		void AddHeader(SerialiseElement * table_elem, SerialiseDoc & doc);

		void AddFooter(SerialiseElement * table_elem, SerialiseDoc & doc, s64 parent_total, s64 child_total, u32 samples);

		SerialiseElement * AddTable(SerialiseElement * elem, SerialiseDoc & doc);

	private:
		ProfileNode * mRoot;
		ProfileNode * mCurrent;
		s64 mTotalCycles;

	public:
		static s64 Hz;							//!< Hz of the CPU, to calculate time.
		static String ReportDirectory;	//!< Path for profile report
	};

	class ScopeProfile
	{
	public:
		ScopeProfile(const Char * name)
			: mName(name)
		{
			ProfileManager::Instance().StartProfiler(mName);
		}

		~ScopeProfile()
		{
			ProfileManager::Instance().EndProfiler(mName);
		}

	private:
		const Char * mName;

	};
}	
#endif

#endif