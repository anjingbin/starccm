//---------------------------------------------------------------------------

#ifndef passwordFH
#define passwordFH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include <Mask.hpp>
//---------------------------------------------------------------------------
class TformPassword : public TForm
{
__published:	// IDE-managed Components
        TLabel *labelName;
        TImage *imageBackGround;
        TEdit *editName;
        TLabel *labelPassword;
        TMaskEdit *maskEditPassword;
        void __fastcall maskEditPasswordChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TformPassword(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TformPassword *formPassword;
//---------------------------------------------------------------------------
#endif
