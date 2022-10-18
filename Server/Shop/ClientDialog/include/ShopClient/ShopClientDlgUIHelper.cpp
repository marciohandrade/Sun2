#include "stdafx.h"
#include <ShopClient/ShopClientDlgUIHelper.h>
#include <ShopClient/ShopClientDlg.h>
#include <public/ShopDefine.h>
#include <public/protocol_shop.h>

CShopClientDlgUIHelper::CShopClientDlgUIHelper()	{}
CShopClientDlgUIHelper::~CShopClientDlgUIHelper()	{}

void CShopClientDlgUIHelper::init(CShopClientDlg* dlg) 
{
	dlg_ = dlg;
	registResource();

	// 기본값 : 임의지정
	setText(IDC_SHOP_EDIT_IP, CString(_T("127.0.0.1")));

	// 게임 선택 콤보박스
	dlg_->m_GameType.ResetContent();
	dlg_->m_GameType.InsertString(0,		_T("MU_KR"));
	dlg_->m_GameType.InsertString(1,		_T("MU_GB"));
	dlg_->m_GameType.InsertString(2,		_T("MU_JP"));
	dlg_->m_GameType.InsertString(3,		_T("MU_VN"));
	dlg_->m_GameType.InsertString(4,		_T("MU_TW"));
	dlg_->m_GameType.InsertString(5,		_T("MU_TH"));
	dlg_->m_GameType.InsertString(6,		_T("BT_KR"));
	dlg_->m_GameType.InsertString(7,		_T("BT_CHANNELING"));
	dlg_->m_GameType.InsertString(8,		_T("R2_KR"));
	dlg_->m_GameType.InsertString(9,		_T("SUN_KR"));
	dlg_->m_GameType.InsertString(10,		_T("SUN_JP"));
	dlg_->m_GameType.InsertString(11,		_T("SUN_GB"));
	dlg_->m_GameType.InsertString(12,		_T("BT_TH"));
	dlg_->m_GameType.InsertString(13,		_T("SUN_RU"));
	dlg_->m_GameType.InsertString(14,		_T("SUN_CN"));

	// 새로 추가된 게임이 기본임
	dlg_->m_GameType.SetCurSel(dlg_->m_GameType.GetCount()-1);
	changeComboGameType();
}

