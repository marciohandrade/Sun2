#pragma once

enum eOptionItemType
{
    OPTION_ITEM_TYPE_CATEGORY,
    OPTION_ITEM_TYPE_ITEM,
    OPTION_ITEM_TYPE_GAUGE,
};


/////////////////////////////////////////////////////////////////////
class cOptionItemBase
{
protected:
    cOptionItemBase() {}

public:
	virtual ~cOptionItemBase() {}

public:
    virtual eOptionItemType GetType() PURE;

    virtual void Render( const RECT& DestRect ) PURE;
};


//////////////////////////////////////////////////////////////////////////////////////////
class cOptionItem
    : public cOptionItemBase
{
private:
    std::string m_Title;
    DWORD m_OptionID;
    nOptionType::eOptionTypeIndex m_OptionTypeIndex;
    std::string m_Description;
	DWORD	m_FontID;
	WzColor	m_FontColor;

protected:
    cOptionItem() {}

public:
	virtual ~cOptionItem() {}

public:
    void SetTitleText( const std::string& ItemName ) { m_Title = ItemName; }
    void SetOptionID( DWORD OptionID ) { m_OptionID = OptionID; }
    void SetOptionTypeIndex( nOptionType::eOptionTypeIndex TypeIndex ) { m_OptionTypeIndex = TypeIndex; }
    void SetDescriptionText( const std::string& DescriptionText ) { m_Description = DescriptionText; }
	void SetFontID(DWORD ID) { m_FontID = ID; }
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
	void SetFontColor(const DWORD& Color) { m_FontColor = Color; }
#else
	void SetFontColor(DWORD Color) { m_FontColor; } 
#endif // _NA_008678_20160203_ADD_HEIM_NOTIFICATION_OPTION
  
    const std::string& GetTitleText() const { return m_Title; }
    int GetOptionID() { return m_OptionID; }
    nOptionType::eOptionTypeIndex GetOptionTypeIndex() { return m_OptionTypeIndex; }
    const std::string& GetDescriptionText() const { return m_Description; }

    virtual void SetItemValue( int Value ) PURE;
    virtual int GetItemValue() PURE;
    virtual void SetNext() PURE;
    virtual void SetPrev() PURE;
};


//////////////////////////////////////////////////////////////////////////////////////////
class cOptionComboItem
    : public cOptionItem
{
private:
    eOptionItemType GetType() { return OPTION_ITEM_TYPE_ITEM; }

    std::vector<std::string> m_ItemArray;
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
	std::vector<DWORD>       font_color_array_;
#endif // _NA_008678_20160203_ADD_HEIM_NOTIFICATION_OPTION
    size_t m_ItemIndex;

public:
    cOptionComboItem();

public:
	virtual ~cOptionComboItem() {}

    void AddItemText( const std::string& ItemText ) { m_ItemArray.push_back( ItemText ); }
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
	void AddItemTextColor( const DWORD& font_color ) { font_color_array_.push_back( font_color ); }
	DWORD GetItemTextColor(const size_t& index) const 
	{
		if (index > font_color_array_.size() - 1 || 
			index > m_ItemArray.size() - 1 || 
			font_color_array_.empty())
		{
			return 0xAAFFFFFF;
		}

		return font_color_array_[index];
	}
#endif // _NA_008678_20160203_ADD_HEIM_NOTIFICATION_OPTION
    const std::string& GetItemText(size_t index) { return m_ItemArray[index]; }
    size_t GetItemCount() { return m_ItemArray.size(); } 

    void SetItemIndex( size_t index ) { m_ItemIndex = index; }
    size_t GetItemIndex() { return m_ItemIndex; }

    void SetNext();
    void SetPrev();
    void SetItemValue( int Value ) { SetItemIndex( Value ); }
    int GetItemValue() { return GetItemIndex(); }

private:
    void Render( const RECT& DestRect );
};

//////////////////////////////////////////////////////////////////////////////////////////
class cOptionGaugeItem
    : public cOptionItem
{
public: 
	enum
	{
		MOVE_NONE = 0,
		MOVE_LEFT,
		MOVE_RIGHT,
		MOVE_MAX,
	};
private:
    eOptionItemType GetType() { return OPTION_ITEM_TYPE_GAUGE; }

    DWORD m_Range;
    DWORD m_Position;
	bool  m_bClick;
	int   m_iMovePosition;

	TCHAR	m_strItemPercentGauge[INTERFACE_STRING_LENGTH];

public:
    cOptionGaugeItem();
	virtual ~cOptionGaugeItem() {}

    DWORD GetGaugeRange() { return m_Range; }
    DWORD GetGaugePosition() { return m_Position; }
    void SetGaugeRange( DWORD Range ) { m_Range = Range; }
    void SetGaugePosition( DWORD Position ) { m_Position = Position > m_Range ? m_Range : Position; }

    void SetNext();
    void SetPrev();

	//gauge 컨트롤에서만 사용됨
	void SetClickMover(bool set , int Move) { m_bClick = set; m_iMovePosition = Move;}
	bool GetClickMover(int* Mover) { (*Mover) = m_iMovePosition ; return m_bClick; }

    void SetItemValue( int Value ) { SetGaugePosition( Value ); }
    int GetItemValue() { return GetGaugePosition(); }

    void Render( const RECT& DestRect );
};

//////////////////////////////////////////////////////////////////////////////////////////
class cOptionCategory
    : public cOptionItemBase
{
public:
    cOptionCategory( const std::string& Title );
	virtual ~cOptionCategory();

    const std::string& GetTitle() { return m_Title; }
    void AddItem( cOptionItem* pItem ) { m_OptionCategory.push_back( pItem ); }
    cOptionItem* GetItem( size_t index ) { return m_OptionCategory[index]; }
    size_t GetItemCount() { return m_OptionCategory.size(); }

    bool IsExpended() { return m_Expended; }
    void SetExpend( bool IsExpend ) { m_Expended = IsExpend; }

private:
    eOptionItemType GetType() { return OPTION_ITEM_TYPE_CATEGORY; }
    void Render( const RECT& DestRect );

private:
    std::vector<cOptionItem*> m_OptionCategory;

    std::string m_Title;
    bool m_Expended;
};

//////////////////////////////////////////////////////////////////////////////////////////
namespace nOptionFacade
{
    nOptionType::eOptionTypeIndex GetOptionTypeIndexFromOptionID( DWORD OptionID );

    void RestoreOptionValue( cOptionItem* pItem );
    void SetOptionValue( cOptionItem* pItem );
    int GetCurrentOption( DWORD OptionID );

};


