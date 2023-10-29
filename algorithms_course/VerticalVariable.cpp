#include "VerticalVariable.h"

namespace KHAS {
	VerticalVariable::VerticalVariable(std::initializer_list<value_type>&& il)
		: arr_{ std::move(il) }
	{
	}
	size_t VerticalVariable::size() const noexcept
	{
		return arr_.size();
	}
	VerticalVariable::value_type VerticalVariable::getValue(size_t index) const noexcept
	{
		return arr_[index];
	}
	void VerticalVariable::setValue(value_type&& new_value, size_t index)
	{
		arr_[index] = std::move(new_value);
	}
}