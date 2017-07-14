##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=bunget
ConfigurationName      :=Debug
WorkspacePath          := "/home/smc8series/SMCSDK/general-electric/src/codelite_projects/hmisrv"
ProjectPath            := "/home/smc8series/APPS/bunget/bunget"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=smc8series
Date                   :=12/07/17
CodeLitePath           :="/home/smc8series/.codelite"
LinkerName             :=g++
SharedObjectLinkerName :=g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-gstab
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=$(PreprocessorSwitch)DEBUG $(PreprocessorSwitch)_DEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="bunget.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../src/libbunget/include $(IncludeSwitch)../src/libbunget 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)crypto++ $(LibrarySwitch)pthread 
ArLibs                 :=  "crypto++" "pthread" 
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := g++
CC       := gcc
CXXFLAGS :=  -g -O0 -std=c++11 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) $(IntermediateDirectory)/libbunget_bu_hci.cpp$(ObjectSuffix) $(IntermediateDirectory)/libbunget_ascon.cpp$(ObjectSuffix) $(IntermediateDirectory)/libbunget_libbungetpriv.cpp$(ObjectSuffix) $(IntermediateDirectory)/libbunget_hci_socket.cpp$(ObjectSuffix) $(IntermediateDirectory)/libbunget_bu_gatt.cpp$(ObjectSuffix) $(IntermediateDirectory)/libbunget_sco_socket.cpp$(ObjectSuffix) $(IntermediateDirectory)/libbunget_gattdefs.cpp$(ObjectSuffix) $(IntermediateDirectory)/libbunget_bt_socket.cpp$(ObjectSuffix) $(IntermediateDirectory)/libbunget_l2cap_socket.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/libbunget_rfcomm_socket.cpp$(ObjectSuffix) $(IntermediateDirectory)/libbunget_bu_gap.cpp$(ObjectSuffix) $(IntermediateDirectory)/libbunget_bybuff.cpp$(ObjectSuffix) $(IntermediateDirectory)/libbunget_uguid.cpp$(ObjectSuffix) $(IntermediateDirectory)/libbunget_secmanp.cpp$(ObjectSuffix) $(IntermediateDirectory)/libbunget_hci_config.cpp$(ObjectSuffix) $(IntermediateDirectory)/libbunget_bt_incinpl.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_crypto.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix): ../src/main.cpp $(IntermediateDirectory)/src_main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/smc8series/APPS/bunget/src/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_main.cpp$(DependSuffix): ../src/main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_main.cpp$(DependSuffix) -MM "../src/main.cpp"

$(IntermediateDirectory)/src_main.cpp$(PreprocessSuffix): ../src/main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_main.cpp$(PreprocessSuffix) "../src/main.cpp"

$(IntermediateDirectory)/libbunget_bu_hci.cpp$(ObjectSuffix): ../src/libbunget/bu_hci.cpp $(IntermediateDirectory)/libbunget_bu_hci.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/smc8series/APPS/bunget/src/libbunget/bu_hci.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/libbunget_bu_hci.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/libbunget_bu_hci.cpp$(DependSuffix): ../src/libbunget/bu_hci.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/libbunget_bu_hci.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/libbunget_bu_hci.cpp$(DependSuffix) -MM "../src/libbunget/bu_hci.cpp"

$(IntermediateDirectory)/libbunget_bu_hci.cpp$(PreprocessSuffix): ../src/libbunget/bu_hci.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/libbunget_bu_hci.cpp$(PreprocessSuffix) "../src/libbunget/bu_hci.cpp"

$(IntermediateDirectory)/libbunget_ascon.cpp$(ObjectSuffix): ../src/libbunget/ascon.cpp $(IntermediateDirectory)/libbunget_ascon.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/smc8series/APPS/bunget/src/libbunget/ascon.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/libbunget_ascon.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/libbunget_ascon.cpp$(DependSuffix): ../src/libbunget/ascon.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/libbunget_ascon.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/libbunget_ascon.cpp$(DependSuffix) -MM "../src/libbunget/ascon.cpp"