void CShopClientDlgUIHelper::registResource()
{
	// EDIT 컨트롤만 등록함.
	resource_list_.push_back(IDC_SHOP_EDIT_MILEAGE_TYPE                                    );
	resource_list_.push_back(IDC_SHOP_EDIT_MILEAGELIVE_TYPE                                );
	resource_list_.push_back(IDC_SHOP_EDIT_STORAGEUSE_ROLLBACK_SSEQ                        );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_TYPE                                    );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_ACCOUNT                                 );	
	resource_list_.push_back(IDC_SHOP_EDIT_BUY_ACCOUNT                                     );
	resource_list_.push_back(IDC_SHOP_EDIT_BUY_GAMECODE                                    );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_STORAGE_ACCOUNT                         );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_STORAGE_GAMECODE                        );
	resource_list_.push_back(IDC_SHOP_EDIT_BUY_PACKAGESEQ                                  );
	resource_list_.push_back(IDC_SHOP_EDIT_BUY_DISPLAYSEQ                                  );
	resource_list_.push_back(IDC_SHOP_EDIT_BUY_ZONE                                        );
	resource_list_.push_back(IDC_SHOP_EDIT_BUY_PRICE                                       );
	resource_list_.push_back(IDC_SHOP_EDIT_BUY_PRODUCTCLASS                                );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_STORAGE_SalesZone                       );
	resource_list_.push_back(IDC_SHOP_EDIT_GIFT_SENDER                                     );
	resource_list_.push_back(IDC_SHOP_EDIT_GIFT_SENDSSERVERINDEX                           );
	resource_list_.push_back(IDC_SHOP_EDIT_GIFT_MESSAGE                                    );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_GAMECODE                                );
	resource_list_.push_back(IDC_SHOP_EDIT_GIFT_PRODUCTCODE                                );
	resource_list_.push_back(IDC_SHOP_EDIT_GIFT_PRODUCTNAME                                );
	resource_list_.push_back(IDC_SHOP_EDIT_BUY_LEVEL                                       );
	resource_list_.push_back(IDC_SHOP_EDIT_GIFT_ZONE                                       );
	resource_list_.push_back(IDC_SHOP_EDIT_GIFT_PRICE                                      );
	resource_list_.push_back(IDC_SHOP_EDIT_BUY_CHARNAME                                    );
	resource_list_.push_back(IDC_SHOP_EDIT_GIFTCASH_SENDER                                 );
	resource_list_.push_back(IDC_SHOP_EDIT_GIFTCASH_SENDSERVERINDEX                        );
	resource_list_.push_back(IDC_SHOP_EDIT_GIFTCASH_CASHVALUE                              );
	resource_list_.push_back(IDC_SHOP_EDIT_GIFTCASH_GAMECODE                               );
	resource_list_.push_back(IDC_SHOP_EDIT_GIFTCASH_MSG                                    );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_STORAGEUSE_CLASS                        );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_STORAGEUSE_LEVEL                        );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_STORAGEUSE_CHARNAME                     );
	resource_list_.push_back(IDC_SHOP_EDIT_BUY_RANK                                        );
	resource_list_.push_back(IDC_SHOP_EDIT_BUY_SERVERINDEX                                 );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_STORAGEUSE_RANK                         );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_STORAGEUSE_SERVERINDEX                  );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_MILEAGE                                 );
	resource_list_.push_back(IDC_SHOP_EDIT_GIFTCASH_SENDERCHAR                             );
	resource_list_.push_back(IDC_SHOP_EDIT_GIFTCASH_RECEIVER                               );
	resource_list_.push_back(IDC_SHOP_EDIT_GIFTCASH_REVSERVERINDEX                         );
	resource_list_.push_back(IDC_SHOP_EDIT_GIFTCASH_REVCHAR                                );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_CHECK_ACCOUNT                           );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_CHECK_GAMECODE                          );
	resource_list_.push_back(IDC_SHOP_EDIT_GIFT_SENDCHAR                                   );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_PRODUCTLIST_SalesZone                   );
	resource_list_.push_back(IDC_SHOP_EDIT_GIFT_RECEIVER_STR                               );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_EVENTPRODUCTLIST_ACCOUNT                );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_EVENTPRODUCTLIST_SALESZONE              );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_EVENTPRODUCTLIST_SEQ                    );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_STORAGEUSE_ACCOUNT                      );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_STORAGEUSE_GAMECODE                     );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_STORAGEUSE_IP                           );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_STORAGEUSE_SSEQ                         );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_STORAGEUSE_PSEQ                         );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_STORAGEUSE_SEP                          );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_STORAGEDEL_ACCOUNT                      );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_STORAGEDEL_GAMECODE                     );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_STORAGEDEL_SSEQ                         );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_STORAGEDEL_PSEQ                         );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_STORAGEDEL_SEP                          );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_PRODUCTLEFTCOUNT                        );
	resource_list_.push_back(IDC_SHOP_EDIT_GIFT_RECSERVERINDEX                             );
	resource_list_.push_back(IDC_SHOP_EDIT_GIFT_RECCHAR                                    );
	resource_list_.push_back(IDC_SHOP_EDIT_MILEAGE_ACCOUNT                                 );
	resource_list_.push_back(IDC_SHOP_EDIT_MILEAGE_GAMECODE                                );
	resource_list_.push_back(IDC_SHOP_EDIT_MILEAGELIVE_ACCOUNT                             );
	resource_list_.push_back(IDC_SHOP_EDIT_MILEAGE_SECTION                                 );
	resource_list_.push_back(IDC_SHOP_EDIT_MILEAGE_POINT                                   );
	resource_list_.push_back(IDC_SHOP_EDIT_MILEAGE_DUDECT_POINT                            );
	resource_list_.push_back(IDC_SHOP_EDIT_MILEAGE_DUDECT_CLASS                            );
	resource_list_.push_back(IDC_SHOP_EDIT_MILEAGE_DUDECT_LEVEL                            );
	resource_list_.push_back(IDC_SHOP_EDIT_MILEAGE_DUDECT_CHARNAME                         );
	resource_list_.push_back(IDC_SHOP_EDIT_MILEAGE_DUDECT_RANK                             );
	resource_list_.push_back(IDC_SHOP_EDIT_MILEAGE_DUDECT_SERVERINDEX                      );
	resource_list_.push_back(IDC_SHOP_EDIT_MILEAGE_DUDECT_ACCOUNT                          );
	resource_list_.push_back(IDC_SHOP_EDIT_MILEAGE_DUDECT_GAMECODE                         );
	resource_list_.push_back(IDC_SHOP_EDIT_MILEAGE_DUDECT_TYPE                             );
	resource_list_.push_back(IDC_SHOP_EDIT_MILEAGE_DUDECT_SECTION                          );
	resource_list_.push_back(IDC_SHOP_EDIT_MILEAGELIVE_GAMECODE                            );
	resource_list_.push_back(IDC_SHOP_EDIT_MILEAGELIVE_SECTION                             );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_ACCOUNT_STR                             );
	resource_list_.push_back(IDC_SHOP_EDIT_ITEMSERIAL_ACCOUNT                              );
	resource_list_.push_back(IDC_SHOP_EDIT_ITEMSERIAL_GAMECODE                             );
	resource_list_.push_back(IDC_SHOP_EDIT_ITEMSERIAL_SSEQ                                 );
	resource_list_.push_back(IDC_SHOP_EDIT_ITEMSERIAL_PSEQ                                 );
	resource_list_.push_back(IDC_SHOP_EDIT_ITEMSERIAL_SERIAL                               );
	resource_list_.push_back(IDC_SHOP_EDIT_BUY_ACCOUNT_STR                                 );
	resource_list_.push_back(IDC_SHOP_EDIT_BUY_IP                                          );
	resource_list_.push_back(IDC_SHOP_EDIT_STORAGEUSE_ROLLBACK_PSEQ                        );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_POINT_PERCENTAGE_GAMECODE               );
	resource_list_.push_back(IDC_SHOP_EDIT_GIFT_GAMECODE                                   );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_POINT_PERCENTAGE_SERVERCODE             );
	resource_list_.push_back(IDC_SHOP_EDIT_GIFT_SENDER_STR                                 );
	resource_list_.push_back(IDC_SHOP_EDIT_GIFT_IP                                         );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_POINT_PERCENTAGE_ACCESSTYPE             );
	resource_list_.push_back(IDC_SHOP_EDIT_MILEAGE_CLASS                                   );
	resource_list_.push_back(IDC_SHOP_EDIT_MILEAGE_LEVEL                                   );
	resource_list_.push_back(IDC_SHOP_EDIT_MILEAGE_DUDECT_CHARNAME                         );
	resource_list_.push_back(IDC_SHOP_EDIT_MILEAGE_CHARNAME                                );
	resource_list_.push_back(IDC_SHOP_EDIT_MILEAGE_RANK                                    );
	resource_list_.push_back(IDC_SHOP_EDIT_MILEAGE_SERVERINDEX                             );
	resource_list_.push_back(IDC_SHOP_EDIT_BUY_REFKEY                                      );
	resource_list_.push_back(IDC_SHOP_EDIT_BUY_CASHTYPECODE                                );
	resource_list_.push_back(IDC_SHOP_EDIT_GIFT_REFKEY                                     );
	resource_list_.push_back(IDC_SHOP_EDIT_GIFT_CASHTYPECODE                               );
	resource_list_.push_back(IDC_SHOP_EDIT_GIFT_RECEIVER                                   );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_EVENTPRODUCTLIST_GAMECODE               );
	resource_list_.push_back(IDC_SHOP_EDIT_MILEAGELIVE_SERVERTYPE                          );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_CHECK_GAMECODE                          );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_PRODUCTLEFTCOUNT_GAMECODE               );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_SHOPVERSION_SALESZONE                   );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_BANNERVERSION_BANNERZONE                );
	resource_list_.push_back(IDC_SHOP_EDIT_STORAGEUSE_ROLLBACK_ACCOUNT                     );
	resource_list_.push_back(IDC_SHOP_EDIT_STORAGEUSE_ROLLBACK_GAMECODE                    );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_PAYMENTTYPE                             );
	resource_list_.push_back(IDC_SHOP_EDIT_BUY_PAYMENTTYPE                                 );
	resource_list_.push_back(IDC_SHOP_EDIT_GIFT_PAYMENTTYPE                                );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_SHOPVERSION_GAMECODE                    );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_MEMBERNO                                );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_STORAGEPAGE_ACCOUNT                     );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_STORAGEPAGE_GAMECODE                    );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_STORAGEPAGE_SalesZone                   );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_STORAGEPAGE_STORAGETYPE                 );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_STORAGEPAGE_NOWPAGE                     );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_STORAGEPAGE_PAGESIZE                    );
	resource_list_.push_back(IDC_SHOP_EDIT_BUY_MEMBERNO                                    );
	resource_list_.push_back(IDC_SHOP_EDIT_GIFT_ACCESSIP                                   );
	resource_list_.push_back(IDC_SHOP_EDIT_IDC_SHOP_EDIT_GIFT_MEMBERNO                     );
	resource_list_.push_back(IDC_SHOP_EDIT_GIFT_MEMBERNO                                   );
	resource_list_.push_back(IDC_SHOP_EDIT_BUY_ACCESSIP                                    );
	resource_list_.push_back(IDC_SHOP_EDIT_GIFTCASH_ACCESSIP                               );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_STORAGE_ACCOUNT_EX                      );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_STORAGE_GAMECODE_EX                     );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_STORAGE_SalesZone_EX                    );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_STORAGEPAGE_ACCOUNT_EX                  );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_STORAGEPAGE_GAMECODE_EX                 );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_STORAGEPAGE_SalesZone_EX                );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_STORAGEPAGE_STORAGETYPE_EX              );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_STORAGEPAGE_NOWPAGE_EX                  );
	resource_list_.push_back(IDC_SHOP_EDIT_INQUIRE_STORAGEPAGE_PAGESIZE_EX                 );
}



