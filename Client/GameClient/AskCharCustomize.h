#pragma once
#include "askdialog.h"

class AskCharCustomize:
    public AskDialog
{
public:
    AskCharCustomize(InterfaceManager *pUIMan);
    virtual ~AskCharCustomize(void);

    WORD customize_type() const { return customize_type_; }
    void set_customize_type(WORD val) { customize_type_ = val; }

    BYTE change_gender() const { return change_gender_; }
    void set_change_gender(BYTE val) { change_gender_ = val; }

    POSTYPE slot_position() const { return slot_position_; }
    void set_slot_position(POSTYPE val) { slot_position_ = val; }

protected:
    virtual void	OnUpdateSolarDialog();
    virtual void    MessageProc( SI_MESSAGE * pMessage );
    virtual VOID	OnShowWindow( BOOL val );

    VOID			SetTexts();
    VOID			DoIt();
    VOID			Cancel();

private:
    WORD customize_type_; 
    BYTE change_gender_;
    POSTYPE slot_position_;
};

