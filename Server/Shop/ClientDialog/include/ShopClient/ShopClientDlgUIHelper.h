#pragma once

// ShopClientDlg �� UI�� �׽�Ʈ ������ ������ ���� Ŭ����
// ���ǻ� ����ϹǷ� ���� ��Ʃ������� �� �ʿ� ����


class CShopClientDlg;
class CShopClientDlgUIHelper 
{
public:
	CShopClientDlgUIHelper();
	~CShopClientDlgUIHelper();

public:
	void init(CShopClientDlg* dlg);	// �ʱ�ȭ(�ʼ�)
	void changeComboGameType();		// ���� ���� �޺��ڽ� ���ý� �׼�

	// �޺� �ڽ� ���ý� ���Ӻ� �׽�Ʈ�� ���� �Լ�
	void setMU_KR();
	void setMU_GB();
	void setMU_JP();
	void setMU_VN();
	void setMU_TW();
	void setMU_TH();
	void setBT_KR();
	void setBT_CNL();
	void setR2_KR();
	void setSUN_KR();
	void setSUN_JP();
	void setSUN_GB();
	void setBT_TH();
	void setSUN_RU();
	void setSUN_CN();

private:
	void registResource();				// ShopClient ���̾�α� ���ҽ� ���
	void enable(int id);				// ���ҽ� ��Ʈ�� Ȱ��ȭ
	void disable(int id);				// ���ҽ� ��Ʈ�� ��Ȱ��ȭ
	void setText(int id, CString& str);	// ���ҽ� ��Ʈ�� �� �Է�
	
	void initControls();				// ��� ���ҽ� ��Ʈ�� �ʱ�ȭ	
	void setGame(long gamecode);		// ���Ӻ� �⺻�� ����
	void setGameData(					// �⺻�� ���� �����
		long gamecode, long port, long payment_type, long sales_zone, long banner_zone, long server_type, CString& ip);

private:
	CShopClientDlg*	dlg_;				// ShopClinet ���̾�α� �ν��Ͻ�
	std::vector<int> resource_list_;	// ���ҽ� ��Ʈ�ѵ��� ���
};
