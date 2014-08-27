
class WindowsConsole
{

public:
	static WindowsConsole* Instance()
	{
		static WindowsConsole* sInstance = nullptr;
		if( sInstance == nullptr )
		{
			sInstance = new WindowsConsole();
		}

		return sInstance;
	}
	
	void ProcessPendingInput()
	{
		if( !mHasLineEnded )
		{
			DWORD pendingInputCount = GetPendingInputEventCount();
			if( pendingInputCount > 0 )
			{
				std::vector< INPUT_RECORD > inputRecords;
				inputRecords.resize( pendingInputCount );

				DWORD readEventCount = 0;

				ReadConsoleInput( mInputHandle, inputRecords.data(), inputRecords.size(), &readEventCount );


				//now run through the events...
				for( int i = 0, count = inputRecords.size(); i < count; ++i )
				{
					const INPUT_RECORD& ir = inputRecords[ i ];
					if( ir.EventType == KEY_EVENT )
					{
						const KEY_EVENT_RECORD& ker = ir.Event.KeyEvent;
						if( ker.bKeyDown )
						{
							for( int keyIndex = 0, keyCount = ker.wRepeatCount; keyIndex < keyCount; ++keyIndex )
							{
								WCHAR c = ker.uChar.UnicodeChar;
								//process the char- if it's backspace, back up!
								switch( c )
								{
								case '\0':
									break;
								case '\b':
									if( mPendingLine.size() > 0 )
									{
										mPendingLine.pop_back();
									}
									break;
								case '\r':
									mPendingLine.push_back( '\n' );
									mHasLineEnded = true;
									break;
								default:
									mPendingLine.push_back( c );
									break;
								}
							
							}
						}
					}
				}

			}

			WritePendingLineToScreen();

		}
	}

	bool ReadPendingLine( std::wstring& outLine )
	{
		if( mHasLineEnded )
		{
			outLine.clear();
			outLine.swap( mPendingLine );
			outLine.push_back( '\0' );

			mHasLineEnded = false;
			return true;
		}

		return false;
	}


	DWORD GetPendingInputEventCount()
	{
		DWORD inputEventCount;
		if( GetNumberOfConsoleInputEvents( mInputHandle, &inputEventCount ) )
		{
			return inputEventCount;
		}
		else
		{
			//get error code?
			return -1;
		}
	}

	void FlushInputBuffer ()
	{
		FlushConsoleInputBuffer( mInputHandle );
	}

	void Write( const WCHAR* inText )
	{
		DWORD numberOfCharsWritten = 0;
		WriteConsole( mOutputHandle, inText, wcslen( inText ), &numberOfCharsWritten, nullptr );
	}


private:
	WindowsConsole()
	{
		mInputHandle = GetStdHandle( STD_INPUT_HANDLE );
		mOutputHandle = GetStdHandle( STD_OUTPUT_HANDLE );
	
		mHasLineEnded = false;
	}

	void ConvertPendingLineToCharInfo( std::vector< CHAR_INFO >& outCharInfo, int inLineWidth )
	{
		
		int pendingLineSize = mPendingLine.size();
		
		//we need to get the whole screen...
		outCharInfo.resize( inLineWidth );
		int i = 0;
		for( int count = pendingLineSize; i < count; ++i )
		{
			CHAR_INFO& ci = outCharInfo[ i ];
			WCHAR c = mPendingLine[ i ];
			if( c != '\n' )
			{
				ci.Char.UnicodeChar = c;
				ci.Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
			}
		}

		//and fill the rest with space...
		for( ; i < inLineWidth; ++i )
		{
			CHAR_INFO& ci = outCharInfo[ i ];
			ci.Char.UnicodeChar = ' ';
			ci.Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
		}
	}

	void WritePendingLineToScreen()
	{
		CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;

		GetConsoleScreenBufferInfo( mOutputHandle, &screenBufferInfo );
		
		

		//and write this to the very top?
		std::vector< CHAR_INFO > charInfo;
		ConvertPendingLineToCharInfo( charInfo, screenBufferInfo.srWindow.Right - screenBufferInfo.srWindow.Left );
		COORD bufferSize = { charInfo.size(), 1 };
		COORD bufferCoord = { 0 , 0 };
		SMALL_RECT targetRect = { 0, screenBufferInfo.srWindow.Bottom, bufferSize.X, screenBufferInfo.srWindow.Bottom + bufferSize.Y };
		WriteConsoleOutput( mOutputHandle, charInfo.data(), bufferSize, bufferCoord, &targetRect );
	}

	HANDLE	mOutputHandle;
	HANDLE	mInputHandle;

	std::wstring	mPendingLine;
	bool			mHasLineEnded;

};