unit Main;
{ This program was write fast, so don`t say nothink about it.
It`s just work :). I`ll try to rewrite it as object-oriented...}

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, Menus, About, ExtCtrls, Buttons, StdCtrls;

type
  TForm1 = class(TForm)
    MainMenu1: TMainMenu;
    Map1: TMenuItem;
    Load1: TMenuItem;
    Save1: TMenuItem;
    Help1: TMenuItem;
    About1: TMenuItem;
    OpenDialog1: TOpenDialog;
    SaveDialog1: TSaveDialog;
    SpeedButton1: TSpeedButton;
    SpeedButton2: TSpeedButton;
    SpeedButton3: TSpeedButton;
    SpeedButton4: TSpeedButton;
    SpeedButton5: TSpeedButton;
    SpeedButton6: TSpeedButton;
    Label1: TLabel;
    Label2: TLabel;
    GroupBox1: TGroupBox;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    Label6: TLabel;
    Edit1: TEdit;
    Edit2: TEdit;
    Edit3: TEdit;
    Edit4: TEdit;
    Label7: TLabel;
    Edit5: TEdit;
    Panel1: TPanel;
    Image1: TImage;
    Button1: TButton;
    Label8: TLabel;
    Options1: TMenuItem;
    Planets1: TMenuItem;
    ConstantSize1: TMenuItem;
    procedure About1Click(Sender: TObject);
    procedure Load1Click(Sender: TObject);
    procedure SpeedButton1Click(Sender: TObject);
    procedure SpeedButton2Click(Sender: TObject);
    procedure SpeedButton3Click(Sender: TObject);
    procedure SpeedButton6Click(Sender: TObject);
    procedure SpeedButton5Click(Sender: TObject);
    procedure SpeedButton4Click(Sender: TObject);
    procedure Save1Click(Sender: TObject);
  {  procedure PaintBox1MouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure PaintBox1MouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);}
    procedure FormResize(Sender: TObject);
    procedure Edit1Exit(Sender: TObject);
    procedure Edit2Change(Sender: TObject);
    procedure Edit3Exit(Sender: TObject);
    procedure Edit5Exit(Sender: TObject);
    procedure Edit4Exit(Sender: TObject);
    procedure Edit2KeyPress(Sender: TObject; var Key: Char);
    procedure FormCreate(Sender: TObject);
    procedure Image1MouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure Image1MouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure FormActivate(Sender: TObject);
    procedure Image1MouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure Button1Click(Sender: TObject);
    procedure Edit1KeyPress(Sender: TObject; var Key: Char);
    procedure Edit3KeyPress(Sender: TObject; var Key: Char);
    procedure Edit4KeyPress(Sender: TObject; var Key: Char);
    procedure Edit5KeyPress(Sender: TObject; var Key: Char);
    procedure ConstantSize1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

  TOptions=class
  public
    PlanetsConstSize:boolean;
  end;

  TPlanet = class
  public
    Name : String;
    OwnerID : integer;
    X : extended;
    Y : extended;
    Size : extended;
    Res : extended;
    Pop : extended;
    Ind : extended;
    Production : integer; //надо узнать у Криса про это по подробнее
  end;

  TRace = class
  public
    ID : integer;
    Name : String;
    Adress : String;
    Password : String;
  end;

var
  Form1: TForm1;
  MapFileName : String; // имя файла карты
  Planets : Array of TPlanet; // Массив планет :)
  Races : Array of TRace;     // Массив расс :)
  Scale : extended;  // Масштаб
  MapSize : extended; // размер карты
  ScreenX,ScreenY : extended; // Координаты цента
  MapLoad : boolean;
  SelectedPlanet : integer; // номер планеты к которой точка клика ближе всего
  MBRightPress : boolean;
  CashBitMap1,CashBitMap2:TBitMap;
  CurrentFrame : boolean;
  MoveScreenStartPoint : TPoint;
  ScreenXBeforeMoving, ScreenYBeforeMoving : extended;
  SettingNewCoordinates : boolean;
  Options : TOptions;
implementation

{$R *.dfm}

Procedure SelectPlanet(PlanetNumber : integer);
begin
  SelectedPlanet:=PlanetNumber;
  Form1.Edit1.Text:=Planets[SelectedPlanet].Name;
  Form1.Edit2.Text:=FloatToStr(Planets[SelectedPlanet].X);
  Form1.Edit3.Text:=FloatToStr(Planets[SelectedPlanet].Y);
  Form1.Edit4.Text:=FloatToStr(Planets[SelectedPlanet].Size);
  Form1.Edit5.Text:=FloatToStr(Planets[SelectedPlanet].Res);
end;

function StringToFloat(StringIn : String): extended;
var TempString : String;
    k : integer;
begin
  k:=Pos('.',StringIn); // позиция точки в числе
  if k=0 then           // если точки нет то просто конвертируем
  begin
    Result:=StrToFloat(StringIn);
  end else
  begin
    TempString:=Copy(StringIn,0,k-1)+Copy(StringIn,k+1,Length(StringIn)-k);
    Result:=StrToFloat(TempString)/exp((Length(StringIn)-k)*ln(10));
  end;
end;


procedure TForm1.About1Click(Sender: TObject);  // About...
begin
  About.AboutBox.Visible:=true;
end;

function Radius(PlanetNumber : integer):extended;
var Rad : extended;
begin
  Rad:=Sqrt(Planets[PlanetNumber].Size*Scale)/15;
  if Rad<1 then Rad:=1;
  if Options.PlanetsConstSize then Rad:=2;
  Result:=Rad;   
end;


Procedure Render;   // it is very simple, maybe i`ll rewrite it.
var i:integer;
    PlanetX,PlanetY:extended;
    PlanetColor:TColor;
