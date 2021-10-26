#ifndef _COMMON_H_
#define _COMMON_H_

#include <unordered_map>

struct Coordinate {
    int x;
    int y;
};


Coordinate operator+(Coordinate const& lhs, Coordinate const& rhs) {
    return Coordinate{lhs.x + rhs.x, lhs.y + rhs.y};
}


namespace chess {
    enum class PieceType {
        PAWN,
        ROOK,
        KNIGHT,
        BISHOP,
        QUEEN,
        KING
    };

    enum class PieceColor {
        WHITE,
        BLACK
    };

    static const std::unordered_map<char, PieceColor> ToColor = {
        {'s', PieceColor::BLACK},
        {'b', PieceColor::WHITE},
    };

    static const std::unordered_map<char, PieceType> ToPieceType = {
        {'p', PieceType::PAWN},
        {'k', PieceType::ROOK},
        {'a', PieceType::KNIGHT},
        {'f', PieceType::BISHOP},
        {'v', PieceType::QUEEN},
        {'s', PieceType::KING},
    };

    static const std::unordered_map<PieceType, double> ToPoint = {
        {PieceType::PAWN, 1},
        {PieceType::ROOK, 5},
        {PieceType::KNIGHT, 3},
        {PieceType::BISHOP, 3},
        {PieceType::QUEEN, 9},
        {PieceType::KING, 100},
    };

}
#endif //_COMMON_H_