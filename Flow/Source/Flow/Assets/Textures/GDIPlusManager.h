#pragma once

class GDIPlusManager
{
public:
	GDIPlusManager();
	~GDIPlusManager();
private:
	static unsigned long long token;
	static int refCount;
};