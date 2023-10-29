#include "simplex_table.h"
#include <cassert>
#include <algorithm>

namespace KHAS {
	SimplexTable::SimplexTable(HeaderVariable&& hv, VerticalVariable&& vv, ObjectiveFunction&& of, std::initializer_list<value_type> il)
		: hv_{ hv }, vv_{ vv }, of_{ of }, arr_{ std::move(il) }, first_width_{ 3 }, other_width_{ 8 }
	{
		assert(arr_.size() > 0);
		assert((hv.size() * vv.size()) == arr_.size());
	}
	void SimplexTable::printTable() const noexcept
	{
        assert(other_width_ >= 4);

        hv_.print(first_width_, other_width_);
        std::cout << "\n";
        for (size_t j{}, je{ vv_.size() }; j != je; ++j) {

            std::cout << std::setw(first_width_) << vv_.getValue(j);
            for (size_t i{}, ie{ hv_.size() }; i != ie; ++i) {
                std::cout << " " << std::fixed << std::setprecision(4) << std::setw(other_width_) << getValue(i, j);
            }
            std::cout << "\n";
        }
	}
    void SimplexTable::printInfo(size_t col_index, size_t row_index, value_type value) const noexcept
    {
        printLine('-');
        std::cout << "Выбранный столбец:\t" << col_index + 1 << "\n";
        std::cout << "Выбранная строка: \t" << row_index + 1 << "\n";
        std::cout << "Главный елемент: \t" << value << "\n";
        printLine('-');
    }

