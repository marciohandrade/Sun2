#pragma once

// ShopClientDlg 의 UI나 테스트 데이터 세팅을 위한 클래스
// 편의상 사용하므로 게임 스튜디오에선 볼 필요 없음


class CShopClientDlg;
class CShopClientDlgUIHelper 
{
public:
	CShopClientDlgUIHelper();
	~CShopClientDlgUIHelper();

public:
	void init(CShopClientDlg* dlg);	// 초기화(필수)
	void changeComboGameType();		// 게임 선택 콤보박스 선택시 액션

	// 콤보 박스 선택시 게임별 테스트값 설정 함수
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
	void registResource();				// ShopClient 다이얼로그 리소스 등록
	void enable(int id);				// 리소스 컨트롤 활성화
	void disable(int id);				// 리소스 컨트롤 비활성화
	void setText(int id, CString& str);	// 리소스 컨트롤 값 입력
	
	void initControls();				// 모든 리소스 컨트롤 초기화	
	void setGame(long gamecode);		// 게임별 기본값 세팅
	void setGameData(					// 기본값 세팅 도우미
		long gamecode, long port, long payment_type, long sales_zone, long banner_zone, long server_type, CString& ip);

private:
	CShopClientDlg*	dlg_;				// ShopClinet 다이얼로그 인스턴스
	std::vector<int> resource_list_;	// 리소스 컨트롤들의 목록
};
