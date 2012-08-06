# ============================================================================
#  Name	 : build_help.mk
#  Part of  : UPPayAppDemo1
# ============================================================================
#  Name	 : build_help.mk
#  Part of  : UPPayAppDemo1
#
#  Description: This make file will build the application help file (.hlp)
# 
# ============================================================================

do_nothing :
	@rem do_nothing

# build the help from the MAKMAKE step so the header file generated
# will be found by cpp.exe when calculating the dependency information
# in the mmp makefiles.

MAKMAKE : UPPayAppDemo1_0xe55dc470.hlp
UPPayAppDemo1_0xe55dc470.hlp : UPPayAppDemo1.xml UPPayAppDemo1.cshlp Custom.xml
	cshlpcmp.bat UPPayAppDemo1.cshlp
ifeq (WINSCW,$(findstring WINSCW, $(PLATFORM)))
	md $(EPOCROOT)epoc32\$(PLATFORM)\c\resource\help
	copy UPPayAppDemo1_0xe55dc470.hlp $(EPOCROOT)epoc32\$(PLATFORM)\c\resource\help
endif

BLD : do_nothing

CLEAN :
	del UPPayAppDemo1_0xe55dc470.hlp
	del UPPayAppDemo1_0xe55dc470.hlp.hrh

LIB : do_nothing

CLEANLIB : do_nothing

RESOURCE : do_nothing
		
FREEZE : do_nothing

SAVESPACE : do_nothing

RELEASABLES :
	@echo UPPayAppDemo1_0xe55dc470.hlp

FINAL : do_nothing
