#include "gameengine.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

GameEngine::GameEngine()
{
	for (int var = 0; var < 8; ++var)
		for (int var2 = 0; var2 < 8; ++var2)
			board[var][var2] = nullptr;
}

void GameEngine::startGame()
{
	readFile();
}

QStringList GameEngine::separateLine(QString line)
{
	QStringList l =  line.split(" ");
	return l;
}

QPair<QString, CheessPiece::PieceColor> GameEngine::findNameColor(QString piece)
{
	QPair<QString, CheessPiece::PieceColor> retVal;
	if(piece.size() == 2) {
		retVal.first = piece.at(0);
		if( piece.at(1) == "s")
			retVal.second = CheessPiece::BLACK;
		else if (piece.at(1) == "b")
			retVal.second = CheessPiece::WHITE;


	} else {
		retVal.first = "u";
		retVal.second = CheessPiece::BLACK;
	}
	return retVal;
}

bool GameEngine::readFile()
{
	QFile f("/home/samet/myfs/codes/Chess/chess-threat/board1.txt");
	if (f.open(QIODevice::ReadOnly))
	{
		QTextStream in(&f);
		int i = 0;
		while (!in.atEnd())
		{
			QString line = in.readLine();
			qDebug() << " line:  "<< line;
			auto l = separateLine(line);
			for(size_t j = 0 ; j < l.size() ; j++) {

				if( l.at(j) != "--") {
					CheessPiece * piece = new CheessPiece;
					auto nameColor =findNameColor(l.at(j));
					piece->name = nameColor.first;
					piece->color = nameColor.second;
					piece->point.setX(i);
					piece->point.setY(j);
					board[i][j] = piece;
					if(piece->color ==CheessPiece::BLACK) {
						blacks.push_back(piece);
					} else
						whites.push_back(piece);
				}
			}
			i++;
		}
		f.close();
		qDebug() << "=============================================================";
		for (int var = 0; var < 8; ++var) {
			for (int var2 = 0; var2 < 8; ++var2) {
				if(board[var][var2] != nullptr) {
					qDebug() << board[var][var2]->name <<  ( board[var][var2]->color == CheessPiece::BLACK ? "s" : "b");
			}
		}

		}
	}
	else
		return  false;
	return true;
}
