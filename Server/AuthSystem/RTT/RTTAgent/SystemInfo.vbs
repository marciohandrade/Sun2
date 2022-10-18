On Error Resume Next

Const wbemFlagReturnImmediately = &h10
Const wbemFlagForwardOnly = &h20

arrComputers = Array("localhost")

Dim SaveString
SaveString =""

For Each strComputer In arrComputers
   Set objWMIService = GetObject("winmgmts:\\" & strComputer & "\root\CIMV2")
  
 
   SaveString = "[Memory]" & vbcrlf      

   Set colItems = objWMIService.ExecQuery("SELECT * FROM Win32_PhysicalMemory", "WQL", _
                                          wbemFlagReturnImmediately + wbemFlagForwardOnly)
   dim i
   i=1
   For Each objItem In colItems	
      SaveString = SaveString & "Physical Total " & i & "="
      SaveString = SaveString & objItem.Capacity & vbcrlf
      i=i+1
   Next


   SaveString = SaveString & vbcrlf


   Set colItems = objWMIService.ExecQuery("SELECT * FROM Win32_PerfFormattedData_PerfOS_Memory", "WQL", _
                                          wbemFlagReturnImmediately + wbemFlagForwardOnly)

   For Each objItem In colItems	
      SaveString = SaveString & "AvailableBytes=" & objItem.AvailableBytes & vbcrlf
      SaveString = SaveString & "CommitLimit=" & objItem.CommitLimit & vbcrlf
      SaveString = SaveString & "CommittedBytes=" & objItem.CommittedBytes & vbcrlf
      SaveString = SaveString & "PercentCommittedBytesInUse=" & objItem.PercentCommittedBytesInUse & vbcrlf
   Next

  
   Set colItems = objWMIService.ExecQuery("SELECT * FROM Win32_Processor", "WQL", _
                                          wbemFlagReturnImmediately + wbemFlagForwardOnly)


   SaveString = SaveString & vbcrlf


   SaveString = SaveString & "[CPU]" & vbcrlf
   i=1
   For Each objItem In colItems
      SaveString = SaveString & "CPU" & i & "=" & objItem.LoadPercentage & vbcrlf
      i=i+1
   Next
  

   Set colItems = objWMIService.ExecQuery("SELECT * FROM Win32_LogicalDisk", "WQL", _
                                          wbemFlagReturnImmediately + wbemFlagForwardOnly)



   SaveString = SaveString & vbcrlf


   For Each objItem In colItems
      If objItem.DriveType = 3 Then
         SaveString = SaveString & "[HDD " & objItem.DeviceID & "]" + vbcrlf
         SaveString = SaveString & "FreeSpace=" & objItem.FreeSpace & vbcrlf
         SaveString = SaveString & "Size=" & objItem.Size & vbcrlf

         SaveString = SaveString & vbcrlf
      End If
   Next
  
	Dim FilePath 
	FilePath = ".\SystemInfo.inf"
	
	'파일로 저장하는 부분
	Set fso = CreateObject("Scripting.FileSystemObject")
	If (fso.FileExists(FilePath )) Then
		fso.DeleteFile(FilePath)
	end if

	Set f = fso.OpenTextFile(FilePath, 2, True)	 
	f.Write SaveString 
	f.Close
	Set fso = null
Next
