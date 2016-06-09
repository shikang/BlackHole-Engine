#ifndef BH_PROFILING_H
#define BH_PROFILING_H

#include "Core/CoreConfig.h"
#include "Core/ProfileManager.h"

#if BH_PROFILING_ENABLED == BH_ENABLED

#define START_PROFILE(name)\
	::BH::ProfileManager::Instance().StartProfiler(name)

#define END_PROFILE(name)\
	::BH::ProfileManager::Instance().EndProfiler(name)

#define PROFILE_SECTION(name)\
	::BH::ScopeProfile _scopeprofile(name)

#define PROFILE_FUNCTION\
	::BH::ScopeProfile _scopeprofile(__FUNCSIG__)

#else

#define START_PROFILE(name)

#define END_PROFILE(name)

#define PROFILE_SECTION(name)

#define PROFILE_FUNCTION

#endif

#endif
