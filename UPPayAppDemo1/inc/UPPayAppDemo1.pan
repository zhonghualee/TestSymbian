/*
 ============================================================================
 Name		: UPPayAppDemo1.pan
 Author	  : 
 Copyright   : Your copyright notice
 Description : This file contains panic codes.
 ============================================================================
 */

#ifndef __UPPAYAPPDEMO1_PAN__
#define __UPPAYAPPDEMO1_PAN__

/** UPPayAppDemo1 application panic codes */
enum TUPPayAppDemo1Panics
	{
	EUPPayAppDemo1Ui = 1
	// add further panics here
	};

inline void Panic(TUPPayAppDemo1Panics aReason)
	{
	_LIT(applicationName, "UPPayAppDemo1");
	User::Panic(applicationName, aReason);
	}

#endif // __UPPAYAPPDEMO1_PAN__
