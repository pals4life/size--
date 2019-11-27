//============================================================================
// @name        : repair.h
// @author      : Thomas Dooms
// @date        : 11/19/19
// @version     :
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description :
//============================================================================

#include <boost/heap/fibonacci_heap.hpp>
#include "absl/container/flat_hash_map.h"

namespace algorithm::repair
{
    std::tuple<Settings, std::vector<Variable>, std::vector<Production>> compress(const std::vector<uint8_t>& bytes)
    {
        constexpr auto compose   = [](uint64_t lhs, uint64_t rhs) -> uint64_t { return (lhs << 32u) + rhs; };
        constexpr auto decompose = [](uint64_t elem) { return std::array{static_cast<uint32_t>(elem >> 32u), static_cast<uint32_t>(elem)}; };

        struct Compare
        {
            bool operator()(void* lhs, void* rhs) const noexcept
            {
                using Type = const std::pair<size_t, std::pair<std::vector<uint32_t>, uint64_t>>*;
                return reinterpret_cast<Type>(lhs)->second.first.size() < reinterpret_cast<Type>(rhs)->second.first.size();
            }
        };

        using Queue = boost::heap::fibonacci_heap<void*, boost::heap::compare<Compare>>;
        using Map   = std::unordered_map<size_t, std::pair<std::vector<uint32_t>, Queue::handle_type>>;
        using Element = const std::pair<size_t, std::pair<std::vector<uint32_t>, Queue::handle_type>>*;

        absl::flat_hash_map<int, int> test;

        const auto emplace = [](auto& map, auto& queue, size_t pair, size_t index)
        {
            const auto [iter, emplaced] = map.try_emplace(pair);

            if(emplaced)
            {
                const auto handle = queue.push(&*iter);
                iter->second.first = std::vector<uint32_t>{ static_cast<uint32_t>(index) };
                iter->second.second = handle;
            }
            else
            {
                iter->second.first.emplace_back(index);
                queue.increase(iter->second.second);
            }
        };

        Queue queue;
        Map map;

        Settings settings;
        std::vector<Variable> string(bytes.begin(), bytes.end());
        std::vector<Production> productions;

        size_t production_counter = 0;

        constexpr size_t cutoff = 4; // magic stuff

        size_t last_pair = 0;

        for(size_t i = 0; i < string.size() - 1; i++)
        {
            const auto pair = compose(string[i], string[i+1]);

            // skip when we have a string of the same chars
            if(pair == last_pair)
            {
                last_pair = 0;
                continue;
            }
            else last_pair = pair;

            emplace(map, queue, pair, i);
        }

        while(true)
        {
            const auto top = reinterpret_cast<Element>(queue.top());
            std::cout << top->second.first.size() << '\n';
            if(top->second.first.size() <= cutoff) break;

            for(auto index : top->second.first)
            {
                const auto lambda = [](const auto elem){ return elem != Settings::end(); };
                const auto curr = string.begin() + index;

                const auto second = std::find_if(curr + 1, string.end(), lambda);
                const auto next = std::find_if(second + 1, string.end(), lambda);

                if(next != string.end())
                {
                    // this should always exists, so we don;t check on out of bounds
                    const auto iter = map.find(compose(*second, *next));
                    queue.decrease(iter->second.second);
//                    iter->sec
                }
                const auto prev = std::find_if(std::reverse_iterator(curr) + 1, string.rend(), lambda);
                if(prev != string.rend())
                {
                    // same here
                    const auto iter = map.find(compose(*prev, *curr));
                    queue.decrease(iter->second.second);
                }

                *curr = settings.offset(production_counter);
                if(second != string.end()) *second = Settings::end();

                if(next != string.end() )emplace(map, queue, compose(*curr, *next), index);
                if(prev != string.rend())emplace(map, queue, compose(*prev, *curr), prev.base() - string.begin());
            }
            productions.emplace_back(decompose(top->first));
            production_counter++;

            queue.pop();
            map.erase(top->first);
        }


        return std::make_tuple(settings, std::move(string), std::move(productions));
    }
}












