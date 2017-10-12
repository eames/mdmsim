#include <vector>
#include <string>
#include <algorithm>

inline const std::vector<std::string>& get_element_vector()
{
	static const std::vector<std::string> elements = {
		"n", "H", "He", "Li", "Be", "B", "C", "N", "O", "F",
		"Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar",
		"K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co",
		"Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr",
		"Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh",
		"Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I", "Xe",
		"Cs", "Ba", "La", "Ce", "Pr", "Nd", "Pm", "Sm", "Eu",
		"Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf",
		"Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl",
		"Pb", "Bi", "Po", "At", "Rn", "Fr", "Ra", "Ac", "Th",
		"Pa", "U", "Np", "Pu", "Am", "Cm", "Bk", "Cf", "Es",
		"Fm", "Md", "No", "Lr", "Rf", "Db", "Sg", "Bh", "Hs",
		"Mt", "D", "Rg", "Cn", "Nh", "Fl", "Mc", "Lv", "Ts", "Og" 
	};
	return elements;
}


int get_element_z(const std::string& element)
{
	std::vector<std::string>::const_iterator i =
		std::find(get_element_vector().begin(), get_element_vector().end(), element);
	
	return i == get_element_vector().end() ?
		-1 : i - get_element_vector().begin();
}
