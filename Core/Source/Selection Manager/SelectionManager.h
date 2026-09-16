#pragma once
#include "pchCore.h"

struct SelectionRange
{
	int first;
	int last;

	SelectionRange(const int& x, const int& y) : first(x), last(y) 
	{}
};

class SelectionManager 
{
private:
	std::vector<SelectionRange> m_selectedRanegs;

public:
	void AddSingleSelection(const int& ind) 
	{
		if (ind < 0) return;
		for (const SelectionRange& range : m_selectedRanegs) 
		{
			if (range.first == ind || range.last == ind) 
			{
				return;
			}
			if (ind < range.last && ind > range.first) 
			{
				return;
			}
		}
		bool needFix = false;
		if (!m_selectedRanegs.empty()) 
		{
			if (ind < m_selectedRanegs.back().first) needFix = true;
		}
		m_selectedRanegs.emplace_back(SelectionRange(ind, ind));
		if (static_cast<int>(m_selectedRanegs.size()) == 1) return;

		if (needFix) 
		{
			std::sort(m_selectedRanegs.begin(), m_selectedRanegs.end(), [](const SelectionRange& a, const SelectionRange& b)
				{
					return ((a.first + a.last) / 2) < ((b.first + b.last) / 2);
				}
			);
		}
		Print();
	}

	void Clear() 
	{
		m_selectedRanegs.clear();
	}

	void Print() 
	{
		std::cout << "######################################################\n"
			      << "################# Selected Ranges ####################\n"
				  << "######################################################\n";
		for (const auto& range : m_selectedRanegs) 
		{
			std::cout << std::format("[{},{}]", range.first, range.last) << std::endl;
		}
		std::cout << "######################################################\n";
	}
};