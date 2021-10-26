#ifndef _THREAT_EVALUATOR_H_
#define _THREAT_EVALUATOR_H_

#include <array>
#include <iostream>

#include "Board.h"

class ThreatEvaluator {
public:
	static void evaluate(Board const& board) {
		double whitePoints = 0;
		double blackPoints = 0;
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				auto piece = board(i, j);
				if (nullptr == piece)
					continue;
				auto& points = (chess::PieceColor::WHITE == piece->color ? whitePoints : blackPoints);
				if (ThreatEvaluator::isThreatenedByPawn(board, piece) || ThreatEvaluator::isThreatenedByKnight(board, piece) || ThreatEvaluator::isThreatenedByQueen(board, piece)) {
					std::cout << piece->position.x << " " << piece->position.y << std::endl;
					points += (chess::ToPoint.at(piece->pieceType) / 2.0);
				} else {
					points += chess::ToPoint.at(piece->pieceType);
				}
				std::cout << "whitePoints: " << whitePoints << " blackPoints: " << blackPoints << std::endl;
			}
		}
		std::cout << "Siyah: " << blackPoints << ", Beyaz: " << whitePoints << std::endl;
	}

private:
	static bool isInsideBoard(ChessPiece *const piece) {
		auto const& x = piece->position.x;
		auto const& y = piece->position.y;
		if (x < 0 || x > 7 || y < 0 || y > 7) {
			return false;
		} else {
			return true;
		}
	}

	static bool isInsideBoard(Coordinate const& coordinate) {
		if (coordinate.x < 0 || coordinate.x > 7 || coordinate.y < 0 || coordinate.y > 7) {
			return false;
		} else {
			return true;
		}
	}

	static const std::unordered_map<chess::PieceColor, std::array<Coordinate, 2>> PawnThreats;

	static const std::array<Coordinate, 8> KnightThreats;

	static const std::array<std::array<Coordinate, 7>, 8> QueenThreats;

	static ChessPiece* const getPieceRelativeTo(ChessPiece* const piece, Coordinate const& relativePosition, Board const& board) {
		return board[piece->position + relativePosition];
	}

	static bool isOppositeColor(ChessPiece *const lhs, ChessPiece *const rhs) {
		return lhs->color != rhs->color;
	}

	static bool isSameColor(ChessPiece *const lhs, ChessPiece *const rhs) {
		return lhs->color == rhs->color;
	}

	static bool isThreatenedByPawn(Board const& board, ChessPiece *const piece) {
		auto const threatPositions = ThreatEvaluator::PawnThreats.at(piece->color);
		for (auto const& possibleThreatRelativePosition : threatPositions) {
			if (ThreatEvaluator::isInsideBoard(piece->position + possibleThreatRelativePosition)) {
				auto maybePiece = ThreatEvaluator::getPieceRelativeTo(piece, possibleThreatRelativePosition, board);
				if (!maybePiece) {
					continue;
				} else if (ThreatEvaluator::isOppositeColor(piece, maybePiece) && maybePiece->pieceType == chess::PieceType::PAWN) {
					return true;
				}
			}
		}
		return false;
	}

	static bool isThreatenedByKnight(Board const& board, ChessPiece *const piece) {
		auto const threatPositions = ThreatEvaluator::KnightThreats;
		for (auto const& possibleThreatRelativePosition : threatPositions) {
			if (ThreatEvaluator::isInsideBoard(piece->position + possibleThreatRelativePosition)) {
				auto maybePiece = ThreatEvaluator::getPieceRelativeTo(piece, possibleThreatRelativePosition, board);
				if (!maybePiece) {
					continue;
				} else if (ThreatEvaluator::isOppositeColor(piece, maybePiece) && maybePiece->pieceType == chess::PieceType::KNIGHT) {
					return true;
				}
			}
		}
		return false;
	}


	static bool isThreatenedByQueen(Board const& board, ChessPiece *const piece) {
		for (auto const& possibleThreatsRelativeInOneDirection : ThreatEvaluator::QueenThreats) {
			for (auto const& possibleThreatRelativePosition : possibleThreatsRelativeInOneDirection) {
				if (!ThreatEvaluator::isInsideBoard(piece->position + possibleThreatRelativePosition))
					break;
				auto maybePiece = ThreatEvaluator::getPieceRelativeTo(piece, possibleThreatRelativePosition, board);
				if (!maybePiece){
					continue;
				}
				else if (ThreatEvaluator::isSameColor(maybePiece, piece)){
					break;
				}
				else if(maybePiece->pieceType == chess::PieceType::QUEEN) {
					return true;
				}
				break;
			}
		}
		return false;
	}
};

const std::unordered_map<chess::PieceColor, std::array<Coordinate, 2>> ThreatEvaluator::PawnThreats = {
	{chess::PieceColor::WHITE, {Coordinate{-1, -1}, Coordinate{1, -1}}},
	{chess::PieceColor::BLACK, {Coordinate{-1, 1}, Coordinate{1, 1}}},
};

const std::array<Coordinate, 8> ThreatEvaluator::KnightThreats{
	Coordinate{2,1},
	Coordinate{-2,1},
	Coordinate{2,-1},
	Coordinate{-2,-1},
	Coordinate{1,2},
	Coordinate{-1,2},
	Coordinate{1,-2},
	Coordinate{-1,-2},
};

const std::array<std::array<Coordinate, 7>, 8> ThreatEvaluator::QueenThreats{
	std::array<Coordinate, 7>{Coordinate{1, 0}, Coordinate{2, 0}, Coordinate{3, 0}, Coordinate{4, 0}, Coordinate{5, 0}, Coordinate{6, 0}, Coordinate{7, 0}},
	std::array<Coordinate, 7>{Coordinate{-1, 0}, Coordinate{-2, 0}, Coordinate{-3, 0}, Coordinate{-4, 0}, Coordinate{-5, 0}, Coordinate{-6, 0}, Coordinate{-7, 0}},
	std::array<Coordinate, 7>{Coordinate{0, 1}, Coordinate{0, 2}, Coordinate{0, 3}, Coordinate{0, 4}, Coordinate{0, 5}, Coordinate{0, 6}, Coordinate{0, 7}},
	std::array<Coordinate, 7>{Coordinate{0, -1}, Coordinate{0, -2}, Coordinate{0, -3}, Coordinate{0, -4}, Coordinate{0, -5}, Coordinate{0, -6}, Coordinate{0, -7}},
	std::array<Coordinate, 7>{Coordinate{1, 1}, Coordinate{2, 2}, Coordinate{3, 3}, Coordinate{4, 4}, Coordinate{5, 5}, Coordinate{6, 6}, Coordinate{7, 7}},
	std::array<Coordinate, 7>{Coordinate{1, -1}, Coordinate{2, -2}, Coordinate{3, -3}, Coordinate{4, -4}, Coordinate{5, -5}, Coordinate{6, -6}, Coordinate{7, -7}},
	std::array<Coordinate, 7>{Coordinate{-1, -1}, Coordinate{-2, -2}, Coordinate{-3, -3}, Coordinate{-4, -4}, Coordinate{-5, -5}, Coordinate{-6, -6}, Coordinate{-7, -7}},
	std::array<Coordinate, 7>{Coordinate{-1, 1}, Coordinate{-2, 2}, Coordinate{-3, 3}, Coordinate{-4, 4}, Coordinate{-5, 5}, Coordinate{-6, 6}, Coordinate{-7, 7}},
};

#endif//_THREAT_EVALUATOR_H_
