#include "HeaderVariable.h"

namespace KHAS {
	HeaderVariable::HeaderVariable(std::initializer_list<value_type>&& il)
		: arr_{ std::move(il) }
	{
	}

	size_t HeaderVariable::size() const noexcept
	{
		return arr_.size();
	}

	void HeaderVariable::print(size_t first_width, size_t other_width) const noexcept
	{
		std::cout << std::setw(first_width) << " ";
		for (auto&& elem : arr_)
			std::cout << std::setw(other_width) << elem << " ";
	}

	HeaderVariable::value_type HeaderVariable::getValue(size_t index) const noexcept
	{
		return arr_[index];
	}

	void HeaderVariable::setValue(value_type&& new_value, size_t index)
	{
		arr_[index] = std::move(new_value);
	}

	void HeaderVariable::removeIndexs(std::vector<size_t>&& indexs)
	{
		for (size_t iter{}; auto index : indexs) {
			if (index < arr_.size()) arr_.erase(arr_.begin() + index - (iter++));
		}
	}

}