void CShopClientDlgUIHelper::enable(int id)
{
	((CButton *)dlg_->GetDlgItem(id))->EnableWindow(TRUE);
}

void CShopClientDlgUIHelper::disable(int id)
{
	((CButton *)dlg_->GetDlgItem(id))->EnableWindow(FALSE);
}

void CShopClientDlgUIHelper::setText(int id, CString& str)
{
	dlg_->SetDlgItemText(id, str);
}


void CShopClientDlgUIHelper::initControls()
{
	// enable all
	{
		std::vector<int>::iterator it = resource_list_.begin();
		for (; it!=resource_list_.end(); ++it) {
			enable(*it);
		}
	}

	// set text all
	{
		CString str(_T(""));
		std::vector<int>::iterator it = resource_list_.begin();
		for (; it!=resource_list_.end(); ++it) {
			setText(*it, str);
		}

		((CButton*)dlg_->GetDlgItem(IDC_SHOP_CHK_BUY_MILEAGE))->SetCheck(0);
		((CButton*)dlg_->GetDlgItem(IDC_SHOP_CHK_GIFT_MILEAGE))->SetCheck(0);
		enable(IDC_SHOP_CHK_BUY_MILEAGE);
		enable(IDC_SHOP_CHK_GIFT_MILEAGE);
		dlg_->UpdateData(FALSE);
	}
}

