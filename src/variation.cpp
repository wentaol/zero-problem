#include "variation.h"
#include "log_file.h"

#include <boost/regex.hpp>

static const boost::regex reVariation_16(
	R"( *(\w++) -> +(\d++)\D++(\d++\.\d++)\D++(\d++\.\d++)\W++PV\W++(.++))");
static const boost::regex reVariation_17(
	R"( *(\w++) -> +(\d++)\D++(\d++\.\d++)\D++(\d++\.\d++)\D++(\d++\.\d++)\W++PV\W++(.++))");
static const boost::regex reCoordinate(R"(\w++)");

bool Variation::create(GtpLz& gtp, Color color, int boardSize)
{
	boost::smatch what;
	const auto line = gtp.getLine();
	bool matched = false;
	int idx_move, idx_playout, idx_v, idx_n, idx_coord;
	if (gtp.getVersion() == "0.16")
	{
		matched = boost::regex_match(line, what, reVariation_16);
		idx_move = 1;
		idx_playout = 2;
		idx_v = 3;
		idx_n = 4;
		idx_coord = 5;
	}
	else if (gtp.getVersion() == "0.17")
	{
		matched = boost::regex_match(line, what, reVariation_17);
		idx_move = 1;
		idx_playout = 2;
		idx_v = 3;
		idx_n = 5;
		idx_coord = 6;
	}

	if (matched) {
		move.color = color;
		move.coord.setFromStr(what[idx_move].str(), boardSize);
		playout = std::stoi(what[idx_playout].str());
		v = std::stod(what[idx_v].str());
		n = std::stod(what[idx_n].str());
		pv.clear();
		Move m;
		m.color = move.color;
		const auto coord = what[idx_coord].str();
		boost::sregex_iterator it(coord.begin(), coord.end(), reCoordinate);
		boost::sregex_iterator itEnd;
		while (it != itEnd) {
			m.coord.setFromStr((*it)[0].str(), boardSize);
			pv.push_back(m);
			if (m.color == Color::BLACK) {
				m.color = Color::WHITE;
			} else {
				m.color = Color::BLACK;
			}
			++it;
		}
		return true;
	}
	return false;
}
