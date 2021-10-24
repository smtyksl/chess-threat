#ifndef GAMEENGINE_H
#define GAMEENGINE_H
#include <QPoint>
#include <QVector>
class CheessPiece{
public:
	enum PieceColor {
		WHITE = 0,
		BLACK = 1
	};
	QString name;
	PieceColor color;
	QPoint point;
	bool underThreat = false;
};

class GameEngine
{
public:
	GameEngine();
	void startGame();
	QStringList separateLine(QString line);
	QPair<QString, CheessPiece::PieceColor>findNameColor(QString piece);

protected:
	bool readFile();
	QVector<CheessPiece *> blacks;
	QVector<CheessPiece *> whites;
	QVector<CheessPiece *> all;
	CheessPiece * board[8][8];
};

#endif // GAMEENGINE_H