void CShopClientDlgUIHelper::setMU_KR()
{
	initControls();
	disable(IDC_SHOP_EDIT_INQUIRE_PAYMENTTYPE);
	disable(IDC_SHOP_EDIT_INQUIRE_ACCOUNT_STR);
	disable(IDC_SHOP_EDIT_BUY_PAYMENTTYPE);
	disable(IDC_SHOP_EDIT_BUY_ACCOUNT_STR);
	disable(IDC_SHOP_EDIT_BUY_IP);
	disable(IDC_SHOP_EDIT_BUY_REFKEY);
	disable(IDC_SHOP_EDIT_BUY_CASHTYPECODE);
	disable(IDC_SHOP_EDIT_GIFT_PAYMENTTYPE);
	disable(IDC_SHOP_EDIT_GIFT_SENDER_STR);
	disable(IDC_SHOP_EDIT_GIFT_IP);
	disable(IDC_SHOP_EDIT_GIFT_REFKEY);
	disable(IDC_SHOP_EDIT_GIFT_RECEIVER_STR);
	disable(IDC_SHOP_EDIT_GIFT_CASHTYPECODE);
	disable(IDC_SHOP_CHK_BUY_MILEAGE);
	disable(IDC_SHOP_CHK_GIFT_MILEAGE);

	setGame(MU_KR);
}


void CShopClientDlgUIHelper::setMU_GB()
{
	initControls();
	disable(IDC_SHOP_EDIT_INQUIRE_PAYMENTTYPE);
	disable(IDC_SHOP_EDIT_INQUIRE_ACCOUNT_STR);
	disable(IDC_SHOP_EDIT_BUY_PAYMENTTYPE);
	disable(IDC_SHOP_EDIT_BUY_ACCOUNT_STR);
	disable(IDC_SHOP_EDIT_BUY_IP);
	disable(IDC_SHOP_EDIT_BUY_REFKEY);
	disable(IDC_SHOP_EDIT_GIFT_PAYMENTTYPE);
	disable(IDC_SHOP_EDIT_GIFT_SENDER_STR);
	disable(IDC_SHOP_EDIT_GIFT_IP);
	disable(IDC_SHOP_EDIT_GIFT_REFKEY);
	disable(IDC_SHOP_EDIT_GIFT_RECEIVER_STR);
	//disable(IDC_SHOP_EDIT_BUY_ACCESSIP);
	//disable(IDC_SHOP_EDIT_GIFT_ACCESSIP);
	disable(IDC_SHOP_EDIT_GIFTCASH_ACCESSIP);

	setGame(MU_GB);
}

void CShopClientDlgUIHelper::setMU_JP()
{
	initControls();
	disable(IDC_SHOP_EDIT_BUY_CASHTYPECODE);
	disable(IDC_SHOP_EDIT_GIFT_CASHTYPECODE);
	disable(IDC_SHOP_EDIT_BUY_REFKEY);
	disable(IDC_SHOP_EDIT_GIFT_REFKEY);
	disable(IDC_SHOP_EDIT_GIFT_RECEIVER_STR);
	disable(IDC_SHOP_EDIT_BUY_ACCESSIP);
	disable(IDC_SHOP_EDIT_GIFT_ACCESSIP);
	disable(IDC_SHOP_EDIT_GIFTCASH_ACCESSIP);

	setGame(MU_JP);	
}


void CShopClientDlgUIHelper::setMU_VN()
{
	initControls();
	disable(IDC_SHOP_EDIT_BUY_CASHTYPECODE);
	disable(IDC_SHOP_EDIT_GIFT_CASHTYPECODE);
	disable(IDC_SHOP_EDIT_BUY_REFKEY);
	disable(IDC_SHOP_EDIT_GIFT_REFKEY);
	disable(IDC_SHOP_EDIT_GIFT_RECEIVER_STR);	
	disable(IDC_SHOP_EDIT_BUY_ACCESSIP);
	disable(IDC_SHOP_EDIT_GIFT_ACCESSIP);
	disable(IDC_SHOP_EDIT_GIFTCASH_ACCESSIP);	
	disable(IDC_SHOP_EDIT_INQUIRE_ACCOUNT_STR);
	disable(IDC_SHOP_EDIT_BUY_ACCOUNT_STR);
	disable(IDC_SHOP_EDIT_GIFT_SENDER_STR);

	setGame(MU_VN);
}

