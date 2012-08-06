/*
 ============================================================================
 Name		: UPPayAppDemo1.cpp
 Author	  : 
 Copyright   : Your copyright notice
 Description : Main application class
 ============================================================================
 */

// INCLUDE FILES
#include <eikstart.h>
#include "UPPayAppDemo1Application.h"

LOCAL_C CApaApplication* NewApplication()
	{
	return new CUPPayAppDemo1Application;
	}

GLDEF_C TInt E32Main()
	{
	return EikStart::RunApplication(NewApplication);
	}