$(IntermediateDirectory)/libbunget_ascon.cpp$(PreprocessSuffix): ../src/libbunget/ascon.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/libbunget_ascon.cpp$(PreprocessSuffix) "../src/libbunget/ascon.cpp"

$(IntermediateDirectory)/libbunget_libbungetpriv.cpp$(ObjectSuffix): ../src/libbunget/libbungetpriv.cpp $(IntermediateDirectory)/libbunget_libbungetpriv.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/smc8series/APPS/bunget/src/libbunget/libbungetpriv.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/libbunget_libbungetpriv.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/libbunget_libbungetpriv.cpp$(DependSuffix): ../src/libbunget/libbungetpriv.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/libbunget_libbungetpriv.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/libbunget_libbungetpriv.cpp$(DependSuffix) -MM "../src/libbunget/libbungetpriv.cpp"

$(IntermediateDirectory)/libbunget_libbungetpriv.cpp$(PreprocessSuffix): ../src/libbunget/libbungetpriv.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/libbunget_libbungetpriv.cpp$(PreprocessSuffix) "../src/libbunget/libbungetpriv.cpp"

$(IntermediateDirectory)/libbunget_hci_socket.cpp$(ObjectSuffix): ../src/libbunget/hci_socket.cpp $(IntermediateDirectory)/libbunget_hci_socket.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/smc8series/APPS/bunget/src/libbunget/hci_socket.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/libbunget_hci_socket.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/libbunget_hci_socket.cpp$(DependSuffix): ../src/libbunget/hci_socket.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/libbunget_hci_socket.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/libbunget_hci_socket.cpp$(DependSuffix) -MM "../src/libbunget/hci_socket.cpp"

$(IntermediateDirectory)/libbunget_hci_socket.cpp$(PreprocessSuffix): ../src/libbunget/hci_socket.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/libbunget_hci_socket.cpp$(PreprocessSuffix) "../src/libbunget/hci_socket.cpp"

$(IntermediateDirectory)/libbunget_bu_gatt.cpp$(ObjectSuffix): ../src/libbunget/bu_gatt.cpp $(IntermediateDirectory)/libbunget_bu_gatt.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/smc8series/APPS/bunget/src/libbunget/bu_gatt.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/libbunget_bu_gatt.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/libbunget_bu_gatt.cpp$(DependSuffix): ../src/libbunget/bu_gatt.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/libbunget_bu_gatt.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/libbunget_bu_gatt.cpp$(DependSuffix) -MM "../src/libbunget/bu_gatt.cpp"

$(IntermediateDirectory)/libbunget_bu_gatt.cpp$(PreprocessSuffix): ../src/libbunget/bu_gatt.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/libbunget_bu_gatt.cpp$(PreprocessSuffix) "../src/libbunget/bu_gatt.cpp"

$(IntermediateDirectory)/libbunget_sco_socket.cpp$(ObjectSuffix): ../src/libbunget/sco_socket.cpp $(IntermediateDirectory)/libbunget_sco_socket.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/smc8series/APPS/bunget/src/libbunget/sco_socket.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/libbunget_sco_socket.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/libbunget_sco_socket.cpp$(DependSuffix): ../src/libbunget/sco_socket.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/libbunget_sco_socket.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/libbunget_sco_socket.cpp$(DependSuffix) -MM "../src/libbunget/sco_socket.cpp"

$(IntermediateDirectory)/libbunget_sco_socket.cpp$(PreprocessSuffix): ../src/libbunget/sco_socket.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/libbunget_sco_socket.cpp$(PreprocessSuffix) "../src/libbunget/sco_socket.cpp"

$(IntermediateDirectory)/libbunget_gattdefs.cpp$(ObjectSuffix): ../src/libbunget/gattdefs.cpp $(IntermediateDirectory)/libbunget_gattdefs.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/smc8series/APPS/bunget/src/libbunget/gattdefs.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/libbunget_gattdefs.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/libbunget_gattdefs.cpp$(DependSuffix): ../src/libbunget/gattdefs.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/libbunget_gattdefs.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/libbunget_gattdefs.cpp$(DependSuffix) -MM "../src/libbunget/gattdefs.cpp"

