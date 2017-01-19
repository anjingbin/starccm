//---------------------------------------------------------------------------

#include <vcl.h>

#include "pet.h"

#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("viewF.cpp", formView);
USEFORM("finishF.cpp", formFinish);
USEFORM("orderF.cpp", formOrder);
USEFORM("passwordF.cpp", formPassword);
USEFORM("saleF.cpp", formSale);
USEFORM("searchF.cpp", formSearch);
USEFORM("cartF.cpp", formCart);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 if (debug)
                 {
                 getComponents() ;
                 }
                 Application->Initialize();
                 Application->Title = "PetStore-Client";
                 Application->CreateForm(__classid(TformPassword), &formPassword);
                 Application->CreateForm(__classid(TformView), &formView);
                 Application->CreateForm(__classid(TformFinish), &formFinish);
                 Application->CreateForm(__classid(TformOrder), &formOrder);
                 Application->CreateForm(__classid(TformSale), &formSale);
                 Application->CreateForm(__classid(TformSearch), &formSearch);
                 Application->CreateForm(__classid(TformCart), &formCart);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        catch (...)
        {
                 try
                 {
                         throw Exception("");
                 }
                 catch (Exception &exception)
                 {
                         Application->ShowException(&exception);
                 }
        }
        return 0;
}
//---------------------------------------------------------------------------