begin
  if CurrentFrame then
  begin
    CashBitMap1.Width:=Form1.Image1.Width;
    CashBitMap1.Height:=Form1.Image1.Height;
    With CashBitMap1 do
    begin
      Canvas.Brush.Color:=clBlack;
      Canvas.Rectangle(0,0,Width,Height);   // закрашиваем в черный цвет
      For i:=0 to Length(Planets)-1 do  // типа планеты
      begin
        PlanetX:=ScreenX*Scale-MapSize*Scale+Width/2+Scale*(Planets[i].X);
        PlanetY:=ScreenY*Scale+Height/2-Scale*(Planets[i].Y);
        if Planets[i].OwnerID=0 then PlanetColor:=clGreen else PlanetColor:=ClWhite;
        if i=SelectedPlanet then PlanetColor:=clRed;
        Canvas.Pen.Color:=PlanetColor;
        Canvas.Brush.Color:=PlanetColor;
        Canvas.Ellipse(                        // тут надо будет ввести радиус...
          Round(PlanetX-Radius(i)),
          Round(PlanetY-Radius(i)),
          Round(PlanetX+Radius(i)),
          Round(PlanetY+Radius(i))
        );
      end;
    end;
  Form1.Image1.Picture.Bitmap:=CashBitmap1;
  CurrentFrame:=not(CurrentFrame);  
  end else
  begin
    CashBitMap2.Width:=Form1.Image1.Width;
    CashBitMap2.Height:=Form1.Image1.Height;
    With CashBitMap2 do
    begin
      Canvas.Brush.Color:=clBlack;
      Canvas.Rectangle(0,0,Width,Height);   // закрашиваем в черный цвет
      For i:=0 to Length(Planets)-1 do  // типа планеты
      begin
        PlanetX:=ScreenX*Scale-MapSize*Scale+Width/2+Scale*(Planets[i].X);
        PlanetY:=ScreenY*Scale+Height/2-Scale*(Planets[i].Y);
        if Planets[i].OwnerID=0 then PlanetColor:=clGreen else PlanetColor:=ClWhite;
        if i=SelectedPlanet then PlanetColor:=clRed;
        Canvas.Pen.Color:=PlanetColor;
        Canvas.Brush.Color:=PlanetColor;
        Canvas.Ellipse(                        // тут надо будет ввести радиус...
          Round(PlanetX-Radius(i)),
          Round(PlanetY-Radius(i)),
          Round(PlanetX+Radius(i)),
          Round(PlanetY+Radius(i))
        );
      end;
    end;
  Form1.Image1.Picture.Bitmap:=CashBitmap2;
  CurrentFrame:=not(CurrentFrame);
  end;

end;


procedure TForm1.Load1Click(Sender: TObject); // Load...
var i : integer; // переменная цикла
    j : integer; // Счетчик рас, планет
    MapFile : TextFile; // переменая связаная с файлом
    CurrentString : String; // текущая читаемая строка
