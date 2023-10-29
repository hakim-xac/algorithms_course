#pragma once

#include <string>
#include <unordered_map>

namespace KHAS {
    class ObjectiveFunction {
    private:
        using key_type = std::string;
        using value_type = double;

    private:

        std::unordered_map<key_type, value_type> data_;
    public:
        explicit ObjectiveFunction(std::initializer_list<std::pair<const key_type, value_type>>&& il);

        const value_type& getValue(const key_type& key) & noexcept;

    };

}