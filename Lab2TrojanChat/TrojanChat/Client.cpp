#include "stdafx.h"

void Client::Run()
{
	WindowsConsole* wc = WindowsConsole::Instance();
	wc->FlushInputBuffer();

	WindowsConsole::Instance()->Write( L"Starting As Client!\n" );

	for (;;)
	{
		wc->ProcessPendingInput();
		std::wstring data;
		if (wc->ReadPendingLine( data ))
		{

			//do whatever you want with the data- for instance, this will write it to the console...
			if (data.size() > 0)
			{

				WindowsConsole::Instance()->Write( data.c_str() );
			}


		}
	}
}