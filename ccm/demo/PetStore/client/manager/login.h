//----------------------------------------------------------------------------
#ifndef loginH
#define loginH
//----------------------------------------------------------------------------
#include <vcl\Buttons.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\Classes.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Windows.hpp>
#include <vcl\System.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
//----------------------------------------------------------------------------
class TPasswordDlg : public TForm
{
__published:
	TEdit *Password;
	TButton *OKBtn;
	TButton *CancelBtn;
        TEdit *UserName;
        TLabel *Label1;
        TLabel *Label2;
        void __fastcall CancelBtnClick(TObject *Sender);
        void __fastcall OKBtnClick(TObject *Sender);
private:
public:
	virtual __fastcall TPasswordDlg(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TPasswordDlg *PasswordDlg;
//----------------------------------------------------------------------------
#endif    
