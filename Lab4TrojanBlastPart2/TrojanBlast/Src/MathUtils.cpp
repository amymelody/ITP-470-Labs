#include <TrojanBlastPCH.h>

float MathUtils::GetRandomFloat() 
{ 
	return ( float ) rand() / ( float ) RAND_MAX; 
}

XMVECTOR MathUtils::GetRandomVector( FXMVECTOR inMin, FXMVECTOR inMax ) 
{ 
	XMVECTOR r = XMVectorSet( GetRandomFloat(), GetRandomFloat(), GetRandomFloat(), GetRandomFloat()  );
	return inMin + ( inMax - inMin ) * r;
}