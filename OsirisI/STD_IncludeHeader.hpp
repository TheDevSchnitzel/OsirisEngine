#ifndef STD_INCLUDEHEADER_HPP_
#define STD_INCLUDEHEADER_HPP_

    #ifdef WINDOWS
	    #define WIN32_LEAN_AND_MEAN             // Selten verwendete Teile der Windows-Header nicht einbinden.
	    // Windows-Headerdateien:
        #ifndef WinHeaders
            #define WinHeaders
            #include <WinSock2.h>
	        #include <Windows.h>
            #pragma comment(lib, "ws2_32.lib")
        #endif
    #endif

	#pragma region Header der STL:
		#include <string>
		#include <atlbase.h>
        #include <stdio.h>
        #include <io.h>
        #include <direct.h>
		#include <algorithm> 
		#include <functional> 
		#include <cctype>
		#include <locale>

		//Container
		#include <vector>
		#include <map>
        #include <unordered_map>
		#include <list>
		#include <queue>
		#include <deque>
		#include <stack>

		//Streams
        #include <iostream>
        #include <istream>
		#include <sstream>
		#include <fstream>
        #include <streambuf>
		#include <strstream>
		#include <ostream>

		//Threads
		#include <mutex>
		#include <thread>
		#include <condition_variable>

		//Time
		#include <chrono>
		#include <ctime>

		//Shared_Ptr
		#include <memory>

        //Error handling
        #include <exception>
	#pragma endregion
#endif /* STD_INCLUDEHEADER_HPP_ */