$(IntermediateDirectory)/libbunget_gattdefs.cpp$(PreprocessSuffix): ../src/libbunget/gattdefs.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/libbunget_gattdefs.cpp$(PreprocessSuffix) "../src/libbunget/gattdefs.cpp"

$(IntermediateDirectory)/libbunget_bt_socket.cpp$(ObjectSuffix): ../src/libbunget/bt_socket.cpp $(IntermediateDirectory)/libbunget_bt_socket.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/smc8series/APPS/bunget/src/libbunget/bt_socket.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/libbunget_bt_socket.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/libbunget_bt_socket.cpp$(DependSuffix): ../src/libbunget/bt_socket.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/libbunget_bt_socket.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/libbunget_bt_socket.cpp$(DependSuffix) -MM "../src/libbunget/bt_socket.cpp"

$(IntermediateDirectory)/libbunget_bt_socket.cpp$(PreprocessSuffix): ../src/libbunget/bt_socket.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/libbunget_bt_socket.cpp$(PreprocessSuffix) "../src/libbunget/bt_socket.cpp"

$(IntermediateDirectory)/libbunget_l2cap_socket.cpp$(ObjectSuffix): ../src/libbunget/l2cap_socket.cpp $(IntermediateDirectory)/libbunget_l2cap_socket.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/smc8series/APPS/bunget/src/libbunget/l2cap_socket.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/libbunget_l2cap_socket.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/libbunget_l2cap_socket.cpp$(DependSuffix): ../src/libbunget/l2cap_socket.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/libbunget_l2cap_socket.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/libbunget_l2cap_socket.cpp$(DependSuffix) -MM "../src/libbunget/l2cap_socket.cpp"

$(IntermediateDirectory)/libbunget_l2cap_socket.cpp$(PreprocessSuffix): ../src/libbunget/l2cap_socket.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/libbunget_l2cap_socket.cpp$(PreprocessSuffix) "../src/libbunget/l2cap_socket.cpp"

$(IntermediateDirectory)/libbunget_rfcomm_socket.cpp$(ObjectSuffix): ../src/libbunget/rfcomm_socket.cpp $(IntermediateDirectory)/libbunget_rfcomm_socket.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/smc8series/APPS/bunget/src/libbunget/rfcomm_socket.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/libbunget_rfcomm_socket.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/libbunget_rfcomm_socket.cpp$(DependSuffix): ../src/libbunget/rfcomm_socket.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/libbunget_rfcomm_socket.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/libbunget_rfcomm_socket.cpp$(DependSuffix) -MM "../src/libbunget/rfcomm_socket.cpp"

$(IntermediateDirectory)/libbunget_rfcomm_socket.cpp$(PreprocessSuffix): ../src/libbunget/rfcomm_socket.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/libbunget_rfcomm_socket.cpp$(PreprocessSuffix) "../src/libbunget/rfcomm_socket.cpp"

$(IntermediateDirectory)/libbunget_bu_gap.cpp$(ObjectSuffix): ../src/libbunget/bu_gap.cpp $(IntermediateDirectory)/libbunget_bu_gap.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/smc8series/APPS/bunget/src/libbunget/bu_gap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/libbunget_bu_gap.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/libbunget_bu_gap.cpp$(DependSuffix): ../src/libbunget/bu_gap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/libbunget_bu_gap.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/libbunget_bu_gap.cpp$(DependSuffix) -MM "../src/libbunget/bu_gap.cpp"

$(IntermediateDirectory)/libbunget_bu_gap.cpp$(PreprocessSuffix): ../src/libbunget/bu_gap.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/libbunget_bu_gap.cpp$(PreprocessSuffix) "../src/libbunget/bu_gap.cpp"

$(IntermediateDirectory)/libbunget_bybuff.cpp$(ObjectSuffix): ../src/libbunget/bybuff.cpp $(IntermediateDirectory)/libbunget_bybuff.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/smc8series/APPS/bunget/src/libbunget/bybuff.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/libbunget_bybuff.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/libbunget_bybuff.cpp$(DependSuffix): ../src/libbunget/bybuff.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/libbunget_bybuff.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/libbunget_bybuff.cpp$(DependSuffix) -MM "../src/libbunget/bybuff.cpp"

