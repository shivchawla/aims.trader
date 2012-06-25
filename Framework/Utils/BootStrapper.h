#pragma once
#include "qobject.h"
class BootStrapper : public QObject
{
public:
	BootStrapper(void);
	~BootStrapper(void);
    static void InitDatabase();
	static void ShutdownDatabase();
};

