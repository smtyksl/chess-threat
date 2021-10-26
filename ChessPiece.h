#ifndef _CHESS_PIECE_H_
#define _CHESS_PIECE_H_

#include <memory>

#include "Common.h"

struct ChessPiece {
	ChessPiece() = default;
	ChessPiece(Coordinate const& coordinate, chess::PieceType pieceType, chess::PieceColor color)
		: position(coordinate)
		, pieceType(pieceType)
		, color(color)
	{}
	Coordinate position;
	chess::PieceType pieceType;
	chess::PieceColor color;
};

class ChessPieceFactory {
public:
	static ChessPiece * create(std::string const& serialized, int x, int y) {
		if (serialized == "--") {
			return nullptr;
		} else {
			return new ChessPiece(Coordinate{x,y}, chess::ToPieceType.at(serialized[0]), chess::ToColor.at(serialized[1]));
		}
	}
};

#endif //_CHESS_PIECE_H_