begin
  if OpenDialog1.Execute then
  begin
    MapFileName:=OpenDialog1.FileName; // для удобства и последующего сохранения в него же
    AssignFile(MapFile, MapFileName);
    Reset(MapFile);
    For i:=1 to 3 do ReadLn(MapFile, CurrentString);
    MapSize:=StringToFloat(CurrentString);
    Repeat
      ReadLn(MapFile, CurrentString);
    Until (CurrentString='@Players');
    j:=0;
    Repeat
      ReadLn(MapFile, CurrentString);
      if CurrentString='@EPlayers' then Break;
      j:=j+1;
      SetLength(Races,j);
      Races[j-1]:=TRace.Create;
      Races[j-1].ID:=j;
      Races[j-1].Name:=CurrentString;
      ReadLn(MapFile, CurrentString);
      Races[j-1].Adress:=CurrentString;
      ReadLn(MapFile, CurrentString);
      Races[j-1].Password:=CurrentString;
      For i:=1 to 20 do ReadLn(MapFile, CurrentString);
    Until false;
    Repeat
      ReadLn(MapFile, CurrentString);
    Until (CurrentString='@Planets');
    j:=0;
    Repeat
      ReadLn(MapFile, CurrentString);
      if CurrentString='@EPlanets' then Break;
      j:=j+1;
      SetLength(Planets,j);
      Planets[j-1]:=TPlanet.Create;
      Planets[j-1].Name:=CurrentString;
      ReadLn(MapFile, CurrentString);
      if CurrentString='' then Planets[j-1].OwnerID:=0 else Planets[j-1].OwnerID:=StrToInt(CurrentString);
      ReadLn(MapFile, CurrentString);
      Planets[j-1].X:=StringToFloat(CurrentString);
      ReadLn(MapFile, CurrentString);
      Planets[j-1].Y:=StringToFloat(CurrentString);
      ReadLn(MapFile, CurrentString);
      Planets[j-1].Size:=StringToFloat(CurrentString);
      ReadLn(MapFile, CurrentString);
      Planets[j-1].Res:=StringToFloat(CurrentString);
      ReadLn(MapFile, CurrentString);
      if CurrentString='' then Planets[j-1].Pop:=0 else Planets[j-1].Pop:=StringToFloat(CurrentString);
      ReadLn(MapFile, CurrentString);
      if CurrentString='' then Planets[j-1].Ind:=0 else Planets[j-1].Ind:=StringToFloat(CurrentString);
      ReadLn(MapFile, CurrentString);
      if CurrentString='' then Planets[j-1].Production:=0 else Planets[j-1].Production:=StrToInt(CurrentString);
      For i:=1 to 6 do ReadLn(MapFile, CurrentString);
    Until false;
    CloseFile(MapFile);
    Scale:=Image1.Height/MapSize; // надо будет сделать с учетом ширины и высоты
    ScreenX:=MapSize/2;
    ScreenY:=MapSize/2;
    MapLoad:=true;
    Edit1.Text:=Planets[SelectedPlanet].Name;
    Edit2.Text:=FloatToStr(Planets[SelectedPlanet].X);
    Edit3.Text:=FloatToStr(Planets[SelectedPlanet].Y);
    Edit4.Text:=FloatToStr(Planets[SelectedPlanet].Size);
    Edit5.Text:=FloatToStr(Planets[SelectedPlanet].Res);
    SelectPlanet(0);
    Render;
  end;
end;


procedure TForm1.SpeedButton1Click(Sender: TObject);
begin
  Scale:=Scale*2;
  Render;
end;

procedure TForm1.SpeedButton2Click(Sender: TObject);
begin
  Scale:=Scale/2;
  Render;
end;

procedure TForm1.SpeedButton3Click(Sender: TObject);
begin
  ScreenY:=ScreenY+Image1.Height/10/Scale;
  Render;
end;

procedure TForm1.SpeedButton6Click(Sender: TObject);
begin
  ScreenY:=ScreenY-Image1.Height/10/Scale;
  Render;
end;

procedure TForm1.SpeedButton5Click(Sender: TObject);
begin
  ScreenX:=ScreenX-Image1.Width/10/Scale;
  Render;
end;

procedure TForm1.SpeedButton4Click(Sender: TObject);
begin
  ScreenX:=ScreenX+Image1.Width/10/Scale;
  Render;
end;

