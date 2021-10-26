#ifndef _BOARD_H_
#define _BOARD_H_

#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

#include "Common.h"
#include "ChessPiece.h"

class Board {
public:
	Board(std::string const& filename) {
		createEmptyBoard();
		std::ifstream file;
		file.open(filename);
		if (file.good()) {
			std::string line;
			int row = 0;
			while (std::getline(file, line)) {
				if(line.back() == '\r')
					line.pop_back();
				auto begin = line.begin();
				auto end = begin;
				int column = 0;
				do {
					end = std::find(begin, line.end(), ' ');
					boardState[column][row] = ChessPieceFactory::create(std::string{begin, end}, column, row);
					if (end != line.end()) {
						begin = end + 1;
					}
					++column;
				} while (end != line.end());
				++row;
			}
		}
		file.close();
	}

	ChessPiece* const operator()(int i, int j) const {
		return boardState[i][j];
	}

	ChessPiece* const operator[](Coordinate const& coordinate) const {
		return boardState[coordinate.x][coordinate.y];
	}
private:
	std::vector<std::vector<ChessPiece *>> boardState;
	void createEmptyBoard() {
		for (int i = 0; i < 8; ++i) {
			std::vector<ChessPiece *> column;
			for (int j = 0; j < 8; ++j) {
				column.push_back(nullptr);
			}
			boardState.push_back(std::move(column));
		}
	}
};

#endif //_BOARD_H_
