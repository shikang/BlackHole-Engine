// Precompiled Headers
#include "Core/StdAfx.h"
#include "Core/LogManager.h"
#include "Core/Time.h"

namespace BH
{

	LogManager::LogManager( )
		: mDefaultChannelHash( Hash::Hash( "General" ) )
	{
		AddChannel( mDefaultChannelHash.GetText( ), Color::WHITE );
	}

	//////////////////////////////////////////////////

	void LogManager::ConnectStream( StreamFn fn, MsgLevel level, bool realtime )
	{
		BH_ASSERT( IsStreamExist( fn ) == false, "Stream already exist, cannot connect" );
		if ( realtime == false )
			mStreams[ fn ] = level;
		else
			mRealtimeStreams[ fn ] = level;
	}

	void LogManager::DisconnectStream( StreamFn fn )
	{
		BH_ASSERT( IsStreamExist( fn ) == true, "Stream does not exist, cannot disconnect" );

		if ( mRealtimeStreams.find( fn ) != mRealtimeStreams.end( ) )
			mRealtimeStreams.erase( fn );
		else
			mStreams.erase( fn );
	}

	void LogManager::AddChannel( const String & channelName, const Color & channelColor )
	{
		HashValue channelHash = Hash::Generate( channelName );
		BH_ASSERT( IsChannelExist( channelHash ) == false, "Channel already exist, cannot add" );

		mChannels[ channelHash ].mChannelColor = channelColor;
		mChannels[ channelHash ].mChannelName = channelName;
	}

	void LogManager::RemoveChannel( const String & channelName )
	{
		HashValue channelHash = Hash::Generate( channelName );
		BH_ASSERT( IsChannelExist( channelHash ) == true, "Channel does not exist, cannot remove" );

		mChannels.erase( channelHash );
	}

	LogManager::Channel & LogManager::GetChannel( const String & channelName )
	{
		return GetChannel( Hash::Generate( channelName ) );
	}

	LogManager::Channel & LogManager::GetChannel( HashValue channelName )
	{
		BH_ASSERT( IsChannelExist( channelName ) == true, "Channel does not exist, cannot get" );
		return mChannels[ channelName ];
	}

	LogManager::Channel & LogManager::GetDefaultChannel( )
	{
		return GetChannel( mDefaultChannelHash.GetHash( ) );
	}

	//////////////////////////////////////////////////

	void LogManager::FlushBuffers( )
	{
		// @TODO
		//  - complete this

		// used to consolidate all buffer
		std::multimap<TimePoint, Message> allMsgs;

		// consolidating all buffers
		for ( auto & ele : mChannels )
		{
			for ( u8 i = 0; i < ele.second.mMsgCount; ++i )
				allMsgs.emplace( ele.second.mMsgBuffer[ i ].mTime, ele.second.mMsgBuffer[ i ] );

			ele.second.EmptyBuffer( );
		}

		// write to all streams
		for ( auto & ele : allMsgs )
		{
			for ( auto & stream : mStreams )
			{
				stream.first( ele.second );
			}
		}
	}

	bool LogManager::IsChannelExist( const String & channel ) const
	{
		return IsChannelExist( Hash::Generate( channel ) );
	}

	bool LogManager::IsChannelExist( HashValue channel ) const
	{
		return mChannels.find( channel ) != mChannels.end( );
	}

	bool LogManager::IsStreamExist( StreamFn stream ) const
	{
		return mStreams.find( stream ) != mStreams.end( ) && mRealtimeStreams.find( stream ) != mRealtimeStreams.end( );
	}

	//------------------------------------------------------------------------------------------
	// Message
	//------------------------------------------------------------------------------------------

	LogManager::Message::Message( const TimePoint & time, MsgLevel lvl,
								  const String & file, u32 line, const String & function,
								  const String & msg )
								  : mTime( time ), mLevel( lvl ), mFile( file ), mLine( line ), mFunction( function ),
								  mMessage( msg )
	{

	}

	//------------------------------------------------------------------------------------------
	// Channel
	//------------------------------------------------------------------------------------------

	LogManager::Channel::Channel( )
	{

	}

	LogManager::Channel::~Channel( )
	{

	}

	void LogManager::Channel::Log( const TimePoint & time, const MsgLevel lvl,
								   const String & filename, u32 line, const String & function,
								   const String & msg )
	{
		if ( IsBufferFull( ) == true )
			LogManager::Instance( ).FlushBuffers( );

		mMsgBuffer[ mMsgCount ].mTime = time;
		mMsgBuffer[ mMsgCount ].mLevel = lvl;
		mMsgBuffer[ mMsgCount ].mFile = filename;
		mMsgBuffer[ mMsgCount ].mLine = line;
		mMsgBuffer[ mMsgCount ].mFunction = function;
		mMsgBuffer[ mMsgCount ].mMessage = msg;

		++mMsgCount;
	}

	void LogManager::Channel::Log( const Message & msg )
	{
		Log( msg.mTime, msg.mLevel, msg.mFile, msg.mLine, msg.mFunction, msg.mMessage );
	}

	void LogManager::Channel::EmptyBuffer( )
	{
		mMsgCount = 0;
	}

	bool LogManager::Channel::IsBufferFull( ) const
	{
		return mMsgCount >= MSG_BUFFER_COUNT;
	}

	LogManager::Message & LogManager::Channel::GetFreeMsg( )
	{
		BH_ASSERT( IsBufferFull( ) == false, "Buffer is full, cannot get free msg" );
		return mMsgBuffer[ mMsgCount ];
	}

	LogManager::Message & LogManager::Channel::operator [] ( u32 idx )
	{
		return mMsgBuffer[ idx ];
	}

	const LogManager::Message & LogManager::Channel::operator [] ( u32 idx ) const
	{
		return mMsgBuffer[ idx ];
	}




}