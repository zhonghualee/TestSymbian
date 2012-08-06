/*
 * MVIWebClientObserver.h
 *
 *  Created on: 18.7.2012
 *      Author: Zhonghua
 */

#ifndef MVIWEBCLIENTOBSERVER_H_
#define MVIWEBCLIENTOBSERVER_H_

#include "WebClientEngine.h"
class CBrCtlSampleAppContainer;


class MVIWebClientObserver : public MWebClientObserver
	{
public:
	MVIWebClientObserver(CBrCtlSampleAppContainer* aContainer);
	virtual ~MVIWebClientObserver();
	
	/**
	 * From MWebClientObserver (see WebClientEngine.h)
	 */
	void ClientEvent( const TDesC& aEventDescription );
	void ClientHeaderReceived( const TDesC& aHeaderData );
	void ClientBodyReceived( const TDesC8& aBodyData );
private:
	CBrCtlSampleAppContainer* iContainer;
	};

#endif /* MVIWEBCLIENTOBSERVER_H_ */
