#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

namespace KHAS {

    class HeaderVariable
    {
        using value_type = std::string;

    private:
        std::vector<value_type> arr_;

    public:
        explicit HeaderVariable(std::initializer_list<value_type>&& il);

        size_t size() const noexcept;

        void print(size_t first_width = 3, size_t other_width = 8) const noexcept;

        value_type getValue(size_t index) const noexcept;

        void setValue(value_type&& new_value, size_t index);

        void removeIndexs(std::vector<size_t>&& indexs);
    };

}