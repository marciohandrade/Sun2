/**
 *	@file		ScreenShotSaver.h
 *	@brief		ScreenShotSaver class	
 *  @author		Unknown
 *	@version	Unknown
 */

// ScreenShotSaver.h

#pragma once

#include <Singleton.h>

class ScreenShotSaver : public Singleton<ScreenShotSaver> 
{
public:
	ScreenShotSaver( void );
	virtual ~ScreenShotSaver( void );

	bool Prepare( void );
	bool Save( void );
	void SaveNow( void );

	bool IsPrepared( void ) const;

private:
	void Initialize( void );
	void RenderNoticeMessage( bool prepared );

private:
	TCHAR m_fileName[MAX_PATH];
	TCHAR m_filePath[MAX_PATH];

	bool m_prepared;
};

//------------------------------------------------------------------------------
/**
*/
inline bool ScreenShotSaver::IsPrepared( void ) const
{
	return m_prepared;
}
