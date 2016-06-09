#ifndef BH_SYSTEM_MESSAGING_H
#define BH_SYSTEM_MESSAGING_H

// To be used by system only
namespace BH
{
	namespace detail
	{
		template< typename T, typename... Rest >
		inline void RecPackParams( Message & msg, T t, Rest... rest )
		{
			msg.AddMsg<T>(t);
			RecPackParams( msg, std::forward<Rest>(rest)... );
		}

		// Specialised for end case of packing message
		template< typename T >
		inline void RecPackParams( Message & msg, T t )
		{
			msg.AddMsg<T>(t);
		}

		// Helper function to prepare message
		template< typename... Args >
		inline void PackParams( Message & msg, Args... args )
		{
			RecPackParams( msg, std::forward<Args>(args)... );
		}

		// Specialised for end case ( Will never run into this situation as delegate will be added )
		//template<>
		//inline void PackParams( Message & )
		//{
		//}

		// Unpack parameters pushed into message
		template< typename T, typename... Rest, typename... Args >
		inline void RecUseParams( s32 index, const Message & msg, std::tuple<T, Rest...> *, Args*... args )
		{
			T * t = msg.GetMsg<T>( index );
			RecUseParams( index + 1, msg, static_cast< std::tuple<Rest...> *>(nullptr), args..., t );
		}

		// Specialise for end case of unpacking message
		template< typename T, typename... Args >
		inline void RecUseParams( s32 index, const Message & msg, std::tuple<T> *, Args*... args )
		{
			// Last parameter is a delegate
			T * t = msg.GetMsg<T>( index );
			( *t )( *args... );
		}

		// Worker function for calling the actual function with actual params
		template< typename... Args >
		inline void WorkImpl( const Message & msg )
		{
			RecUseParams( 0, msg, static_cast< std::tuple<Args...> * >(nullptr) );
		}

		// Specialised to handle empty message ( Will never run into this situation as delegate will be added )
		//template<>
		//inline void WorkImpl( const Message & )
		//{
		//}
	}
}

#endif