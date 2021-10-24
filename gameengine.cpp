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
			if(board[i][j]) {
			isUnderThreat(board[i][j]);
			calculatePiecePoint(board[i][j]);
			}
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
	if(chechThreatFromKnight(piece) || chechThreatFromPawn(piece) || chechThreatFromQueen(piece))
		return true;
	return false;
}
//Turkish Satranc Shah Vezir Kale Fil    At     Biyon
//English Chess   King Queen Rook Bishop Knight Pawn

void GameEngine::calculatePiecePoint(CheessPiece *piece)
{
	if(piece) {
		std::map<std::string,int>::iterator it;
		it = piecePoint.find(piece->name.toStdString());
		piece->point = it->second;

		if( piece->underThreat)
			piece->point /= 2;
		qDebug() <<piece->name <<" "<< piece->point <<"Color: "<<(piece->color == CheessPiece::BLACK ? "black" :"white" );
	}
}

bool GameEngine::chechThreatFromQueen(CheessPiece *piece)
{
	auto queen = findQueen( (piece->color == CheessPiece::BLACK ? CheessPiece::WHITE : CheessPiece::BLACK));
	auto x = piece->coordinate.x();
	auto y = piece->coordinate.y();
	if( std::abs(x- queen->coordinate.x()) == std::abs(y- queen->coordinate.y())) {// queen at piece diagonally
		if( queen->coordinate.x() < x && queen->coordinate.y() < y ) {//queen at topleft
			for(int i = x ; i > queen->coordinate.x() ; i-- )
				for(int j = y ; j > queen->coordinate.y() ; j-- )
					if(board[i][j])
						return true;
		}
		else if( queen->coordinate.x() > x && queen->coordinate.y() < y ) {//queen at topright
			for(int i = x ; i < queen->coordinate.x() ; i++)
				for(int j = y ; j > queen->coordinate.y() ; j-- )
					if(board[i][j])
						return true;
		}
		else if( queen->coordinate.x() < x && queen->coordinate.y() > y ) {//queen at buttleft
			for(int i = x ; i < queen->coordinate.x() ; i-- )
				for(int j = y ; j > queen->coordinate.y() ; j++ )
					if(board[i][j])
						return true;
		}
		else if( queen->coordinate.x() > x && queen->coordinate.y() > y ) {//queen at topleft
			for(int i = x ; i > queen->coordinate.x() ; i++ )
				for(int j = y ; j >queen->coordinate.y() ; j++ )
					if(board[i][j])
						return true;
		}else
			return false;

	}
	else if(queen->coordinate.x() == piece->coordinate.x()) {// aynı satırda
		if(queen->coordinate.y() > piece->coordinate.y()) {
			for(int i = queen->coordinate.y() ; i > piece->coordinate.y(); i--)
				if(board[piece->coordinate.x()][i]) {
					return true;
				}
				else if ( queen->coordinate.y() < piece->coordinate.y()) {
					for(int i = queen->coordinate.y() ; i > piece->coordinate.y(); i--)
						if(board[piece->coordinate.x()][i])
							return true;
				}

		} else if(queen->coordinate.y() == piece->coordinate.y()) {// aynı sutunda
			if(queen->coordinate.x() > piece->coordinate.x()) {
				for(int i = queen->coordinate.x() ; i > piece->coordinate.x(); i--)
					if(board[i][piece->coordinate.y()]) {
						return true;
					}
					else if ( queen->coordinate.x() < piece->coordinate.x()) {
						for(int i = queen->coordinate.x() ; i > piece->coordinate.x(); i--)
							if(board[i][piece->coordinate.y()])
								return true;
					}
			}
		}

	}
	return false;
}

CheessPiece *GameEngine::findQueen(CheessPiece::PieceColor ColorQueen)
{
	for(int i = 0 ; i< 8 ; i++)
		for(int j = 0 ; j< 8 ; j++) {
			if(board[i][j])
				if( board[i][j]->color == ColorQueen && board[i][j]->name == "v")
					return board[i][j];
		}
	return nullptr;
}

bool GameEngine::chechThreatFromKnight(CheessPiece *piece)
{
	auto x = piece->coordinate.x();
	auto y = piece->coordinate.y();
	if(piece->color == CheessPiece::WHITE) {
		// check possibilities
		if( board[x-1][y-2]) {
			if(board[x-1][y-2]->color == CheessPiece::BLACK && board[x-1][y-2]->name == "a") {
				return true;
			}
		}
		else if( board[x-2][y-1]) {
			if(board[x-2][y-1]->color == CheessPiece::BLACK && board[x-2][y-1]->name == "a") {
				return true;
			}
		} else if( 0 && board[x+2][y+1]) {
			if(board[x+2][y+1]->color == CheessPiece::BLACK && board[x+2][y+1]->name == "a") {
				return true;
			}
		} else if( 0&& board[x+1][y+2]) {
			if(board[x+1][y+2]->color == CheessPiece::BLACK && board[x+1][y+2]->name == "a") {
				return true;
			}
		}

	} else if(piece->color == CheessPiece::BLACK) {
		// check possibilities
		if(0 && board[x-1][y-2]) {
			if(board[x-1][y-2]->color == CheessPiece::WHITE && board[x-1][y-2]->name == "a") {
				return true;
			}
		}
		else if(0 &&  board[x-2][y-1]) {
			if(board[x-2][y-1]->color == CheessPiece::WHITE && board[x-2][y-1]->name == "a") {
				return true;
			}
		} else if( 0 && board[x+2][y+1]) {
			if(board[x+2][y+1]->color == CheessPiece::WHITE && board[x+2][y+1]->name == "a") {
				return true;
			}
		} else if( 0 && board[x+1][y+2]) {
			if(board[x+1][y+2]->color == CheessPiece::WHITE && board[x+1][y+2]->name == "a") {
				return true;
			}
		}
	}
	return false;
}

bool GameEngine::chechThreatFromPawn(CheessPiece *piece)
{
	auto x = piece->coordinate.x();
	auto y = piece->coordinate.y();
	if(piece->color == CheessPiece::WHITE) {
		if( board[x+1][y-1]) { // check right top
			if(board[x+1][y-1]->color ==  CheessPiece::BLACK && board[x+1][y-1]->name == "p")
				return true;
		} else if (board[x-1][y-1]) { //check left top
			if(board[x-1][y-1]->color ==  CheessPiece::BLACK && board[x-1][y-1]->name == "p")
				return true;
		} else
			return  false;
	} else 	if(piece->color == CheessPiece::BLACK) {
		if( board[x+1][y+1]) { // check right buttom
			if(board[x+1][y+1]->color ==  CheessPiece::WHITE && board[x+1][y+1]->name == "p")
				return true;
		} else if (0 && board[x-1][y+1] ) { //check left buttom
			if(board[x-1][y+1]->color ==  CheessPiece::WHITE && board[x-1][y+1]->name == "p")
				return true;
		} else
			return  false;
	}
	return false;
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
