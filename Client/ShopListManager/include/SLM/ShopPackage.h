
/**************************************************************************************************

��Ű�� ��ü

���� ��Ű�� ������ ������ �ִ�.
���� ��Ű���� ��� �Ǿ��ִ� "��ǰ ��ȣ" ��ϰ� "���� ��ȣ" ����� ������ �ִ�.

��Ű���� ���� ��ǰ�� ����ִ� ��쿡�� ���� ��ȣ�� �� �� �̴�.
��Ű���� ���� ������ �����Ǿ� �ִٸ� ��ǰ�� �� �� �̴�.

(exe 1)
A ��Ű���� a�� a' ��� ��ǰ�� �� �� �ִٸ� ���� ������ ���� �� �� ����. 
-> A��ǰ a, a' 5000��

(exe 2)
B ��Ű���� b ��� ��ǰ�� �ϳ��� �ִٸ� 1000��, 2000��, 3000�� ���� ���� ������ ���� �� �� �ִ�. 
-> B��ǰ b 1���� 1000��
-> B��ǰ b 2���� 2000��
-> B��ǰ b 4���� 3000��
�� ������ �ٸ� ��Ű���� �����־�� �Ѵ�.

**************************************************************************************************/

#pragma once
#include <SLM/CommonSLM.h>

class CShopPackage
{
public:
	CShopPackage();
	virtual ~CShopPackage();

	// version 3
	bool	SetPackageVer3(tstring& strData, const tstring& delimiter);
	bool	SetPackageVer3W(std::wstring& strData, const std::wstring& delimiter);

	// version 6
	bool	SetPackageVer6(tstring& strData, const tstring& delimiter);
	bool	SetPackageVer6W(std::wstring& strData, const std::wstring& delimiter);

	// version 3, 6
	void	SetLeftCount(int nCount);

	int		GetProductCount();									// ��Ű�� ������ ��ǰ �� ��������
	void	SetProductSeqFirst();								// ��Ű�� ������ ù��° ��ǰ ��ȣ�� ��ġ
	bool	GetProductSeqFirst(int& ProductSeq);				// ��Ű�� ������ ù��° ��ǰ ��ȣ�� �������� ���� ��ǰ ��ȣ�� �̵�
	bool	GetProductSeqNext(int& ProductSeq);					// ��ǰ ��ȣ �������� ���� ��ġ�� �̵�

	int		GetPriceCount();									// ��Ű�� ������ ���� �� ��������
	void	SetPriceSeqFirst();									// ��Ű�� ������ ù��° ���� ��ȣ�� ��ġ
	bool	GetPriceSeqFirst(int& PriceSeq);					// ��Ű�� ������ ù��° ���� ��ȣ�� �������� ���� ���� ��ȣ�� �̵�
	bool	GetPriceSeqNext(int& PriceSeq);						// ���� ��ȣ �������� ���� ��ġ�� �̵�

public:

	// version 3
	int		ProductDisplaySeq;									//  1. ��Ű���� �����ִ� ī�װ� ��ȣ
	int		ViewOrder;											//  2. ���� ����
	int		PackageProductSeq;									//  3. ��Ű�� ��ȣ
	TCHAR	PackageProductName[SHOPLIST_LENGTH_PACKAGENAME];	//  4. ��Ű�� ��
	WCHAR	PackageProductNameW[SHOPLIST_LENGTH_PACKAGENAME];	//  4. ��Ű�� ��			(UseUnicodeString �� ��� ����)
	int		PackageProductType;									//  5. ��Ű�� ���� (170:�Ϲ� ��ǰ, 171:�̺�Ʈ ��ǰ)
	int		Price;												//  6. ����
	TCHAR	Description[SHOPLIST_LENGTH_PACKAGEDESC];			//  7. �� ����
	WCHAR	DescriptionW[SHOPLIST_LENGTH_PACKAGEDESC];			//  7. �� ����			(UseUnicodeString �� ��� ����)
	TCHAR	Caution[SHOPLIST_LENGTH_PACKAGECAUTION];			//  8. ���� ����
	WCHAR	CautionW[SHOPLIST_LENGTH_PACKAGECAUTION];			//  8. ���� ����			(UseUnicodeString �� ��� ����)
	int		SalesFlag;											//  9. ���� ���� ����(���Ź�ư ���⿩��) (182:����, 183:�Ұ�)
	int		GiftFlag;											// 10. ���� ���� ����(������ư ���⿩��) (184:����, 185:�Ұ�)
	tm		StartDate;											// 11. �Ǹ� ������
	tm		EndDate;											// 12. �Ǹ� ������
	int		CapsuleFlag;										// 13. ĸ�� ��ǰ ���� (176:ĸ��, 177:�Ϲ�)
	int		CapsuleCount;										// 14. ��Ű���� ���Ե� ��ǰ ����
	TCHAR	ProductCashName[SHOPLIST_LENGTH_PACKAGECASHNAME];	// 15. ���� ĳ�� ��
	WCHAR	ProductCashNameW[SHOPLIST_LENGTH_PACKAGECASHNAME];	// 15. ���� ĳ�� ��			(UseUnicodeString �� ��� ����)
	TCHAR	PricUnitName[SHOPLIST_LENGTH_PACKAGEPRICEUNIT];		// 16. ���� ���� ǥ�� ��
	WCHAR	PricUnitNameW[SHOPLIST_LENGTH_PACKAGEPRICEUNIT];	// 16. ���� ���� ǥ�� ��	(UseUnicodeString �� ��� ����)
	int		DeleteFlag;											// 17. ���� ���� (180:����, 181:Ȱ��)
	int		EventFlag;											// 18. �̺�Ʈ ��ǰ ���� (199:�̺�Ʈ ��ǰ, 200:�Ϲ� ��ǰ)
	int		ProductAmount;										// 19. ���� ��ǰ ����	
	TCHAR	InGamePackageID[SHOPLIST_LENGTH_INGAMEPACKAGEID];	// 21. ��Ű�� ������ �ڵ�
	WCHAR	InGamePackageIDW[SHOPLIST_LENGTH_INGAMEPACKAGEID];	// 21. ��Ű�� ������ �ڵ�	(UseUnicodeString �� ��� ����)
	int		ProductCashSeq;										// 22. ���� ĳ�� ���� �ڵ�
	int		PriceCount;											// 23. ���� ��ǰ ���� ��å ���� ���� (���� ��ǰ�� 1���� ��쿡�� PriceSeq�� ���� ���� �� �ִ�.)
	bool	DeductMileageFlag;									// 25. ���ϸ����� ���� ��ǰ ���� (false : �Ϲ�, true : ���ϸ��� ���� ��ǰ)
	int		CashType;											// 26. �۷ι� ���� : Wcoin(C), WCoin(P) ����
	int		CashTypeFlag;										// 27. �۷ι� ���� : Wcoin(C), WCoin(P) ���� or �ڵ� ����(668: ���μ���, 669: �ڵ�����)