void CShopClientDlgUIHelper::setMU_TW()
{
	initControls();
	disable(IDC_SHOP_EDIT_BUY_CASHTYPECODE);
	disable(IDC_SHOP_EDIT_GIFT_CASHTYPECODE);
	disable(IDC_SHOP_EDIT_BUY_REFKEY);
	disable(IDC_SHOP_EDIT_GIFT_REFKEY);
	disable(IDC_SHOP_EDIT_GIFT_RECEIVER_STR);
	disable(IDC_SHOP_EDIT_BUY_ACCESSIP);
	disable(IDC_SHOP_EDIT_GIFT_ACCESSIP);
	disable(IDC_SHOP_EDIT_GIFTCASH_ACCESSIP);
	disable(IDC_SHOP_EDIT_INQUIRE_ACCOUNT_STR);
	disable(IDC_SHOP_EDIT_BUY_ACCOUNT_STR);
	disable(IDC_SHOP_EDIT_GIFT_SENDER_STR);

	setGame(MU_TW);
}

void CShopClientDlgUIHelper::setMU_TH()
{
	initControls();
	disable(IDC_SHOP_EDIT_BUY_CASHTYPECODE);
	disable(IDC_SHOP_EDIT_GIFT_CASHTYPECODE);
	disable(IDC_SHOP_EDIT_BUY_REFKEY);
	disable(IDC_SHOP_EDIT_GIFT_REFKEY);
	disable(IDC_SHOP_EDIT_BUY_ACCESSIP);
	disable(IDC_SHOP_EDIT_GIFT_ACCESSIP);
	disable(IDC_SHOP_EDIT_GIFTCASH_ACCESSIP);

	setGame(MU_TH);
}

void CShopClientDlgUIHelper::setBT_KR()
{
	initControls();
	disable(IDC_SHOP_EDIT_INQUIRE_PAYMENTTYPE);
	disable(IDC_SHOP_EDIT_INQUIRE_ACCOUNT_STR);
	disable(IDC_SHOP_EDIT_BUY_PAYMENTTYPE);
	disable(IDC_SHOP_EDIT_BUY_ACCOUNT_STR);
	disable(IDC_SHOP_EDIT_BUY_IP);
	disable(IDC_SHOP_EDIT_BUY_REFKEY);
	disable(IDC_SHOP_EDIT_BUY_CASHTYPECODE);
	disable(IDC_SHOP_EDIT_GIFT_PAYMENTTYPE);
	disable(IDC_SHOP_EDIT_GIFT_SENDER_STR);
	disable(IDC_SHOP_EDIT_GIFT_IP);
	disable(IDC_SHOP_EDIT_GIFT_REFKEY);
	disable(IDC_SHOP_EDIT_GIFT_RECEIVER_STR);
	disable(IDC_SHOP_EDIT_GIFT_CASHTYPECODE);
	disable(IDC_SHOP_CHK_BUY_MILEAGE);
	disable(IDC_SHOP_CHK_GIFT_MILEAGE);

	setGame(BT_KR);
}

void CShopClientDlgUIHelper::setBT_CNL()
{
	initControls();
	disable(IDC_SHOP_EDIT_BUY_CASHTYPECODE);
	disable(IDC_SHOP_EDIT_GIFT_CASHTYPECODE);

	setGame(BT_CNL);
}

void CShopClientDlgUIHelper::setR2_KR()
{
	initControls();
	disable(IDC_SHOP_EDIT_INQUIRE_PAYMENTTYPE);
	disable(IDC_SHOP_EDIT_INQUIRE_ACCOUNT_STR);
	disable(IDC_SHOP_EDIT_BUY_PAYMENTTYPE);
	disable(IDC_SHOP_EDIT_BUY_ACCOUNT_STR);
	disable(IDC_SHOP_EDIT_BUY_IP);
	disable(IDC_SHOP_EDIT_BUY_REFKEY);
	disable(IDC_SHOP_EDIT_BUY_CASHTYPECODE);
	disable(IDC_SHOP_EDIT_GIFT_PAYMENTTYPE);
	disable(IDC_SHOP_EDIT_GIFT_SENDER_STR);
	disable(IDC_SHOP_EDIT_GIFT_IP);
	disable(IDC_SHOP_EDIT_GIFT_REFKEY);
	disable(IDC_SHOP_EDIT_GIFT_RECEIVER_STR);
	disable(IDC_SHOP_EDIT_GIFT_CASHTYPECODE);
	disable(IDC_SHOP_CHK_BUY_MILEAGE);
	disable(IDC_SHOP_CHK_GIFT_MILEAGE);

	setGame(R2_KR);
}

