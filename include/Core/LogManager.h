#ifndef BH_LOGMANAGER_H
#define BH_LOGMANAGER_H

#include "Core/CoreEssential.h"
#include "Core/Singleton.h"

#include "Core/Color.h"
#include "Core/Time.h"

namespace BH
{

	class LogManager
	{
	public:
		DECLARE_SINGLETON( LogManager );
		~LogManager( ) = default;

	private:
		LogManager( );

	public:
		enum class MsgLevel
		{
			Todo,

			Console,
			Info,

			Warning,
			Error,

			Count
		};

	private:
		enum CONSTS
		{
			INVALID_CHANNEL = 0,
			MSG_BUFFER_COUNT = 255
		};

	private:
		// used to store all the logs and information about the logs
		struct Message
		{
			Message( ) = default;
			Message( const TimePoint & time, MsgLevel lvl,
					 const String & file, u32 line, const String & function,
					 const String & msg );

			TimePoint mTime;
			MsgLevel mLevel;

			String mFile;
			u32 mLine;
			String mFunction;

			String mMessage;
		};

		typedef std::array< Message, CONSTS::MSG_BUFFER_COUNT> MessageContainer;

		// Used to filter and categorize messsages
		// Example of channels will be "Physics Channel", "AI channel" or "Graphics Channel"
		struct Channel
		{
			// @TODO
			//   - threadsafe this 

			Channel( );
			~Channel( );

			void Log( const TimePoint & time, const MsgLevel lvl,
					  const String & filename, u32 line, const String & function,
					  const String & msg );

			void Log( const Message & msg );

			void EmptyBuffer( );

			bool IsBufferFull( ) const;
			Message & GetFreeMsg( );

			Message & operator[] ( u32 idx );
			const Message & operator[] ( u32 idx ) const;

			String mChannelName;
			Color mChannelColor;

			MessageContainer mMsgBuffer;
			u8 mMsgCount = 0;
		};

		typedef std::map<HashValue, Channel> ChannelContainer;

	public:
		// used for user to connect their stream function
		typedef void( *StreamFn ) ( const Message & msg );

	private:
		// Messages are written out to streams when internal buffer is full
		// Stream will usually be writing out to an external file or stream
		struct Stream
		{
			StreamFn mCallback;
			MsgLevel mLevel;
		};

		typedef std::map<StreamFn, MsgLevel> StreamContainer;

	public:
		void ConnectStream( StreamFn fn, MsgLevel level, bool realtime );
		void DisconnectStream( StreamFn fn );

		void AddChannel( const String & channelName, const Color & channelColor );
		void RemoveChannel( const String & channelName );

		Channel & GetChannel( const String & channelName );
		Channel & GetChannel( HashValue channelName );

		Channel & GetDefaultChannel( );

	private:
		void FlushBuffers( );

		bool IsChannelExist( HashValue channel ) const;
		bool IsChannelExist( const String & channel ) const;

		bool IsStreamExist( StreamFn stream ) const;

	private:
		ChannelContainer mChannels;
		StreamContainer mRealtimeStreams;
		StreamContainer mStreams;

		const Hash mDefaultChannelHash;
	};

}

#include "Core/LogManager.inl"

//------------------------------------------------------------------------------------------
// Macro Helper
//------------------------------------------------------------------------------------------

#define _LOGINTERNAL( channel, lvl, msg ) \
	{ \
		Ostringstream oss; \
		oss << msg; \
		channel.Log( ::BH::Time::GetCurrentGameTime(), lvl, __FILE__, __LINE__, __FUNCDNAME__, oss.str() ); \
	}

#define _LOGDEFAULTCHANNEL( lvl, msg ) \
	_LOGINTERNAL( ::BH::LogManager::Instance().GetDefaultChannel(), lvl, msg )

#define LOGBASIC( msg ) \
	::BH::LogManager::Instance().GetDefaultChannel().Log( ::BH::Time::GetCurrentGameTime(), ::BH::LogManager::MsgLevel::Info, __FILE__, __LINE__, __FUNCDNAME__, msg )

//------------------------------------------------------------------------------------------
// Default Channel Logging
//------------------------------------------------------------------------------------------

#define LOG( msg ) \
	_LOGDEFAULTCHANNEL( ::BH::LogManager::MsgLevel::Info, msg )

#define LOGINFO( msg ) \
	LOG( msg )

#define LOGWARNING( msg ) \
	_LOGDEFAULTCHANNEL( ::BH::LogManager::MsgLevel::Warning, msg )

#define LOGERROR( msg ) \
	_LOGDEFAULTCHANNEL( ::BH::LogManager::MsgLevel::Error, msg )

//------------------------------------------------------------------------------------------
// Channel Specific Logging
//------------------------------------------------------------------------------------------

#define LOG_CHANNEL( channel, msg ) \
	_LOGINTERNAL( channel, ::BH::LogManager::MsgLevel::Info, msg )

#define LOGINFO_CHANNEL( channel, msg ) \
	LOG( msg )

#define LOGWARNING_CHANNEL( channel, msg ) \
	_LOGINTERNAL( channel, ::BH::LogManager::MsgLevel::Warning, msg )

#define LOGERROR_CHANNEL( channel, msg ) \
	_LOGINTERNAL( channel, ::BH::LogManager::MsgLevel::Error, msg )

//------------------------------------------------------------------------------------------
// Sample Code
//------------------------------------------------------------------------------------------

#if 0

LOG( "Hello World" );
LOG( "Hello" << " World" );

#endif

#endif 