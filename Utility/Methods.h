#pragma once


template< typename Type >
inline Type GCD( Type Value1, Type Value2 )
{
	Type tmp = 0;
	while( Value1 )
	{
		tmp = Value2%Value1;
		Value2 = Value1;
		Value1 = tmp;
	}
	return Value2;
}

template< typename Type >
inline Type LCM( Type Value1, Type Value2 )
{
	return Value1/GCD(Value1,Value2)*Value2;
}

