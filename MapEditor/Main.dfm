object Form1: TForm1
  Left = 193
  Top = 119
  Width = 808
  Height = 496
  Caption = 'GalaxyNG Map Editor'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnActivate = FormActivate
  OnCreate = FormCreate
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 392
    Width = 3
    Height = 13
  end
  object Label2: TLabel
    Left = 8
    Top = 411
    Width = 3
    Height = 13
  end
  object Image1: TImage
    Left = 16
    Top = 0
    Width = 609
    Height = 385
    OnMouseDown = Image1MouseDown
    OnMouseMove = Image1MouseMove
    OnMouseUp = Image1MouseUp
  end
  object Label8: TLabel
    Left = 632
    Top = 348
    Width = 3
    Height = 13
  end
  object GroupBox1: TGroupBox
    Left = 632
    Top = 160
    Width = 121
    Height = 177
    TabOrder = 0
    object Label3: TLabel
      Left = 8
      Top = 24
      Width = 28
      Height = 13
      Caption = 'Name'
    end
    object Label4: TLabel
      Left = 24
      Top = 40
      Width = 7
      Height = 13
      Caption = 'X'
    end
    object Label5: TLabel
      Left = 24
      Top = 64
      Width = 7
      Height = 13
      Caption = 'Y'
    end
    object Label6: TLabel
      Left = 16
      Top = 91
      Width = 20
      Height = 13
      Caption = 'Size'
    end
    object Label7: TLabel
      Left = 15
      Top = 117
      Width = 19
      Height = 13
      Caption = 'Res'
    end
    object Edit1: TEdit
      Left = 40
      Top = 20
      Width = 73
      Height = 21
      TabOrder = 0
      Text = '0'
      OnExit = Edit1Exit
      OnKeyPress = Edit1KeyPress
    end
    object Edit2: TEdit
      Left = 40
      Top = 40
      Width = 73
      Height = 21
      TabOrder = 1
      Text = '0'
      OnChange = Edit2Change
      OnKeyPress = Edit2KeyPress
    end
    object Edit3: TEdit
      Left = 40
      Top = 64
      Width = 73
      Height = 21
      TabOrder = 2
      Text = '0'
      OnExit = Edit3Exit
      OnKeyPress = Edit3KeyPress
    end
    object Edit4: TEdit
      Left = 40
      Top = 88
      Width = 73
      Height = 21
      TabOrder = 3
      Text = '0'
      OnExit = Edit4Exit
      OnKeyPress = Edit4KeyPress
    end
    object Edit5: TEdit
      Left = 40
      Top = 112
      Width = 73
      Height = 21
      TabOrder = 4
      Text = '0'
      OnExit = Edit5Exit
      OnKeyPress = Edit5KeyPress
    end
    object Button1: TButton
      Left = 8
      Top = 144
      Width = 105
      Height = 25
      Caption = 'Set new coordinates'
      TabOrder = 5
      OnClick = Button1Click
    end
  end
  object Panel1: TPanel
    Left = 632
    Top = 0
    Width = 121
    Height = 153
    TabOrder = 1
    object SpeedButton1: TSpeedButton
      Left = 16
      Top = 8
      Width = 33
      Height = 33
      Caption = '+'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -32
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton1Click
    end
    object SpeedButton2: TSpeedButton
      Left = 64
      Top = 8
      Width = 33
      Height = 33
      Caption = '-'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -40
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton2Click
    end
    object SpeedButton3: TSpeedButton
      Left = 50
      Top = 56
      Width = 23
      Height = 22
      Caption = '^'
      OnClick = SpeedButton3Click
    end
    object SpeedButton4: TSpeedButton
      Left = 26
      Top = 88
      Width = 23
      Height = 22
      Caption = '<'
      OnClick = SpeedButton4Click
    end
    object SpeedButton5: TSpeedButton
      Left = 74
      Top = 88
      Width = 23
      Height = 22
      Caption = '>'
      OnClick = SpeedButton5Click
    end
    object SpeedButton6: TSpeedButton
      Left = 50
      Top = 115
      Width = 23
      Height = 22
      Caption = 'v'
      OnClick = SpeedButton6Click
    end
  end
  object MainMenu1: TMainMenu
    Left = 24
    Top = 32
    object Map1: TMenuItem
      Caption = 'Map'
      object Load1: TMenuItem
        Caption = 'Load...'
        OnClick = Load1Click
      end
      object Save1: TMenuItem
        Caption = 'Save as...'
        OnClick = Save1Click
      end
    end
    object Options1: TMenuItem
      Caption = 'Options'
      object Planets1: TMenuItem
        Caption = 'Planets'
        object ConstantSize1: TMenuItem
          Caption = 'Constant Radius On/Off'
          OnClick = ConstantSize1Click
        end
      end
    end
    object Help1: TMenuItem
      Caption = 'Help'
      object About1: TMenuItem
        Caption = 'About...'
        OnClick = About1Click
      end
    end
  end
  object OpenDialog1: TOpenDialog
    Left = 24
    Top = 64
  end
  object SaveDialog1: TSaveDialog
    Left = 24
    Top = 96
  end
end
