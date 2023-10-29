#pragma once

#include <string>
#include <vector>

namespace KHAS{

    class VerticalVariable
    {
        using value_type = std::string;

    private:
        std::vector<value_type> arr_;

    public:
        explicit VerticalVariable(std::initializer_list<value_type>&& il);

        size_t size() const noexcept;

        value_type getValue(size_t index) const noexcept;

        void setValue(value_type&& new_value, size_t index);
    };

}
