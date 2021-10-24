#include "gameengine.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

GameEngine::GameEngine()
{
	for (int var = 0; var < 8; ++var)
		for (int var2 = 0; var2 < 8; ++var2)
			board[var][var2] = nullptr;
	piecePoint["p"]=1;
	piecePoint["a"]=3;
	piecePoint["f"]=3;
	piecePoint["k"]=5;
	piecePoint["v"]=9;
	piecePoint["s"]=100;
}

void GameEngine::startGame()
{
	readFile();
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			isUnderThreat(board[i][j]);
			calculatePiecePoint(board[i][j]);
		}
	}
	calculateTotalPoints();
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

bool GameEngine::isUnderThreat(CheessPiece *piece)
{
	if(piece) {
		for(size_t i = 0; i< 8; i++) {
			for(size_t j = 0; j< 8; j++) {
				if(piece->coordinate.x() == (int)i && piece->coordinate.y() == (int)j)
					continue;
				else {
					checkDiagonalThreats(piece);
				}

			}
		}
		return true;
	}
}

//Turkish Satranc Shah Vezir Kale Fil    At     Biyon
//English Chess   King Queen Rook Bishop Knight Pawn
/*
	check king, queen, bishop, pwan
*/
//{	if(board[piece->point.x() -1 ][piece->point.y() -1 ]->name == "p" || board[piece->point.x() -1 ][piece->point.y() -1 ]->name == "s")
bool GameEngine::checkDiagonalThreats(CheessPiece *piece)
{
	// top-left
	for(int i = piece->coordinate.x() -1 ; i > -1 ; i--) {
		for(int j = piece->coordinate.x() -1 ; j > -1 ; j--) {
			if(board[i][j]) {
				if( board[i][j]->color != piece->color) {
					if ((board[i][j]->name  == "s" || board[i][j]->name  == "p") && (i == piece->coordinate.x() -1 && j == piece->coordinate.y() -1 ) ) // şah ve piyon un ilk çaprazda olma durumu
						return true;
					else if(board[i][j]->name  == "v" || board[i][j]->name  == "f")
						return true;
					else
						return false;
				}
				else if (board[i][j]->color == piece->color) {
					return false;
				}
			}
		}
	}
	// top right
	for(int i = piece->coordinate.x() +1 ; i < 8 ; i++) {
		for(int j = piece->coordinate.x() -1 ; j > -1 ; j--) {
			if(board[i][j]) {
				if( board[i][j]->color != piece->color) {
					if ((board[i][j]->name  == "s" || board[i][j]->name  == "p") && (i == piece->coordinate.x() +1 && j == piece->coordinate.y() +1 ) ) // şah ve piyon un ilk çaprazda olma durumu
						return true;
					else if(board[i][j]->name  == "v" || board[i][j]->name  == "f")
						return true;
					else
						return false;
				}
				else if (board[i][j]->color == piece->color) {
					return false;
				}
			}
		}
	}

	//buttom left
	for(int i = piece->coordinate.x() -1 ; i > -1 ; i--) {
		for(int j = piece->coordinate.x() +1 ; j < 8 ; j++) {
			if(board[i][j]) {
				if( board[i][j]->color != piece->color) {
					if ((board[i][j]->name  == "s" || board[i][j]->name  == "p") && (i == piece->coordinate.x() -1 && j == piece->coordinate.y() +1 ) ) // şah ve piyon un ilk çaprazda olma durumu
						return true;
					else if(board[i][j]->name  == "v" || board[i][j]->name  == "f")
						return true;
					else
						return false;
				}
			}
			else if (board[i][j]->color == piece->color) {
				return false;
			}
		}
	}

	//buttom right
	//buttom left
	for(int i = piece->coordinate.x() +1 ; i < 8 ; i++) {
		for(int j = piece->coordinate.x() +1 ; j < 8 ; j++) {
			if(board[i][j]) {
				if( board[i][j]->color != piece->color) {
					if ((board[i][j]->name  == "s" || board[i][j]->name  == "p") && (i == piece->coordinate.x() +1 && j == piece->coordinate.y() +1 ) ) // şah ve piyon un ilk çaprazda olma durumu
						return true;
					else if(board[i][j]->name  == "v" || board[i][j]->name  == "f")
						return true;
					else
						return false;
				}
				else if (board[i][j]->color == piece->color) {
					return false;
				}
			}
		}
	}
}

