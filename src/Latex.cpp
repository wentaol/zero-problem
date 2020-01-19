#include "Latex.h"
#include <sstream>

std::string GetLatex(const std::string& title, const std::vector<std::string>& problems, int boardSize)
{
	std::ostringstream o;

	o << "\\documentclass{article}" << std::endl;
	o << "\\usepackage{igo}" << std::endl;
	o << "\\usepackage[a4paper, margin = 2cm, landscape]{geometry}" << std::endl;
	o << "\\usepackage{multicol}" << std::endl;
	o << std::endl;
	o << "\\begin{document}" << std::endl;
	o << "\\igofontsize{9}" << std::endl;	
	bool even = false;
	for (auto& problem : problems)
	{
		o << problem;
		//o << "\\\\" << std::endl;
		o << std::endl;
		// 2 rows per page		
		if (even)
		{
			o << "\\newpage" << std::endl;
		}
		even = !even;		
	}
	o << "\\end{document}" << std::endl;
	return o.str();
}