$(IntermediateDirectory)/libbunget_bybuff.cpp$(PreprocessSuffix): ../src/libbunget/bybuff.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/libbunget_bybuff.cpp$(PreprocessSuffix) "../src/libbunget/bybuff.cpp"

$(IntermediateDirectory)/libbunget_uguid.cpp$(ObjectSuffix): ../src/libbunget/uguid.cpp $(IntermediateDirectory)/libbunget_uguid.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/smc8series/APPS/bunget/src/libbunget/uguid.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/libbunget_uguid.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/libbunget_uguid.cpp$(DependSuffix): ../src/libbunget/uguid.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/libbunget_uguid.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/libbunget_uguid.cpp$(DependSuffix) -MM "../src/libbunget/uguid.cpp"

$(IntermediateDirectory)/libbunget_uguid.cpp$(PreprocessSuffix): ../src/libbunget/uguid.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/libbunget_uguid.cpp$(PreprocessSuffix) "../src/libbunget/uguid.cpp"

$(IntermediateDirectory)/libbunget_secmanp.cpp$(ObjectSuffix): ../src/libbunget/secmanp.cpp $(IntermediateDirectory)/libbunget_secmanp.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/smc8series/APPS/bunget/src/libbunget/secmanp.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/libbunget_secmanp.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/libbunget_secmanp.cpp$(DependSuffix): ../src/libbunget/secmanp.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/libbunget_secmanp.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/libbunget_secmanp.cpp$(DependSuffix) -MM "../src/libbunget/secmanp.cpp"

$(IntermediateDirectory)/libbunget_secmanp.cpp$(PreprocessSuffix): ../src/libbunget/secmanp.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/libbunget_secmanp.cpp$(PreprocessSuffix) "../src/libbunget/secmanp.cpp"

$(IntermediateDirectory)/libbunget_hci_config.cpp$(ObjectSuffix): ../src/libbunget/hci_config.cpp $(IntermediateDirectory)/libbunget_hci_config.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/smc8series/APPS/bunget/src/libbunget/hci_config.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/libbunget_hci_config.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/libbunget_hci_config.cpp$(DependSuffix): ../src/libbunget/hci_config.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/libbunget_hci_config.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/libbunget_hci_config.cpp$(DependSuffix) -MM "../src/libbunget/hci_config.cpp"

$(IntermediateDirectory)/libbunget_hci_config.cpp$(PreprocessSuffix): ../src/libbunget/hci_config.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/libbunget_hci_config.cpp$(PreprocessSuffix) "../src/libbunget/hci_config.cpp"

$(IntermediateDirectory)/libbunget_bt_incinpl.cpp$(ObjectSuffix): ../src/libbunget/bt_incinpl.cpp $(IntermediateDirectory)/libbunget_bt_incinpl.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/smc8series/APPS/bunget/src/libbunget/bt_incinpl.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/libbunget_bt_incinpl.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/libbunget_bt_incinpl.cpp$(DependSuffix): ../src/libbunget/bt_incinpl.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/libbunget_bt_incinpl.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/libbunget_bt_incinpl.cpp$(DependSuffix) -MM "../src/libbunget/bt_incinpl.cpp"

$(IntermediateDirectory)/libbunget_bt_incinpl.cpp$(PreprocessSuffix): ../src/libbunget/bt_incinpl.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/libbunget_bt_incinpl.cpp$(PreprocessSuffix) "../src/libbunget/bt_incinpl.cpp"

$(IntermediateDirectory)/src_crypto.cpp$(ObjectSuffix): ../src/crypto.cpp $(IntermediateDirectory)/src_crypto.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/smc8series/APPS/bunget/src/crypto.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_crypto.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_crypto.cpp$(DependSuffix): ../src/crypto.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_crypto.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_crypto.cpp$(DependSuffix) -MM "../src/crypto.cpp"

$(IntermediateDirectory)/src_crypto.cpp$(PreprocessSuffix): ../src/crypto.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_crypto.cpp$(PreprocessSuffix) "../src/crypto.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


