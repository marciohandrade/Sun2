#ifndef __SCITEM_H__
#define __SCITEM_H__

#pragma once

struct BASE_ITEMINFO;

class SCItem
{
public:		SCItem();
			~SCItem();

	inline	BASE_ITEMINFO *			GetItemInfo() { return m_pItemInfo;	}
	inline	VOID					SetItemInfo( BASE_ITEMINFO * pItemInfo ) { m_pItemInfo = pItemInfo;	}

private:	BASE_ITEMINFO *			m_pItemInfo;
};

#endif // __SCITEM_H__