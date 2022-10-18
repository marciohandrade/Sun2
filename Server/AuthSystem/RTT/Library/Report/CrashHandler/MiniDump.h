#ifndef  __MiniDump_H
#define  __MiniDump_H

class MiniDump
{
public:
	static	bool	GenerateMiniDump( PEXCEPTION_POINTERS exceptionInfo, LPCTSTR dumpFileName );

private:
			MiniDump()	{}
			~MiniDump()	{}
};

#endif