    void SimplexTable::start()
    {
        assert(vv_.size() > 0);

        simplexTableRecovery();
        printLine('-');
        printText("Значение", vv_.getValue(vv_.size() - 1), "равно 0");
        printText("Значит,исходная задача разрешена,");
        printText("и мы можем построить начальную симплексную таблицу");
        printText("исходной задачи.");
        printLine('-');
        updateTable();
        simplexTableRecovery();
    }
    void SimplexTable::simplexTableRecovery()
    {
        while (!checkPositive())
        {
            size_t col_index{ findIndexMaxFromMin() };

            auto [row_index, value] { findPairMinFromMax(col_index) };

            printLine('=');
            printTable();
            printInfo(col_index, row_index, value);

            newXY(col_index, row_index);

            newValueRow(col_index, row_index, value);
            newValueColumn(col_index, row_index, value);
            newValueElem(col_index, row_index);
            swapHeaderPosition(col_index, row_index);
        }
        printLine('=');
        printTable();
    }
    void SimplexTable::printLine(char del) const noexcept
    {
        std::cout << std::string(other_width_ * (hv_.size() + 1), del) << "\n";
    }
    bool SimplexTable::checkPositive() const noexcept
    {
        return std::all_of(std::end(arr_) - hv_.size(), std::end(arr_) - 1, [](auto elem)
            { return elem >= 0; });
    }
    size_t SimplexTable::findIndexMaxFromMin() const noexcept
    {
        assert(vv_.size() > 0);
        assert(hv_.size() > 0);

        size_t index{};
        size_t row_index{ vv_.size() - 1 };

        for (size_t i{ 1 }, ie{ hv_.size() - 1 }; i != ie; ++i) {
            if (getValue(i, row_index) < getValue(index, row_index)) {
                index = i;
            }
        }

        return index;
    }
    std::pair<size_t, SimplexTable::value_type> SimplexTable::findPairMinFromMax(size_t col_index) const noexcept
    {
        assert(col_index >= 0);
        assert(hv_.size() >= 0);
        assert(vv_.size() >= 2);
        assert(arr_.size() >= 0 && arr_.size() >= col_index);

        size_t return_index{};
        value_type return_value{ arr_[col_index] };
        value_type tmp{ arr_[hv_.size() - 1] / arr_[col_index] };

        for (size_t step{ hv_.size() }, i{ 1 }, ie{ vv_.size() - 1 }; i != ie; ++i)
        {
            value_type right_elem{ arr_[(i + 1) * step - 1] };
            value_type select_elem{ arr_[i * step + col_index] };
            if (select_elem <= 0)
                continue;
            value_type ratio{ right_elem / select_elem };

            if (tmp <= 0 || ratio < tmp)
            {
                tmp = ratio;
                return_index = i;
                return_value = select_elem;
            }
        }
        return { return_index, return_value };
    }
    void SimplexTable::newValueRow(size_t col_index, size_t row_index, value_type value)
    {
        for (size_t i{}, ie{ vv_.size() }; i != ie; ++i) {
            if (i == row_index) continue;
            value_type select_value{ getValue(col_index, i) };
            if (select_value == 0) continue;

            setValue(-select_value / value, col_index, i);
        }
    }
    void SimplexTable::newValueColumn(size_t col_index, size_t row_index, value_type value)
    {

        for (size_t i{}, ie{ hv_.size() }; i != ie; ++i) {
            if (i == col_index) continue;
            value_type select_value{ getValue(i, row_index) };
            if (select_value == 0) continue;

            setValue(select_value / value, i, row_index);
        }
    }
    void SimplexTable::newValueElem(size_t col_index, size_t row_index)
    {
        setValue(1. / getValue(col_index, row_index), col_index, row_index);
    }
    void SimplexTable::newXY(size_t col_index, size_t row_index)
    {
        for (size_t j{}, je{ hv_.size() }; j != je; ++j) {
            if (j == col_index) continue;
            for (size_t i{}, ie{ vv_.size() }; i != ie; ++i) {
                if (i == row_index) continue;
                value_type main_value{ getValue(col_index, row_index) };
                value_type value{ getValue(j, i) };
                value_type prev_value{ getValue(col_index, i) };
                value_type next_value{ getValue(j, row_index) };
                setValue(value - prev_value * next_value / main_value, j, i);
            }
        }
    }
    void SimplexTable::swapHeaderPosition(size_t col_index, size_t row_index)
    {
        auto hv{ hv_.getValue(col_index) };
        auto vv{ vv_.getValue(row_index) };

        hv_.setValue(std::move(vv), col_index);
        vv_.setValue(std::move(hv), row_index);
    }
    void SimplexTable::updateTable()
    {
        auto fname{ vv_.getValue(vv_.size() - 1) };

        std::transform(fname.begin(), fname.end(), fname.begin(),
            [](unsigned char c) { return std::tolower(c); });

        vv_.setValue(std::move(fname), vv_.size() - 1);

        auto&& [vec_columns_to_remove, vec_column_index] { vecColumnsToRemove() };

        arr_ = newSimplexTable(std::move(vec_columns_to_remove));

        hv_.removeIndexs(std::move(vec_column_index));
        calculatingNewFunctionValues();
    }
    void SimplexTable::calculatingNewFunctionValues()
    {
        assert(hv_.size() * vv_.size() == arr_.size());
        assert(hv_.size() > 0);

        for (size_t i{}, ie{ hv_.size() }; i != ie; ++i) {

            value_type sum_coef{};
            for (size_t j{}, je{ vv_.size() - 1 }; j != je; ++j) {
                auto hv_coef{ of_.getValue(vv_.getValue(j)) };
                sum_coef += hv_coef * getValue(i, j);
            }
            sum_coef -= of_.getValue(hv_.getValue(i));
            setValue(sum_coef, i, vv_.size() - 1);
        }
    }
    std::vector<SimplexTable::value_type> SimplexTable::newSimplexTable(std::vector<size_t>&& vec_columns_to_remove) const
    {
        std::vector<value_type> new_arr;
        new_arr.reserve(arr_.size());

        for (size_t i{}, ie{ arr_.size() }; i != ie; ++i) {
            auto fn{ find_if(vec_columns_to_remove.begin(), vec_columns_to_remove.end(), [i, mod = hv_.size()](auto&& elem) {
                return i % mod == elem % mod;
                }) };
            if (fn == vec_columns_to_remove.end()) new_arr.emplace_back(arr_[i]);
        }
        new_arr.shrink_to_fit();
        return new_arr;
    }
    std::pair<std::vector<size_t>, std::vector<size_t>> SimplexTable::vecColumnsToRemove() const
    {
        assert(arr_.size() > 0);
        assert(arr_.size() >= hv_.size());

        std::vector<size_t> arr_elems;
        arr_elems.reserve(hv_.size());
        std::vector<size_t> arr_index;
        arr_index.reserve(hv_.size());

        for (size_t i{ arr_.size() - hv_.size() }, ie{ arr_.size() }; i != ie; ++i) {
            if (equal(arr_[i], value_type{ 1 })) {
                arr_elems.emplace_back(i);
                arr_index.emplace_back(i % hv_.size());
            }
        }

        if (arr_elems.size() == 0) {
            printText("ERROR!", "НЕДОПУСТИМЫЙ РЕЗУЛТАТ!");
            return {};
        }
        arr_elems.shrink_to_fit();
        arr_index.shrink_to_fit();

        return { std::move(arr_elems), std::move(arr_index) };
    }
    SimplexTable::value_type SimplexTable::getValue(size_t col_index, size_t row_index) const noexcept
    {
        assert(col_index * row_index < arr_.size());

        return arr_[hv_.size() * row_index + col_index];
    }
    void SimplexTable::setValue(value_type new_value, size_t col_index, size_t row_index) noexcept
    {
        assert(col_index * row_index < arr_.size());

        arr_[hv_.size() * row_index + col_index] = new_value;
    }
}