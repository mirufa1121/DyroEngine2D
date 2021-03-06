#ifndef _LOGIC_H
#define _LOGIC_H

#include "System.h"
#include "Interfaces/IDrawable.h"

class AbstractGame;

class Logic : public System, public IDrawable
{
public:
	Logic();
	virtual ~Logic();

	virtual bool initialize();
	void update();
	void draw();
	virtual bool shutdown();

	void setGame(AbstractGame* game);

private:

	AbstractGame* game;
};

#endif