void CShopClientDlgUIHelper::setSUN_KR()
{
	initControls();
	disable(IDC_SHOP_EDIT_INQUIRE_PAYMENTTYPE);
	disable(IDC_SHOP_EDIT_INQUIRE_ACCOUNT_STR);
	disable(IDC_SHOP_EDIT_BUY_PAYMENTTYPE);
	disable(IDC_SHOP_EDIT_BUY_ACCOUNT_STR);
	disable(IDC_SHOP_EDIT_BUY_IP);
	disable(IDC_SHOP_EDIT_BUY_CASHTYPECODE);
	disable(IDC_SHOP_EDIT_BUY_REFKEY);
	disable(IDC_SHOP_CHK_BUY_MILEAGE);
	disable(IDC_SHOP_EDIT_GIFT_PAYMENTTYPE);
	disable(IDC_SHOP_EDIT_GIFT_SENDER_STR);
	disable(IDC_SHOP_EDIT_GIFT_IP);
	disable(IDC_SHOP_EDIT_GIFT_CASHTYPECODE);
	disable(IDC_SHOP_EDIT_GIFT_REFKEY);
	disable(IDC_SHOP_CHK_GIFT_MILEAGE);

	setGame(SUN_KR);
}

void CShopClientDlgUIHelper::setSUN_JP()
{
	initControls();
	disable(IDC_SHOP_EDIT_BUY_ACCESSIP);
	disable(IDC_SHOP_EDIT_BUY_CASHTYPECODE);
	disable(IDC_SHOP_EDIT_BUY_REFKEY);
	disable(IDC_SHOP_CHK_BUY_MILEAGE);
	disable(IDC_SHOP_EDIT_GIFT_ACCESSIP);
	disable(IDC_SHOP_EDIT_GIFT_CASHTYPECODE);
	disable(IDC_SHOP_EDIT_GIFT_REFKEY);
	disable(IDC_SHOP_CHK_GIFT_MILEAGE);

	setGame(SUN_JP);
}

void CShopClientDlgUIHelper::setSUN_GB()
{
	initControls();
	disable(IDC_SHOP_EDIT_INQUIRE_PAYMENTTYPE);
	disable(IDC_SHOP_EDIT_INQUIRE_ACCOUNT_STR);
	disable(IDC_SHOP_EDIT_BUY_PAYMENTTYPE);
	disable(IDC_SHOP_EDIT_BUY_ACCOUNT_STR);
	disable(IDC_SHOP_EDIT_BUY_IP);
	disable(IDC_SHOP_EDIT_BUY_REFKEY);
	disable(IDC_SHOP_EDIT_GIFT_PAYMENTTYPE);
	disable(IDC_SHOP_EDIT_GIFT_SENDER_STR);
	disable(IDC_SHOP_EDIT_GIFT_IP);
	disable(IDC_SHOP_EDIT_GIFT_REFKEY);
	disable(IDC_SHOP_EDIT_GIFT_RECEIVER_STR);
	//disable(IDC_SHOP_EDIT_BUY_ACCESSIP);
	//disable(IDC_SHOP_EDIT_GIFT_ACCESSIP);
	disable(IDC_SHOP_EDIT_GIFTCASH_ACCESSIP);

	setGame(SUN_GB);
}

void CShopClientDlgUIHelper::setBT_TH()
{
	initControls();
	disable(IDC_SHOP_EDIT_BUY_ACCESSIP);
	disable(IDC_SHOP_EDIT_BUY_CASHTYPECODE);
	disable(IDC_SHOP_EDIT_BUY_REFKEY);
	disable(IDC_SHOP_CHK_BUY_MILEAGE);
	disable(IDC_SHOP_EDIT_GIFT_ACCESSIP);
	disable(IDC_SHOP_EDIT_GIFT_CASHTYPECODE);
	disable(IDC_SHOP_EDIT_GIFT_REFKEY);
	disable(IDC_SHOP_CHK_GIFT_MILEAGE);

	setGame(BT_TH);
}

void CShopClientDlgUIHelper::setSUN_RU()
{
	initControls();
	disable(IDC_SHOP_EDIT_BUY_ACCESSIP);
	disable(IDC_SHOP_EDIT_BUY_CASHTYPECODE);
	disable(IDC_SHOP_EDIT_BUY_REFKEY);
	disable(IDC_SHOP_CHK_BUY_MILEAGE);
	disable(IDC_SHOP_EDIT_GIFT_ACCESSIP);
	disable(IDC_SHOP_EDIT_GIFT_CASHTYPECODE);
	disable(IDC_SHOP_EDIT_GIFT_REFKEY);
	disable(IDC_SHOP_CHK_GIFT_MILEAGE);

	setGame(SUN_RU);
}

