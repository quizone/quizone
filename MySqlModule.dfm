object DataModule2: TDataModule2
  OldCreateOrder = False
  Height = 397
  Width = 518
  object MyCommand1: TMyCommand
    SQL.Strings = (
      
        'INSERT INTO `foralexon`.`02` (`id`, `Question`, `Hint`) VALUES (' +
        #39'2'#39', '#39'South Africa'#39', '#39'Has another country inside it'#39');')
    Left = 128
    Top = 216
  end
  object MyQuery1: TMyQuery
    Left = 184
    Top = 144
  end
  object MyTable1: TMyTable
    TableName = 'Actions'
    Left = 288
    Top = 192
  end
  object MediaPlayer1: TMediaPlayer
    FileName = 'c:\\Quizone\\Software\\Win32\\Debug\\Lose.mp3'
    Left = 288
    Top = 304
  end
  object MediaPlayer2: TMediaPlayer
    FileName = 'c:\\Quizone\\Software\\Win32\\Debug\\win.mp3'
    Left = 368
    Top = 304
  end
  object NetHTTPClient1: TNetHTTPClient
    Asynchronous = False
    ConnectionTimeout = 60000
    ResponseTimeout = 60000
    AllowCookies = True
    HandleRedirects = True
    UserAgent = 'Embarcadero URI Client/1.0'
    OnRequestError = NetHTTPClient1RequestError
    Left = 344
    Top = 104
  end
end