	int		LeftCount;											// �ܿ� ����

	// version 6 : version 3�� �ߺ� ��� �ּ�ó��
	//int		PackageProductSeq;								//  1. ��Ű�� ��ȣ
	//TCHAR	PackageProductName[SHOPLIST_LENGTH_PACKAGENAME];	//  2. ��Ű�� ��
	//int		PackageProductType;								//  3. ��Ű�� ���� (170:�Ϲ� ��ǰ, 171:�̺�Ʈ ��ǰ)
	//int		Price;											//  4. ����
	//TCHAR	Description[SHOPLIST_LENGTH_PACKAGEDESC];			//  5. �� ����
	//TCHAR	Caution[SHOPLIST_LENGTH_PACKAGECAUTION];			//  6. ���� ����
	//int		SalesFlag;										//  7. ���� ���� ����(���Ź�ư ���⿩��) (182:����, 183:�Ұ�)
	//int		GiftFlag;										//  8. ���� ���� ����(������ư ���⿩��) (184:����, 185:�Ұ�)
	//tm		StartDate;										//  9. �Ǹ� ������
	//tm		EndDate;										// 10. �Ǹ� ������
	//int		CapsuleFlag;									// 11. ĸ�� ��ǰ ���� (176:ĸ��, 177:�Ϲ�)
	//int		CapsuleCount;									// 12. ��Ű���� ���Ե� ��ǰ ����
	//TCHAR	ProductCashName[SHOPLIST_LENGTH_PACKAGECASHNAME];	// 13. ���� ĳ�� ��
	//TCHAR	PricUnitName[SHOPLIST_LENGTH_PACKAGEPRICEUNIT];		// 14. ���� ���� ǥ�� ��
	//int		DeleteFlag;										// 15. ���� ���� (180:����, 181:Ȱ��)
	//int		ProductAmount;									// 16. ���� ��ǰ ����	
	//TCHAR	InGamePackageID[SHOPLIST_LENGTH_INGAMEPACKAGEID];	// 18. ��Ű�� ������ �ڵ�
	//int		ProductCashSeq;									// 19. ���� ĳ�� ���� �ڵ�
	//int		PriceCount;										// 20. ���� ��ǰ ���� ��å ���� ���� (���� ��ǰ�� 1���� ��쿡�� PriceSeq�� ���� ���� �� �ִ�.)
	//bool	DeductMileageFlag;									// 22. ���ϸ����� ���� ��ǰ ���� (false : �Ϲ�, true : ���ϸ��� ���� ��ǰ)
	//int		CashType;										// 23. �۷ι� ���� : Wcoin(C), WCoin(P) ����
	//int		CashTypeFlag;									// 24. �۷ι� ���� : Wcoin(C), WCoin(P) ���� or �ڵ� ����(668: ���μ���, 669: �ڵ�����)
	int		ImageTagType;										// 25. �̹����±� �ڵ� �� (���� ���� �ڵ� ���� ����)
	//int		LeftCount;		

private:

	// version 3, 6
	void	SetProductSeqList(tstring strdata);
	void	SetProductSeqListW(std::wstring strdata);
	void	SetPriceSeqList(tstring strdata);
	void	SetPriceSeqListW(std::wstring strdata);

	std::vector<int> ProductSeqList;							// 20. ��Ű���� ���Ե� ��ǰ ��ȣ ���
	std::vector<int>::iterator ProductSeqIter;

	std::vector<int> PriceSeqList;								// 24. ��Ű���� ���ԵǴ� ���� ��ȣ ��� (��ǰ�� 1���� ��쿡�� ���� ����.)
	std::vector<int>::iterator PriceSeqIter;
};