void CShopClientDlgUIHelper::setSUN_CN()
{
	initControls();
	disable(IDC_SHOP_EDIT_BUY_ACCESSIP);
	disable(IDC_SHOP_EDIT_BUY_CASHTYPECODE);
	disable(IDC_SHOP_EDIT_BUY_REFKEY);
	disable(IDC_SHOP_CHK_BUY_MILEAGE);
	disable(IDC_SHOP_EDIT_GIFT_ACCESSIP);
	disable(IDC_SHOP_EDIT_GIFT_CASHTYPECODE);
	disable(IDC_SHOP_EDIT_GIFT_REFKEY);
	disable(IDC_SHOP_CHK_GIFT_MILEAGE);

	// 캐시조회
	disable(IDC_SHOP_EDIT_INQUIRE_ACCOUNT_STR);	//Account ID/USN
	disable(IDC_SHOP_EDIT_INQUIRE_MILEAGE);		//MileageSection
	disable(IDC_SHOP_CHK_SUM);					//only sum
	disable(IDC_SHOP_EDIT_INQUIRE_MEMBERNO);	//MemberNO
	disable(IDC_SHOP_EDIT_INQUIRE_TYPE);		//ViewType

	// 상품구매
	disable(IDC_SHOP_CHK_BUY_MILEAGE);			//is mileage deduct prod?
	disable(IDC_SHOP_EDIT_BUY_ACCOUNT_STR);		//Account ID/USN
	disable(IDC_SHOP_EDIT_BUY_MEMBERNO);		//MemberNO
	disable(IDC_SHOP_EDIT_BUY_REFKEY);			//RefKey
	disable(IDC_SHOP_EDIT_BUY_ACCESSIP);		//AccessIP

	// 상품선물
	disable(IDC_SHOP_CHK_GIFT_MILEAGE);			//is mileage deduct prod?
	disable(IDC_SHOP_EDIT_GIFT_MEMBERNO);		//MemberNO
	disable(IDC_SHOP_EDIT_GIFT_REFKEY);			//RefKey
	disable(IDC_SHOP_EDIT_GIFT_ACCESSIP);		//AccessIP

	setGame(SUN_CN);
}


void CShopClientDlgUIHelper::setGameData(
	long gamecode, long port, long payment_type, long sales_zone, long banner_zone, long server_type, CString& ip)
{
	// port
	CString port_str;
	port_str.Format(_T("%ld"), port);
	setText(IDC_SHOP_EDIT_PORT, port_str);

	// gamecode
	CString gamecode_str;
	gamecode_str.Format(_T("%ld"), gamecode);

	setText(IDC_SHOP_EDIT_INQUIRE_GAMECODE, gamecode_str);
	setText(IDC_SHOP_EDIT_BUY_GAMECODE, gamecode_str);
	setText(IDC_SHOP_EDIT_GIFT_GAMECODE, gamecode_str);
	setText(IDC_SHOP_EDIT_INQUIRE_SHOPVERSION_GAMECODE, gamecode_str);
	setText(IDC_SHOP_EDIT_GIFTCASH_GAMECODE, gamecode_str);
	setText(IDC_SHOP_EDIT_INQUIRE_STORAGE_GAMECODE, gamecode_str);
	setText(IDC_SHOP_EDIT_INQUIRE_STORAGEPAGE_GAMECODE, gamecode_str);
	setText(IDC_SHOP_EDIT_INQUIRE_STORAGE_GAMECODE_EX, gamecode_str);
	setText(IDC_SHOP_EDIT_INQUIRE_STORAGEPAGE_GAMECODE_EX, gamecode_str);
	setText(IDC_SHOP_EDIT_INQUIRE_STORAGEUSE_GAMECODE, gamecode_str);
	setText(IDC_SHOP_EDIT_INQUIRE_STORAGEDEL_GAMECODE, gamecode_str);
	setText(IDC_SHOP_EDIT_STORAGEUSE_ROLLBACK_GAMECODE, gamecode_str);
	setText(IDC_SHOP_EDIT_INQUIRE_POINT_PERCENTAGE_GAMECODE, gamecode_str);
	setText(IDC_SHOP_EDIT_MILEAGE_GAMECODE, gamecode_str);
	setText(IDC_SHOP_EDIT_MILEAGE_DUDECT_GAMECODE, gamecode_str);
	setText(IDC_SHOP_EDIT_MILEAGELIVE_GAMECODE, gamecode_str);
	setText(IDC_SHOP_EDIT_ITEMSERIAL_GAMECODE, gamecode_str);
	setText(IDC_SHOP_EDIT_INQUIRE_EVENTPRODUCTLIST_GAMECODE, gamecode_str);
	setText(IDC_SHOP_EDIT_INQUIRE_CHECK_GAMECODE, gamecode_str);
	setText(IDC_SHOP_EDIT_INQUIRE_PRODUCTLEFTCOUNT_GAMECODE, gamecode_str);

	// 결제 종류
	CString payment_type_str;
	payment_type_str.Format(_T("%ld"), payment_type);

	setText(IDC_SHOP_EDIT_INQUIRE_PAYMENTTYPE, payment_type_str);
	setText(IDC_SHOP_EDIT_BUY_PAYMENTTYPE, payment_type_str);
	setText(IDC_SHOP_EDIT_GIFT_PAYMENTTYPE, payment_type_str);

	// 판매 영역
	CString sales_zone_str;
	sales_zone_str.Format(_T("%ld"), sales_zone);

	setText(IDC_SHOP_EDIT_BUY_ZONE, sales_zone_str);	
	setText(IDC_SHOP_EDIT_GIFT_ZONE, sales_zone_str);
	setText(IDC_SHOP_EDIT_INQUIRE_SHOPVERSION_SALESZONE, sales_zone_str);
	setText(IDC_SHOP_EDIT_INQUIRE_STORAGE_SalesZone, sales_zone_str);
	setText(IDC_SHOP_EDIT_INQUIRE_STORAGEPAGE_SalesZone, sales_zone_str);
	setText(IDC_SHOP_EDIT_INQUIRE_STORAGE_SalesZone_EX, sales_zone_str);
	setText(IDC_SHOP_EDIT_INQUIRE_STORAGEPAGE_SalesZone_EX, sales_zone_str);
	setText(IDC_SHOP_EDIT_INQUIRE_PRODUCTLIST_SalesZone, sales_zone_str);

	// 배너 영역
	CString banner_zone_str;
	banner_zone_str.Format(_T("%ld"), banner_zone);
	setText(IDC_SHOP_EDIT_INQUIRE_BANNERVERSION_BANNERZONE, banner_zone_str);

	// 서버 유형
	CString server_type_str;
	server_type_str.Format(_T("%ld"), server_type);
	setText(IDC_SHOP_EDIT_INQUIRE_POINT_PERCENTAGE_SERVERCODE, server_type_str);

	// IP
	setText(IDC_SHOP_EDIT_BUY_IP, ip);
	setText(IDC_SHOP_EDIT_GIFT_IP, ip);
	setText(IDC_SHOP_EDIT_BUY_ACCESSIP, ip);
	setText(IDC_SHOP_EDIT_GIFT_ACCESSIP, ip);

	dlg_->UpdateData(FALSE);
}


