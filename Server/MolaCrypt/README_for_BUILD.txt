[Build.MOLA.CurVerRebuild.bat]
	- 이미 생성된 난수코드 파일을 재활용해서 빌드시

Build.MOLA.ProtocolUpdate.bat
	- SUN 프로젝트에서 ODD_NUMBER계열이 변경되고, 새로운 난수 코드를 생성할 필요가 있을 경우 사용
		- 흔히 개발 빌드시 MOLA 업데이트 한다는 의미는 바로 이것으로 빌드한 결과물이다.
	- SRC.MOLA/BBTanStream/FI_VAR_FUNC_COLLECTOR.def, BBTanStream.T1.cpp BBTanStream.T2.cpp
		- 상기 파일들 읽기 전용 해제 필요