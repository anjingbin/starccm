//---------------------------------------------------------------------------

#ifndef searchFH
#define searchFH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TformSearch : public TForm
{
__published:	// IDE-managed Components
        TRadioGroup *radioGroup;
        TEdit *editSearch;
        TButton *buttonOk;
        TButton *buttonCancel;
        void __fastcall buttonOkClick(TObject *Sender);
        void __fastcall buttonCancelClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
        __fastcall TformSearch(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TformSearch *formSearch;
//---------------------------------------------------------------------------
#endif
