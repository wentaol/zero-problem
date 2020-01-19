#include "illustration.h"
#include "log_file.h"

#include <sstream>
#include <iomanip>
#include <map>

std::string getLatexMoves(const Board& board,
	const std::vector<Move>& moves,
	const Color& lastMoveColor);

void Illustration::clear()
{
	board.clear();
	position = 0;
	referencePosition = 0;
	for (int i = 0; i < 2; ++i) {
		referenceBoards[i].clear();
		winrate[i] = 0.0;
		moves[i].clear();
	}
}

std::string Illustration::latex(const std::string& id)
{
	std::ostringstream o;
	std::string tmp = metadata;
	std::replace(tmp.begin(), tmp.end(), '\\', '/');

	const auto& lastMove = board.lastMove;
	const auto& lastMoveColor = lastMove.color;

	std::vector<std::string> blackstones, whitestones;
	for (int row = 1; row <= board.size; ++row) {
		for (int column = 1; column <= board.size; ++column) {
			if (row == lastMove.coord.row && column == lastMove.coord.column)
				continue;
			Color c = board.stones[row - 1][column - 1];
			Coordinate coord;
			coord.column = column;
			coord.row = row;
			if (c == Color::BLACK) {
				blackstones.push_back(coord.texStr(board.size));
			}
			else if (c == Color::WHITE) {
				whitestones.push_back(coord.texStr(board.size));
			}
		}
	}

	// Metadata
	o << "\\begin{verbatim}" << std::endl;
	o << tmp << "    ";
	o << std::fixed << std::setprecision(2) << winrate[0] << ", ";
	o << std::fixed << std::setprecision(2) << winrate[1] << std::endl;
	o << "\\end{verbatim}" << std::endl;
	o << "\\begin{multicols}{3}" << std::endl;
	//
	// Goban 1
	//
	o << "\\begin{center}" << std::endl;
	o << "\\cleargoban" << std::endl;
	o << "\\white{";
	for (auto it = whitestones.begin(); it != whitestones.end(); ++it)
	{
		o << *it;
		if(std::next(it) != whitestones.end())
			o << ",";
	}
	o << "}" << std::endl;
	o << "\\black{";
	for (auto it = blackstones.begin(); it != blackstones.end(); ++it)
	{
		o << *it;
		if (std::next(it) != blackstones.end())
			o << ",";
	}
	o << "}" << std::endl;
	// last move
	if (lastMoveColor == Color::WHITE)
		o << "\\white[\\igotriangle]{";
	else
		o << "\\black[\\igotriangle]{";
	o << lastMove.coord.texStr(board.size)<< "}" << std::endl;
	o << "\\copytogoban{2}" << std::endl;
	o << "\\copytogoban{3}" << std::endl;
	o << "\\showfullgoban" << std::endl;
	o << "\\end{center}" << std::endl;
	o << "\\columnbreak" << std::endl;
	
	//
	// Goban 2
	//
	o << "\\begin{center}" << std::endl;
	o << "\\usegoban{2}" << std::endl;
	o << getLatexMoves(board, moves[0], lastMoveColor);
	o << "\\end{center}" << std::endl;
	o << "\\columnbreak" << std::endl;
	
	//
	// Goban 3
	//
	o << "\\begin{center}" << std::endl;
	o << "\\usegoban{3}" << std::endl;
	o << getLatexMoves(board, moves[1], lastMoveColor);
	o << "\\end{center}" << std::endl;
	o << "\\end{multicols}" << std::endl;
	return o.str();
}

std::string getLatexMoves(const Board& board, const std::vector<Move>& moves, const Color& lastMoveColor)
{
	std::ostringstream o;
	if (lastMoveColor == Color::WHITE)
		o << "\\black[1]{";
	else
		o << "\\white[1]{";
	std::map<std::string, std::vector<int>> movenums;
	int idx = 1;
	for (auto it = moves.begin(); it != moves.end(); ++it)
	{
		const auto movestr = (*it).coord.texStr(board.size);
		movenums[movestr].push_back(idx);
		o << movestr;
		if (std::next(it) != moves.end())
			o << ",";
		idx++;
	}
	o << "}" << std::endl;
	o << "\\showfullgoban" << std::endl;
	o << "\\\\";
	// Print overlapping stones
	for (auto it = movenums.begin(); it != movenums.end(); ++it)
	{
		if (it->second.size() > 1)
		{			
			o << "{\\small ";
			for (auto moveit = it->second.begin(); moveit != it->second.end(); ++moveit)
			{
				auto movenum = (*moveit);
				if ((lastMoveColor == Color::WHITE && movenum % 2 == 1)
					|| (lastMoveColor == Color::BLACK && movenum % 2 == 0))
				{
					o << "\\blackstone[" << movenum << "]";
				}
				else
				{
					o << "\\whitestone[" << movenum << "]";
				}
				if (std::next(moveit) != it->second.end())
					o << " = ";
				else
					o << "}  " << std::endl;
			}
		}
	}
	return o.str();
}

std::string Illustration::json(const std::string& id)
{
	std::ostringstream o;
	const int boardSize = board.size;
	if (boardSize == 0) {
		return std::string();
	}

	const auto& lastMove = board.lastMove;
	const std::string lastMoveCoord = lastMove.coord.isPass() ? "--" : lastMove.coord.sgfStr();
	const char nextMoveColor = lastMove.color == Color::BLACK ? 'W' : 'B';
	
	std::string tmp = metadata;
	std::replace(tmp.begin(), tmp.end(), '\\', '/');

	o << "\t\t{\n";
	o << "\t\t\tmetadata: '" << tmp << "',\n";
	o << "\t\t\tid: '" << id << "',\n";
	o << "\t\t\tsz: " << boardSize << ",\n";
	o << "\t\t\tps: " << (position + 1) << ",\n";
	o << "\t\t\twr: [ " << std::fixed << std::setprecision(2) << winrate[0] << ", ";
	o << std::fixed << std::setprecision(2) << winrate[1] << " ],\n";
	o << "\t\t\tlm: '" << lastMoveCoord << "',\n";
	o << "\t\t\tcl: '" << nextMoveColor << "',\n";

	o << "\t\t\tst: '";
	std::vector<int> v;
	const auto& stones = board.stones;
	for (int row = 1; row <= boardSize; ++row) {
		for (int column = 1; column <= boardSize; ++column) {
			Color c = stones[row - 1][column - 1];
			if (c == Color::BLACK) {
				v.push_back(1);
			} else if (c == Color::WHITE) {
				v.push_back(2);
			} else {
				v.push_back(0);
			}
		}
	}
	while (v.size() % 3U) {
		v.push_back(0);
	}
	const int size = static_cast<int>(v.size());
	for (int i = 0; i < size; i += 3) {
		int a = 0x40 + v[i + 2] * 9 + v[i + 1] * 3 + v[i];
		o << static_cast<char>(a);
	}
	o << "',\n";

	o << "\t\t\tva: [ '";
	for (const auto& m : moves[0]) {
		const auto s = m.coord.sgfStr();
		if (s == "") {
			o << "--";
		} else {
			o << s;
		}
	}
	o << "', '";
	for (const auto& m : moves[1]) {
		const auto s = m.coord.sgfStr();
		if (s == "") {
			o << "--";
		} else {
			o << s;
		}
	}
	o << "' ]\n";

	o << "\t\t}";
	return o.str();
}