procedure TForm1.Save1Click(Sender: TObject);  // Save as... примерно тот же Load только наоборот :)
var FileIn, FileOut : TextFile;
    CurrentString : String;
    i,j : integer;
begin
  if SaveDialog1.Execute then
  begin
    If (SaveDialog1.FileName=MapFileName) then  ShowMessage('Choose not edditing now file. (Will remove this message later :)') else
    begin

    AssignFile(FileOut, MapFileName);         // Копируем текущий файл в резервную копию
    Reset(FileOut);
    AssignFile(FileIn, MapFileName+'.bak');
    Rewrite(FileIn);
    repeat
      ReadLn(FileOut, CurrentString);
      Write(FileIn, CurrentString+Chr(10));
    until EOF(FileOut);
    CloseFile(FileOut);
    CloseFile(FileIn);

    AssignFile(FileIn, SaveDialog1.FileName);  // А затем из резервной копии "получаем" отредактированную версию.
    AssignFile(FileOut,MapFileName);
    reset(FileOut);
    rewrite(FileIn);

    Repeat                       // копируем все до секции планет (затем надо будет добавить возможность редактировать игроков)
      ReadLn(FileOut, CurrentString);
      Write(FileIn,CurrentString,Chr(10));
    Until (CurrentString='@Planets');

    For j:=0 to Length(Planets)-1 do  // секция планет
    begin
      Write(FileIn,Planets[j].Name,Chr(10));
      if Planets[j].OwnerID=0 then Write(FileIn,Chr(10)) else Write(FileIn,Planets[j].OwnerID,Chr(10));
      Write(FileIn,Planets[j].X:0:2,Chr(10));
      Write(FileIn,Planets[j].Y:0:2,Chr(10));
      Write(FileIn,Planets[j].Size:0:3,Chr(10));
      Write(FileIn,Planets[j].Res:0:2,Chr(10));
      if Planets[j].Pop=0 then Write(FileIn,Chr(10)) else Write(FileIn,Planets[j].Pop:0:2,Chr(10));
      if Planets[j].Ind=0 then Write(FileIn,Chr(10)) else Write(FileIn,Planets[j].Ind:0:2,Chr(10));
      if Planets[j].Production=0 then Write(FileIn,Chr(10)) else Write(FileIn,Planets[j].Production,Chr(10));
      For i:=1 to 6 do Write(FileIn,Chr(10));
    end;

    //теперь просто копируем все остальное
    Repeat //пропускаем в исходном файле секцию планет
      ReadLn(FileOut,CurrentString);
    Until CurrentString='@EPlanets';
    Write(FileIn,CurrentString,Chr(10));
    Repeat
      ReadLn(FileOut,CurrentString);
      Write(FileIn,CurrentString,Chr(10));
    Until EOF(FileOut);
    CloseFile(FileIn);
    CloseFile(FileOut);

    end;
  end;
end;


procedure TForm1.FormResize(Sender: TObject);
begin
  Form1.Image1.Width:=Form1.Width-180;
  Form1.Image1.Height:=Form1.Height-100;
  Form1.GroupBox1.Left:=Form1.Width-160;
  Form1.Panel1.Left:=Form1.Width-160;
  Form1.Label8.Left:=Form1.Width-160;
  Form1.Label1.Top:=Form1.Height-90;
  Form1.Label2.Top:=Form1.Height-70;
  Render;
end;

procedure TForm1.Edit1Exit(Sender: TObject);
begin
  Planets[SelectedPlanet].Name:=Edit1.Text;
end;

procedure TForm1.Edit2Change(Sender: TObject);
begin
  Planets[SelectedPlanet].X:=StrToFloat(Edit2.Text);
end;

procedure TForm1.Edit3Exit(Sender: TObject);
begin
  Planets[SelectedPlanet].Y:=StrToFloat(Edit3.Text);
end;

procedure TForm1.Edit4Exit(Sender: TObject);
begin
  Planets[SelectedPlanet].Size:=StrToFloat(Edit4.Text);
end;


procedure TForm1.Edit5Exit(Sender: TObject);
begin
  Planets[SelectedPlanet].Res:=StrToFloat(Edit5.Text);
end;


procedure TForm1.Edit2KeyPress(Sender: TObject; var Key: Char);
begin
  if Sender is TEdit then
  if Key=#13 then Key:=#0;
  Render;
end;

