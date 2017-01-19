//---------------------------------------------------------------------------

#ifndef cartFH
#define cartFH
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
class TformCart : public TForm
{
__published:	// IDE-managed Components
        TSplitter *topSplitter;
        TPanel *panelTop;
        TImage *imageBackGround;
        TSpeedButton *speedButtonView;
        TSpeedButton *speedButtonExit;
        TImage *imageHome;
        TImage *imageCart;
        TLabel *petLabel;
        TImage *imageDog;
        TImage *imageCat;
        TImage *imageFish;
        TImage *imageBird;
        TEdit *editSearchContext;
        TComboBox *comboBoxKind;
        TButton *buttonSearch;
        TStringGrid *stringGridIntroduction;
        TPanel *panelBottom;
        TLabel *labelTotal;
        TLabel *labelTotalPrice;
        TLabel *labelRMB;
        TImage *imageNext;
        TImage *imageFirst;
        TButton *buttonClear;
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall speedButtonExitClick(TObject *Sender);
        void __fastcall imageHomeClick(TObject *Sender);
        void __fastcall imageNextClick(TObject *Sender);
        void __fastcall imageFirstClick(TObject *Sender);
        void __fastcall imageDogClick(TObject *Sender);
        void __fastcall imageCatClick(TObject *Sender);
        void __fastcall imageFishClick(TObject *Sender);
        void __fastcall imageBirdClick(TObject *Sender);
        void __fastcall buttonSearchClick(TObject *Sender);
        void __fastcall imageCartClick(TObject *Sender);
        void __fastcall buttonClearClick(TObject *Sender);
        void __fastcall stringGridIntroductionMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall stringGridIntroductionClick(TObject *Sender);
        void __fastcall stringGridIntroductionKeyPress(TObject *Sender,
          char &Key);
    void __fastcall speedButtonViewClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TformCart(TComponent* Owner);
        
        enum PetStore::ProductCategory productC_;
        char* searchContext_;
        char* kindText_;
        int YCol,XRow;
        void __fastcall TformCart::setTotalPrice();
        void __fastcall TformCart::showProduction();

};
//---------------------------------------------------------------------------
extern PACKAGE TformCart *formCart;
//---------------------------------------------------------------------------
#endif
