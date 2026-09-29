#pragma once

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
	std::vector<SelectionRange> m_selectedRanges;
	int m_anchor = -1;
	int m_cursor = -1;
public:
	void AddSingleSelection(const int& ind)
	{
		const int size = static_cast<int>(m_selectedRanges.size());
		if (ind < 0) return;
		// Check if it already exist
		for (const SelectionRange& range : m_selectedRanges)
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
		auto it = std::upper_bound(m_selectedRanges.begin(), m_selectedRanges.end(), ind,
			[](int val, const SelectionRange& range)
			{
				return val < range.first;
			}
		);
		const int positionToInsertIn = std::distance(m_selectedRanges.begin(), it);

		//           |
		//           V
		// [1,2] , [3,3] , [4,7]
		if ((positionToInsertIn - 1 >= 0 && positionToInsertIn <= size - 1) &&
			(m_selectedRanges[positionToInsertIn - 1].last - ind == -1 && m_selectedRanges[positionToInsertIn].first - ind == 1))
		{
			m_selectedRanges[positionToInsertIn - 1].last = m_selectedRanges[positionToInsertIn].last;
			m_selectedRanges.erase(it);
		}
		//           |
		//           V
		// [1,2] , [3,3]
		else if (positionToInsertIn - 1 >= 0 && m_selectedRanges[positionToInsertIn - 1].last - ind == -1)
		{
			m_selectedRanges[positionToInsertIn - 1].last = ind;
		}
		//   |
		//   V
		// [3,3] , [4,7]
		else if (positionToInsertIn <= size - 1 && m_selectedRanges[positionToInsertIn].first - ind == 1)
		{
			m_selectedRanges[positionToInsertIn].first = ind;
		}
		else
		{
			m_selectedRanges.insert(it, SelectionRange(ind, ind));
		}
	}

	void toggleSingleSelection(const int& ind)
	{
		const int size = static_cast<int>(m_selectedRanges.size());
		if (ind < 0) return;
		// Check if it already exist
		for (size_t i = 0; i < size; i++)
		{
			SelectionRange& range = m_selectedRanges[i];
			if (ind >= range.first && ind <= range.last)
			{
				if (ind == range.first && ind == range.last)
				{
					m_selectedRanges.erase(m_selectedRanges.begin() + i);
				}
				else if (ind == range.first)
				{
					range.first++;
				}
				else if (ind == range.last)
				{
					range.last--;
				}
				else if (ind > range.first && ind < range.last)
				{
					const int oldLast = range.last;
					m_selectedRanges.insert(m_selectedRanges.begin() + i + 1, SelectionRange(ind + 1, oldLast));
					m_selectedRanges[i].last = ind - 1;
				}
				return;
			}
		}
		auto it = std::upper_bound(m_selectedRanges.begin(), m_selectedRanges.end(), ind,
			[](int val, const SelectionRange& range)
			{
				return val < range.first;
			}
		);
		const int positionToInsertIn = std::distance(m_selectedRanges.begin(), it);

		//           |
		//           V
		// [1,2] , [3,3] , [4,7]
		if ((positionToInsertIn - 1 >= 0 && positionToInsertIn <= size - 1) &&
			(m_selectedRanges[positionToInsertIn - 1].last - ind == -1 && m_selectedRanges[positionToInsertIn].first - ind == 1))
		{
			m_selectedRanges[positionToInsertIn - 1].last = m_selectedRanges[positionToInsertIn].last;
			m_selectedRanges.erase(it);
		}
		//           |
		//           V
		// [1,2] , [3,3]
		else if (positionToInsertIn - 1 >= 0 && m_selectedRanges[positionToInsertIn - 1].last - ind == -1)
		{
			m_selectedRanges[positionToInsertIn - 1].last = ind;
		}
		//   |
		//   V
		// [3,3] , [4,7]
		else if (positionToInsertIn <= size - 1 && m_selectedRanges[positionToInsertIn].first - ind == 1)
		{
			m_selectedRanges[positionToInsertIn].first = ind;
		}
		else
		{
			m_selectedRanges.insert(it, SelectionRange(ind, ind));
		}
	}

	void SelectRange(int start, int end)
	{
		if (start > end) std::swap(start, end);
		Clear();
		m_selectedRanges.emplace_back(SelectionRange(start, end));
	}

	void SelectAnchoredRange(int end)
	{
		int anchor;
		if (m_anchor < 0) anchor = 0;
		else 
		{
			anchor = m_anchor;
		}
		if (anchor > end) std::swap(anchor, end);
		Clear();
		m_selectedRanges.emplace_back(SelectionRange(anchor, end));
	}

	void AddRange(int start, int end)
	{
		if (start > end) std::swap(start, end);

		// first range that overlap/touch [start, end]
		auto firstIt = std::find_if(m_selectedRanges.begin(), m_selectedRanges.end(),
			[&start](const SelectionRange& range) { return start <= range.last + 1; });

		// first range that starts after end
		auto secondIt = std::find_if(firstIt, m_selectedRanges.end(),
			[&end](const SelectionRange& range) { return end < range.first - 1; });

		if (firstIt == secondIt)
		{
			m_selectedRanges.emplace(firstIt, SelectionRange(start, end));
			return;
		}

		// merge everything in [firstIt, secondIt) into one range
		firstIt->first = (std::min)(firstIt->first, start);
		firstIt->last = (std::max)(std::prev(secondIt)->last, end);
		m_selectedRanges.erase(firstIt + 1, secondIt);
	}

	void ToggleRange(int start, int end)
	{
		if (start > end) std::swap(start, end);

		auto startIt = std::lower_bound(
			m_selectedRanges.begin(), m_selectedRanges.end(), start,
			[](const SelectionRange& range, int val) { return range.last < val; }
		);

		auto endIt = std::upper_bound(
			startIt, m_selectedRanges.end(), end,
			[](int val, const SelectionRange& range) { return val < range.first; }
		);

		if (startIt == endIt)
		{
			m_selectedRanges.insert(startIt, SelectionRange(start, end));
			return;
		}

		std::vector<SelectionRange> replacement;
		int current = start;

		for (auto it = startIt; it != endIt; ++it)
		{
			if (it->first < current)
			{
				replacement.emplace_back(it->first, current - 1);
			}
			else if (current < it->first)
			{
				replacement.emplace_back(current, it->first - 1);
			}

			current = it->last + 1;
		}

		if (current <= end)
		{
			replacement.emplace_back(current, end);
		}
		else if (end + 1 <= std::prev(endIt)->last)
		{
			replacement.emplace_back(end + 1, std::prev(endIt)->last);
		}

		auto eraseStart = m_selectedRanges.erase(startIt, endIt);
		m_selectedRanges.insert(eraseStart, replacement.begin(), replacement.end());
	}

	const bool IsSelected(int ind) const
	{
		int head = 0;
		int tail = static_cast<int>(m_selectedRanges.size()) - 1;

		while (head <= tail)
		{
			int mid = head + (tail - head) / 2;

			const auto& range = m_selectedRanges[mid];

			if (ind < range.first)
			{
				tail = mid - 1;
			}
			else if (ind > range.last)
			{
				head = mid + 1;
			}
			else
			{
				return true;
			}
		}

		return false;
	}

	const int Find(int ind) const 
	{
		int head = 0;
		int tail = static_cast<int>(m_selectedRanges.size()) - 1;

		while (head <= tail)
		{
			int mid = head + (tail - head) / 2;

			const auto& range = m_selectedRanges[mid];

			if (ind < range.first)
			{
				tail = mid - 1;
			}
			else if (ind > range.last)
			{
				head = mid + 1;
			}
			else
			{
				return mid;
			}
		}

		return -1;
	}

	const std::vector<SelectionRange>& GetRanges() const
	{
		return m_selectedRanges;
	}

	void SetAnchor(const int& NewAnchor) 
	{
		if (NewAnchor < 0)
		{
			m_anchor = -1;
			m_cursor = m_anchor;
			return;
		}
		m_anchor = NewAnchor;
		m_cursor = m_anchor;
	}

	void ResetAnchor() 
	{
		m_anchor = -1;
		m_cursor = -1;
		std::cout << "anchor reseted" << std::endl;
	}

	void SetCursor(const int& c) 
	{
		m_cursor = c; 
	}

	int GetCursor() const 
	{ 
		return m_cursor; 
	}

	const bool IsAnchor(const int val) const 
	{
		return val == m_anchor;
	}

	const bool IsCursor(const int val) const { return val == m_cursor; }

	const bool Empty() const
	{
		return m_selectedRanges.empty();
	}

	void SetRanges(const std::vector<SelectionRange>& ranges) 
	{
		m_selectedRanges = ranges;
	}

	const int GetCount() const
	{
		int count = 0;
		for (const SelectionRange& range : m_selectedRanges) 
		{
			count += (range.last - range.first) + 1;
		}
		return count;
	}

	const int GetAnchor() const
	{
		return m_anchor;
	}

	const int GetFirstInd() const 
	{
		if (m_selectedRanges.empty()) return -1;
		return m_selectedRanges[0].first;
	}

	const int GetLastInd() const
	{
		if (m_selectedRanges.empty()) return -1;
		return m_selectedRanges[static_cast<int>(m_selectedRanges.size()) - 1].last;
	}

	void Clear()  
	{
		m_selectedRanges.clear();
	}

	int Size() 
	{
		return static_cast<int>(m_selectedRanges.size());
	}

	void Reset() 
	{
		m_selectedRanges.clear();
		m_anchor = -1;
		m_cursor = -1;
	}

	void Print() 
	{
		std::cout << "######################################################\n"
			      << "################# Selected Ranges ####################\n"
				  << "######################################################\n";
		for (const auto& range : m_selectedRanges) 
		{
			std::cout << std::format("[{},{}]", range.first, range.last) << std::endl;
		}
		std::cout << "######################################################\n";
	}
};