#ifndef _ABSTRACTGAME_H
#define _ABSTRACTGAME_H

class AbstractGame
{
public:
	AbstractGame();
	virtual ~AbstractGame();

	virtual bool initialize() = 0;
	virtual bool shutdown() = 0;
};

#endif