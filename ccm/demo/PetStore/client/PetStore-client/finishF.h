//---------------------------------------------------------------------------

#ifndef finishFH
#define finishFH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Grids.hpp>

#include "pet.h"
//---------------------------------------------------------------------------
class TformFinish : public TForm
{
__published:	// IDE-managed Components
        TStringGrid *stringGridIntroduction;
        TSplitter *topSplitter;
        TPanel *panelTop;
        TImage *imageBackGround;
        TSpeedButton *speedButtonView;
        TSpeedButton *speedButtonExit;
        TImage *imageHome;
        TImage *imageDog;
        TImage *imageCat;
        TImage *imageFish;
        TImage *imageBird;
        TPanel *panelButtom;
        TLabel *Label2;
        TGroupBox *groupBoxUser;
        TLabel *labelNameS;
        TLabel *labelAddressS;
        TSplitter *splitterLeft;
        TLabel *labelName;
        TLabel *labelAddress;
        TLabel *labelOrderNumber;
        TLabel *labelTotalPrice;
        void __fastcall imageHomeClick(TObject *Sender);
        void __fastcall speedButtonExitClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall speedButtonViewClick(TObject *Sender);
        void __fastcall imageDogClick(TObject *Sender);
        void __fastcall imageCatClick(TObject *Sender);
        void __fastcall imageFishClick(TObject *Sender);
        void __fastcall imageBirdClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        enum PetStore::ProductCategory productC_;
        __fastcall TformFinish(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TformFinish *formFinish;
//---------------------------------------------------------------------------
#endif
