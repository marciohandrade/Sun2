//-------------------------------------------------------------------------------------------
/** 
	FTextSentence   
	@Author : y2jinc ( y2inc@webzen.co.kr )
	@Desc  : FTextParam 들의 모임
	@Since : 2006. 3. 13.	
	@Function
	@TODO
*/

#pragma once

class FTextParam;

class FTextSentence
{
public:
	FTextSentence(void);
	FTextSentence( FTextSentence & rSentence );
	virtual ~FTextSentence(void);

	void Release();

	void Pushback( const FTextParam & rParam );

	int	 Size() const;

	const FTextParam & 
		 GetParam(int index) const;

	// operator
	FTextSentence & operator =( FTextSentence & rSentence );
    
private:
	std::vector<FTextParam *> VecParam;
};
