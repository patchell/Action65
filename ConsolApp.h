#pragma once

class CConsolApp
{
public:
	CConsolApp();
	virtual ~CConsolApp();
	virtual bool Create();
	virtual bool Run() = 0;
};

