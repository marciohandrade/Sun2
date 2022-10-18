#pragma once

#ifdef __CN_269_MATRIX_CARD_AUTH
#include "uiBaseMan.h"

class MatrixCardAuth;
class uiMatrixCardMan : public uiBaseMan
{
public:
	uiMatrixCardMan(InterfaceManager * pUIMAN);

public:
	virtual void            OnInitialize();

private:
	MatrixCardAuth *		m_pMatrixCardAuth;
};
#endif // __CN_269_MATRIX_CARD_AUTH