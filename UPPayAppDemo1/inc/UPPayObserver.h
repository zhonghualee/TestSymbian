
#ifndef __UPPAYOBSERVER_H__
#define __UPPAYOBSERVER_H__

class MUPPayObserver
{
public:
	virtual void UPPayEvent(const TDesC8& resultStatus) = 0;
};

#endif /* __UPPAYOBSERVER_H__ */
