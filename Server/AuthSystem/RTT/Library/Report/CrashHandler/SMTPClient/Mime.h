#ifndef  _Mime_H_
#define  _Mime_H_

class Mime
{
public:
			Mime();
			~Mime();

			char*	GetContentType( const char* extension );
			char*	GetDefaultBoundary(){ return m_defaultBoundary; };

			char*	CreateMimeBoundary();

private:
			char	m_contentType[128];
			char	m_defaultBoundary[128];
			char	m_boundry[128];
};

#endif