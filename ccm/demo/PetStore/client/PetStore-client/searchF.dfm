object formSearch: TformSearch
  Left = 352
  Top = 218
  Width = 376
  Height = 271
  Caption = #25628#32034
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object radioGroup: TRadioGroup
    Left = 24
    Top = 8
    Width = 321
    Height = 145
    Caption = #25628#32034#31867#22411
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    Items.Strings = (
      '  BY   '#31867#21035
      '  BY    ID'
      '  BY   ALL')
    ParentFont = False
    TabOrder = 0
  end
  object editSearch: TEdit
    Left = 24
    Top = 176
    Width = 321
    Height = 21
    TabOrder = 1
  end
  object buttonOk: TButton
    Left = 104
    Top = 208
    Width = 49
    Height = 25
    Caption = #30830#23450
    TabOrder = 2
    OnClick = buttonOkClick
  end
  object buttonCancel: TButton
    Left = 184
    Top = 208
    Width = 49
    Height = 25
    Caption = #21462#28040
    TabOrder = 3
    OnClick = buttonCancelClick
  end
end
