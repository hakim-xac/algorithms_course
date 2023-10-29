#include "ObjectiveFunction.h"



namespace KHAS {
	ObjectiveFunction::ObjectiveFunction(std::initializer_list<std::pair<const key_type, value_type>>&& il)
		: data_{ std::move(il) }
	{
	}
	const ObjectiveFunction::value_type& ObjectiveFunction::getValue(const key_type& key) & noexcept
	{
		return data_[key];
	}
}