bool GameEngine::checkLinarThreats(CheessPiece *piece)
{
	{
		//	left
		auto j = piece->coordinate.y();
		for(int i = piece->coordinate.x() -1 ; i > -1 ; i--) {
			if(board[i][j]) {
				if( board[i][j]->color != piece->color) {
					if ((board[i][j]->name  == "s") && i == piece->coordinate.x() -1  ) // şah in bir birim yanda olma durumu
						return true;
					else if(board[i][j]->name  == "v" || board[i][j]->name  == "k")
						return true;
					else
						return false;
				}
			}
			else if (board[i][j]->color == piece->color) {
				return false;
			}
		}
	}
	{
		//	right
		auto j = piece->coordinate.y();
		for(int i = piece->coordinate.x() +1 ; i < 8 ; i++) {
			if(board[i][j]) {
				if( board[i][j]->color != piece->color) {
					if ((board[i][j]->name  == "s") && i == piece->coordinate.x() +1  ) // şah in bir birim yanda olma durumu
						return true;
					else if(board[i][j]->name  == "v" || board[i][j]->name  == "k")
						return true;
					else
						return false;
				}
			}
			else if (board[i][j]->color == piece->color) {
				return false;
			}
		}
	}
	{
		//	down
		auto i = piece->coordinate.x();
		for(int j = piece->coordinate.y() +1 ; j < 8 ; j++) {
			if(board[i][j]) {
				if( board[i][j]->color != piece->color) {
					if ((board[i][j]->name  == "s") && j == piece->coordinate.y() +1  ) // şah in bir birim yanda olma durumu
						return true;
					else if(board[i][j]->name  == "v" || board[i][j]->name  == "k")
						return true;
					else
						return false;
				}
			}
			else if (board[i][j]->color == piece->color) {
				return false;
			}
		}
	}
	{
		//	down
		auto i = piece->coordinate.x();
		for(int j = piece->coordinate.y() -1 ; j > -1 ; j++) {
			if(board[i][j]) {
				if( board[i][j]->color != piece->color) {
					if ((board[i][j]->name  == "s") && j == piece->coordinate.y() -1  ) // şah in bir birim yanda olma durumu
						return true;
					else if(board[i][j]->name  == "v" || board[i][j]->name  == "k")
						return true;
					else
						return false;
				}
			}
			else if (board[i][j]->color == piece->color) {
				return false;
			}
		}
	}
}

void GameEngine::calculatePiecePoint(CheessPiece *piece)
{
	if(piece) {
		std::map<std::string,int>::iterator it;
		it = piecePoint.find(piece->name.toStdString());
		piece->point = it->second;
		if( piece->underThreat)
			piece->point /= 2;
	}
}

void GameEngine::calculateTotalPoints()
{
	int totalBlack=0;
	int totalWhite=0;
	for (int i = 0; i < 8; ++i) {
		for (int j= 0; j < 8; ++j) {
			if(board[i][j]) {
				if(board[i][j]->color ==CheessPiece::BLACK) {
					totalBlack += board[i][j]->point;
				}
				else if(board[i][j]->color ==CheessPiece::WHITE) {
					totalWhite += board[i][j]->point;
				}
			}
		}
	}
	qDebug()<< "black : " << totalBlack;
	qDebug()<< "white : " << totalWhite;
}

bool GameEngine::readFile()
{
	QFile f("/home/samet/myfs/codes/Chess/chess-threat/board3.txt");
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
					piece->coordinate.setX(i);
					piece->coordinate.setY(j);
					board[i][j] = piece;
				}
			}
			i++;
		}
		f.close();
		QString aa = "";
		qDebug() << "=============================================================";
		for (int var = 0; var < 8; ++var) {
			for (int var2 = 0; var2 < 8; ++var2) {
				if(board[var][var2] != nullptr)
					aa +=board[var][var2]->name +( board[var][var2]->color == CheessPiece::BLACK ? "s" : "b") +" ";
				else
					aa +="-- ";
			}

			qDebug() << "" << aa;
			aa = "";
		}


	}
	else
		return  false;
	return true;
}
