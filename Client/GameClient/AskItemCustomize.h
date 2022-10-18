#pragma once
#include "askdialog.h"

class AskItemCustomize:
    public AskDialog
{
public:
    enum
    {
        kExtractShell,
        kExtractEtheria,
        kExtractAll,
    };

    AskItemCustomize(InterfaceManager *pUIMan);
    virtual ~AskItemCustomize(void);

    BYTE customize_type() const { return customize_type_; }
    POSTYPE slot_pos() const { return slot_pos_; }
    POSTYPE use_pos() const { return use_pos_; }

    void set_customize_type(BYTE val, POSTYPE slot_pos, POSTYPE use_pos);

protected:
    virtual void	OnUpdateSolarDialog();
    virtual void    MessageProc( SI_MESSAGE * pMessage );
    virtual VOID	OnShowWindow( BOOL val );

    VOID			SetTexts();
    VOID			DoIt();
    VOID			Cancel();

private:
    BYTE customize_type_; 
    POSTYPE slot_pos_; 
    POSTYPE use_pos_; 
    
};

inline void AskItemCustomize::set_customize_type(BYTE val, 
                                                 POSTYPE slot_pos, 
                                                 POSTYPE use_pos)
{
    customize_type_ = val; 
    slot_pos_ = slot_pos; 
    use_pos_ = use_pos; 
}


