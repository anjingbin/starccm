object Form2: TForm2
  Left = 302
  Top = 186
  Width = 591
  Height = 552
  BorderIcons = []
  Caption = #35746#21333#27983#35272
  Color = clSkyBlue
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDefault
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object StringGrid1: TStringGrid
    Left = 48
    Top = 296
    Width = 502
    Height = 165
    Align = alCustom
    Color = clBackground
    ColCount = 6
    FixedColor = clTeal
    FixedCols = 0
    RowCount = 6
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clYellow
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRowSizing, goColSizing, goRowSelect]
    ParentFont = False
    ParentShowHint = False
    ShowHint = False
    TabOrder = 0
    ColWidths = (
      64
      83
      69
      98
      94
      84)
  end
  object Panel1: TPanel
    Left = 49
    Top = 256
    Width = 153
    Height = 33
    Align = alCustom
    Color = clBackground
    TabOrder = 1
    object Edit1: TEdit
      Left = 9
      Top = 3
      Width = 135
      Height = 28
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = True
      ParentFont = False
      TabOrder = 0
      Text = '  '#35746'   '#21333'    '#32454'    '#33410
    end
  end
  object btn_ok: TBitBtn
    Left = 272
    Top = 480
    Width = 75
    Height = 25
    Caption = #36820#22238
    TabOrder = 2
    OnClick = btn_okClick
  end
  object StringGrid2: TStringGrid
    Left = 48
    Top = 56
    Width = 499
    Height = 169
    Align = alCustom
    BiDiMode = bdLeftToRight
    Color = clBackground
    FixedColor = clTeal
    FixedCols = 0
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clYellow
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRowSizing, goColSizing, goRowSelect]
    ParentBiDiMode = False
    ParentFont = False
    TabOrder = 3
    OnMouseDown = StringGrid2MouseDown
    ColWidths = (
      64
      77
      161
      82
      77)
  end
  object Panel2: TPanel
    Left = 144
    Top = 0
    Width = 289
    Height = 49
    Color = clBackground
    TabOrder = 4
    object Edit2: TEdit
      Left = 15
      Top = 7
      Width = 262
      Height = 35
      Font.Charset = GB2312_CHARSET
      Font.Color = clMaroon
      Font.Height = -27
      Font.Name = #38582#20070
      Font.Style = [fsBold]
      ParentColor = True
      ParentFont = False
      TabOrder = 0
      Text = #35746'   '#21333'   '#27983'   '#35272' '
    end
  end
end
