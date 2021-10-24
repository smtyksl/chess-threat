#ifndef GAMEENGINE_H
#define GAMEENGINE_H
#include <QPoint>
#include <QVector>
#include <map>
class CheessPiece{
public:
	enum PieceColor {
		WHITE = 0,
		BLACK = 1
	};
	QString name;
	PieceColor color;
	QPoint coordinate;
	float point;
	bool underThreat = false;
};

class GameEngine
{
public:
	GameEngine();
	void startGame();
	QStringList separateLine(QString line);
	QPair<QString, CheessPiece::PieceColor>findNameColor(QString piece);
	bool isUnderThreat(CheessPiece *piece);
	void calculateTotalPoints();
	void calculatePiecePoint(CheessPiece *piece);

	bool chechThreatFromQueen(CheessPiece *piece);

	CheessPiece * findQueen(CheessPiece::PieceColor colorQueen);
	bool chechThreatFromKnight(CheessPiece *piece);
	bool chechThreatFromPawn(CheessPiece *piece);
protected:
	bool readFile();

	CheessPiece * board[8][8];
	std::map<std::string,int> piecePoint;
};

#endif // GAMEENGINE_H
