#pragma once
#include <iostream>
#include "ObjectiveFunction.h"
#include "HeaderVariable.h"
#include "VerticalVariable.h"

namespace KHAS
{
    
    class SimplexTable
    {
        using value_type = double;

    private:
        HeaderVariable hv_;
        VerticalVariable vv_;
        ObjectiveFunction of_;
        std::vector<value_type> arr_;
        size_t first_width_;
        size_t other_width_;

    public:
        explicit SimplexTable(HeaderVariable&& hv
            , VerticalVariable&& vv
            , ObjectiveFunction&& of
            , std::initializer_list<value_type> il);

        void printTable() const noexcept;

        void printInfo(size_t col_index, size_t row_index, value_type value) const noexcept;

        void start();

    private:

        void simplexTableRecovery();

        void printLine(char del) const noexcept;

        bool checkPositive() const noexcept;

        size_t findIndexMaxFromMin() const noexcept;

        std::pair</* index */ size_t, /* value*/ value_type> findPairMinFromMax(size_t col_index) const noexcept;

        void newValueRow(size_t col_index, size_t row_index, value_type value);

        void newValueColumn(size_t col_index, size_t row_index, value_type value);

        void newValueElem(size_t col_index, size_t row_index);

        void newXY(size_t col_index, size_t row_index);

        void swapHeaderPosition(size_t col_index, size_t row_index);

        void updateTable();

        void calculatingNewFunctionValues();

        std::vector<value_type> newSimplexTable(std::vector<size_t>&& vec_columns_to_remove) const;

        std::pair<std::vector<size_t>, std::vector<size_t>> vecColumnsToRemove() const;

        value_type getValue(size_t col_index, size_t row_index) const noexcept;

        void setValue(value_type new_value, size_t col_index, size_t row_index) noexcept;

    private:

        template <typename... TText>
        void printText(TText &&...text) const
        {
            ((std::cout << std::forward<TText>(text) << " "), ...);
            std::cout << "\n";
        }
        template <typename TText>
        void printText(TText&& text) const
        {
            std::cout << std::forward<TText>(text) << "\n";
        }

        template<typename T>
            requires (!std::is_floating_point_v<T>)
        bool equal(const T& lhs, const T& rhs) const
        {
            return lhs == rhs;
        }
        template<typename T>
            requires std::is_floating_point_v<T>
        bool equal(T lhs, T rhs) const
        {
            const double accuracy{ 1.E-08 };

            if (lhs > rhs) return (lhs - rhs) < accuracy;
            if (lhs < rhs) return (rhs - lhs) < accuracy;
        }
    };

}