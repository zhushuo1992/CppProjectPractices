//
// Created by zhushuo on 2023/8/29.
//

#ifndef INC_05_SUMMARY_HPP
#define INC_05_SUMMARY_HPP

#include "cpplang.hpp"
#include "SalesData.hpp"
#include "SpinLock.hpp"

namespace cpp_study {
    class Summary final
    {
    public:
        using this_t = Summary;
        using sales_t = SalesData;
        using lock_t = SplinLock;
        using lock_guard_t = SplinLockGuard;
        using string_t = std::string;

        using map_t = std::map<string_t, sales_t>;
        using minmax_sales_t = std::pair<string_t, string_t>;
    public:
        Summary() = default;
        ~Summary() = default;
        Summary(const this_t&) = delete;
        Summary& operator=(const this_t&) = delete;

    private:
        mutable lock_t m_lock;
        map_t  m_sales;
    public:
        void add_sales(const sales_t& s)
        {
            lock_guard_t guard(m_lock);

            const auto& id = s.id();
            if(m_sales.find(id) == m_sales.end())
            {
                m_sales[id] = s;
                return;
            }

            m_sales[id].inc_sold(s.sold());
            m_sales[id].inc_revenue(s.revenue());
        }

        minmax_sales_t minmax_sales() const
        {
            lock_guard_t guard(m_lock);
            if(m_sales.empty())
            {
                return  minmax_sales_t();
            }

            auto ret = std::minmax_element(std::begin(m_sales), std::end(m_sales), [](const auto& a, const auto& b){
                return  a.second.sold() < b.second.sold();
            });

            auto min_pos = ret.first;
            auto max_pos = ret.second;

            return {min_pos->second.id(), max_pos->second.id()};


        }
    };
}

#endif //INC_05_SUMMARY_HPP
