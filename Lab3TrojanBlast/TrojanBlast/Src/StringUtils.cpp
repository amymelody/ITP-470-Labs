#include <TrojanBlastPCH.h>
#include "StringUtils.h"

wstring StringUtils::GetCommandLineArg( int inIndex )
{
	if( inIndex < __argc )
	{
		return wstring( __wargv[ inIndex ] );
	}

	return wstring();
}
