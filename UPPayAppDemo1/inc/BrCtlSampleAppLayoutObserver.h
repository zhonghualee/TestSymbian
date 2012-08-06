/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Symbian Foundation License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.symbianfoundation.org/legal/sfl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Handle scrollbar and tab events
*
*/



#ifndef BRCTLSAMPLEAPPLAYOUTOBSERVER_H
#define BRCTLSAMPLEAPPLAYOUTOBSERVER_H

// INCLUDES
#include <BrCtlLayoutObserver.h>
#include <BrCtlInterface.h>

// FORWARD DECLARATIONS
class CBrCtlSampleAppContainer;

// CLASS DECLARATION
/**
* CBrCtlSampleAppLayoutObserver class.
* This class inherits from the MBrCtlLayoutObserver interface. Methods on this class will be
* called when layout events occur. 
*/
class CBrCtlSampleAppLayoutObserver : public CBase, public MBrCtlLayoutObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CBrCtlSampleAppLayoutObserver* NewL(CBrCtlSampleAppContainer* aContainer);

        /**
        * Destructor.
        */
        ~CBrCtlSampleAppLayoutObserver();

        /**
        * Update the position of vertical scrollbar.
        * @since 2.8
        * @param aDocumentHeight The total height of the markup page
        * @param aDisplayHeight The height of the display
        * @param aDisplayPosY The current Y position
        * @return void
        */
        void UpdateBrowserVScrollBarL(TInt aDocumentHeight, 
                                      TInt aDisplayHeight,
                                      TInt aDisplayPosY ) ;

        /**
        * Update the position of horizontal scrollbar.
        * @since 2.8
        * @param aDocumentWidth The total width of the markup page
        * @param aDisplayWidth The width of the display
        * @param aDisplayPosX The current X position
        * @return void
        */
        void UpdateBrowserHScrollBarL(TInt aDocumentWidth, 
                                      TInt aDisplayWidth,
                                      TInt aDisplayPosX) ;

        /**
        * Inform the layout of the page: right to left or left to right. Useful when the application draws the scrollbar itself.
        * @since 2.8
        * @param aNewLayout RTL or LTR
        * @return void
        */
        void NotifyLayoutChange( TBrCtlLayout aNewLayout) ;

        /**
        * Update the title of the page in history view
        * @since 3.0
        * @param aTitle Title of the page
        * @return void
        */
        void UpdateTitleL( const TDesC& aTitle );  

    private:
	    /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL(CBrCtlSampleAppContainer* aContainer);

    private:   // data
        // Pointer to the container class for our convenience.
        CBrCtlSampleAppContainer* iContainer;        
    };

#endif      // BRCTLSAMPLEAPPLAYOUTOBSERVER_H
            
// End of File