procedure TForm1.FormCreate(Sender: TObject);
begin
  CashBitMap1:=TBitMap.Create;
  CashBitMap2:=TBitMap.Create;
  Options:=TOptions.Create;
  Options.PlanetsConstSize:=false;
  SelectedPlanet:=0;
end;

procedure TForm1.Image1MouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
var X1,Y1: extended; // координаты клика в l.y.
    L, Lmin : extended; // рассояние от точки клика до планеты и минимальное расстояние
    i: integer;
begin
  If Button=MBLeft then           // левая кнопка - выделяем планету
  begin
    If MapLoad then
    begin
      if SettingNewCoordinates then
      begin
        Planets[SelectedPlanet].X:=(X-ScreenX*Scale+MapSize*Scale-Form1.Image1.Width/2)/Scale;
        Planets[SelectedPlanet].Y:=(-Y+ScreenY*Scale+Form1.Image1.Height/2)/Scale;
        SettingNewCoordinates:=false;
      end else
      begin
        With Form1.Image1 do
        begin
          X1:=(X-ScreenX*Scale+MapSize*Scale-Form1.Image1.Width/2)/Scale;
          Y1:=(-Y+ScreenY*Scale+Form1.Image1.Height/2)/Scale;
        end;
        Lmin:=sqrt(sqr(x1-Planets[0].x)+sqr(y1-Planets[0].y));
        SelectedPlanet:=0;
        For i:=1 to Length(Planets)-1 do
        begin
          L:=sqrt(sqr(x1-Planets[i].x)+sqr(y1-Planets[i].y));
          if L<Lmin then
          begin
            SelectedPlanet:=i;
            Lmin:=L;
          end;
        end;
        SelectPlanet(SelectedPlanet);
      end;
    end;
  end;
  If Button=MBRight then              // правая кнопка - двигаем карту
  begin
    MBRightPress:=true;
 //   GetCursorPos(MoveScreenStartPoint);
    MoveScreenStartPoint.x:=x;
    MoveScreenStartPoint.Y:=y;
    ScreenXBeforeMoving:=ScreenX;
    ScreenYBeforeMoving:=ScreenY;
  end;
  Render;
end;


procedure TForm1.Image1MouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
var CurrentCursorPos : TPoint;
begin
  if MapLoad then
  begin
    If MBRightPress then                // типа двигаем карту правой мышой
    begin
      With Form1.Image1 do
      begin
        ScreenX:=ScreenXBeforeMoving+(X-MoveScreenStartPoint.x)/scale;
        ScreenY:=ScreenYBeforeMoving+(Y-MoveScreenStartPoint.y)/scale;
      end;
      Render;
    end;
    With Form1.Image1 do                                     // выводим координаты внизу экрана
    begin
      Label1.Caption:='X: '+FloatToStr(Int((X-ScreenX*Scale+MapSize*Scale-Form1.Image1.Width/2)/Scale*100)/100);
      Label2.Caption:='Y: '+FloatToStr(Int((-Y+ScreenY*Scale+Form1.Image1.Height/2)/Scale*100)/100);
    end;
  end;
end;

procedure TForm1.FormActivate(Sender: TObject);
begin
  Render;
end;

procedure TForm1.Image1MouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  If Button=MBRight then
  begin
    MBRightPress:=false;
  end;

end;

procedure TForm1.Button1Click(Sender: TObject);
begin
  Label8.Caption:='Click on new planet position';
  SettingNewCoordinates:=true;
end;

procedure TForm1.Edit1KeyPress(Sender: TObject; var Key: Char);
begin
  if Sender is TEdit then
  if Key=#13 then Key:=#0;
  Render;
end;

procedure TForm1.Edit3KeyPress(Sender: TObject; var Key: Char);
begin
  if Sender is TEdit then
  if Key=#13 then Key:=#0;
  Render;
end;

procedure TForm1.Edit4KeyPress(Sender: TObject; var Key: Char);
begin
  if Sender is TEdit then
  if Key=#13 then Key:=#0;
  Render;
end;

procedure TForm1.Edit5KeyPress(Sender: TObject; var Key: Char);
begin
  if Sender is TEdit then
  if Key=#13 then Key:=#0;
  Render;
end;

procedure TForm1.ConstantSize1Click(Sender: TObject);
begin
  Options.PlanetsConstSize:=not(Options.PlanetsConstSize);
  Render;
end;

end.