void CShopClientDlgUIHelper::setGame(long gamecode)
{
	dlg_->m_dwGame = gamecode;

	switch(gamecode)
	{
	case MU_KR	:	setGameData(gamecode, 45613, ShopProtocol::Inbound					, 327, 604,   0, CString(_T("192.168.0.11")));	break;
	case MU_GB	:	setGameData(gamecode, 45615, ShopProtocol::Inbound					, 591, 583,   0, CString(_T("192.168.0.12")));	break;
	case MU_JP	:	setGameData(gamecode, 45616, ShopProtocol::Inbound					,   0,   0,   0, CString(_T("192.168.0.13")));	break;
	case MU_VN	:	setGameData(gamecode, 45632, ShopProtocol::Outbound_FPTOnline		, 707, 708,   0, CString(_T("192.168.0.14")));	break;
	case MU_TW	:	setGameData(gamecode, 45642, ShopProtocol::Outbound_WebzenTW		, 834, 835, 832, CString(_T("192.168.0.15")));	break;
	case MU_TH	:	setGameData(gamecode, 45644, ShopProtocol::Outbound_WinneronlineTH	, 840, 841, 838, CString(_T("192.168.0.16")));	break;
	case BT_KR	:	setGameData(gamecode, 45623, ShopProtocol::Inbound					, 706,   0,   0, CString(_T("192.168.0.17")));	break;
	case BT_CNL	:	setGameData(gamecode, 45623, ShopProtocol::Outbound_Channeling		, 813,   0,   0, CString(_T("192.168.0.18")));	break;
	case R2_KR	:	setGameData(gamecode, 45625, ShopProtocol::Inbound					, 729,   0,   0, CString(_T("192.168.0.19")));	break;
	case SUN_KR	:	setGameData(gamecode, 45621, ShopProtocol::Inbound					, 329, 853, 696, CString(_T("192.168.0.20")));	break;
	case SUN_JP	:	setGameData(gamecode, 45651, ShopProtocol::Outbound_SUN_JP			, 847,   0, 846, CString(_T("192.168.0.21")));	break;
	case SUN_GB	:	setGameData(gamecode, 45617, ShopProtocol::Inbound					, 505,   0, 687, CString(_T("192.168.0.22")));	break;
	case BT_TH	:	setGameData(gamecode, 45629, ShopProtocol::Outbound_BT_TH			, 854,   0, 851, CString(_T("192.168.0.23")));	break;
	case SUN_RU	:	setGameData(gamecode, 45653, ShopProtocol::Outbound_SUN_RU			, 853,   0, 852, CString(_T("192.168.0.24")));	break;
	case SUN_CN	:	setGameData(gamecode, 45655, ShopProtocol::Outbound_SUN_CN			, 860, 861,   0, CString(_T("192.168.0.25")));	break;
	default: break;
	}
}



void CShopClientDlgUIHelper::changeComboGameType()
{
	switch(dlg_->m_GameType.GetCurSel())
	{
	case 0:		setMU_KR();			break;
	case 1:		setMU_GB();			break;
	case 2:		setMU_JP();			break;
	case 3:		setMU_VN();			break;
	case 4:		setMU_TW();			break;
	case 5:		setMU_TH();			break;
	case 6:		setBT_KR();			break;
	case 7:		setBT_CNL();		break;
	case 8:		setR2_KR();			break;
	case 9:		setSUN_KR();		break;
	case 10:	setSUN_JP();		break;
	case 11:	setSUN_GB();		break;
	case 12:	setBT_TH();			break;
	case 13:	setSUN_RU();		break;
	case 14:	setSUN_CN();		break;
	default:	AfxMessageBox(_T("not defined game type")); break;	
	}
}
