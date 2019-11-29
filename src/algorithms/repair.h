//============================================================================
// @name        : repair.h
// @author      : Thomas Dooms
// @date        : 11/19/19
// @version     :
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description :
//============================================================================

namespace algorithm::repair
{
    using Pair = uint64_t;

    namespace dot
    {
        bool less(const std::pair<uint64_t, uint32_t>& lhs, const std::pair<uint64_t, uint32_t>& rhs) noexcept
        {
            return lhs.second < rhs.second;
        }
        bool greater(const std::pair<uint64_t, uint32_t>& lhs, const std::pair<uint64_t, uint32_t>& rhs) noexcept
        {
            return lhs.second > rhs.second;
        }

        class queue
        {
        public:
            using iterator = std::vector<std::pair<Pair, uint32_t>>::iterator;
            using reverse_iterator = std::vector<std::pair<Pair, uint32_t>>::reverse_iterator;
            using pointer  = std::vector<std::pair<Pair, uint32_t>>::pointer;

            explicit queue(size_t amount) : low(200), high(), high_index(0), low_index(200)
            {
                high.reserve(amount / 1000);

                std::fill(increase_cache.begin(), increase_cache.end(), nullptr);
                std::fill(decrease_cache.begin(), decrease_cache.end(), nullptr);

                if(not smart_reserve) return;
                // really smart magic numbers
                low[0].reserve(amount / 250);
                for(size_t i = 1; i < low.size(); i++)
                {
                    low[i].reserve(amount / (50 * std::sqrt(i)));
                }
            }

            void insert(Pair pair)
            {
                low[1].emplace(pair, false);
            }

            void increase(Pair pair, uint32_t num)
            {
                if(num >= low.size())
                {
                    const auto cache_iter = std::find_if(increase_cache.begin(), increase_cache.end(), [&](auto ptr){ return ptr != nullptr and ptr->first == pair; });

                    iterator iter;
                    iterator upper;

                    if(cache_iter != increase_cache.end() and (*cache_iter)->first == pair)
                    {
                        iter = iterator(*cache_iter);
                        upper = std::find_if(std::reverse_iterator(iter), high.rend(), [&](auto elem){ return elem.second != num; }).base();

                        (*cache_iter) = &*upper;
                    }
                    else
                    {
                        upper = std::lower_bound(high.begin() + high_index, high.end(), std::make_pair(pair, num), greater);
                        iter = std::find_if(upper, high.end(), [&](const auto& elem){ return elem.first == pair; });

                        increase_cache[increase_index] = &*upper;
                        increase_index = (increase_index + 1) % increase_size;
                    }

                    iter->second++;
                    std::swap(*iter, *upper);
                }
                else
                {
                    // we actually dont need to erase, so we dont
                    if(erase) low[num].erase(pair);
                    if (num + 1 < low.size())
                    {
                        low[num + 1].emplace(pair, false);
                    }
                    else
                    {
                        const auto cap = high.capacity();
                        high.emplace_back(pair, num + 1);
                        if(cap != high.capacity())
                        {
                            //reset caches
                            increase_index = 0;
                            decrease_index = 0;
                            std::fill(increase_cache.begin(), increase_cache.end(), nullptr);
                            std::fill(increase_cache.begin(), increase_cache.end(), nullptr);
                        }
                    }
                }
            }

            void decrease(Pair pair, uint32_t num)
            {
                if(num >= low.size())
                {
                    const auto cache_iter = std::find_if(decrease_cache.begin(), decrease_cache.end(), [&](auto ptr){ return ptr != nullptr and ptr->first == pair; });

                    iterator iter;
                    iterator lower;

                    if(cache_iter != decrease_cache.end() and (*cache_iter)->first == pair)
                    {
                        iter = iterator(*cache_iter);
                        lower = std::find_if(iter, high.end(), [&](auto elem){ return elem.second != num; }) - 1;

                        (*cache_iter) = &*lower;
                    }
                    else
                    {
                        lower = std::lower_bound(high.rbegin(), high.rend(), std::make_pair(pair, num), less).base() - 1;
                        iter = std::find_if(reverse_iterator(lower)-1, high.rend(), [&](const auto& elem){ return elem.first == pair; }).base() - 1;

                        decrease_cache[decrease_index] = &*lower;
                        decrease_index = (decrease_index + 1) % decrease_size;
                    }

                    iter->second--;
                    std::swap(*iter, *lower);

                    if(num == low.size())
                    {
                        high.pop_back();
                        low.back().emplace(pair, false);
                    }
                }
                else
                {
                    // we actually dont need to erase, so we dont
                    if(erase) low[num].erase(pair);
                    low[num - 1].emplace(pair, false);
                }
            }

            std::pair<Pair, uint32_t> pop() noexcept
            {
                if(high_index < high.size()) return high[high_index++];

                for(auto i = low.size(); i != 0; i--)
                {
                    if(low[i - 1].empty()) continue;

                    const auto copy = low[i - 1].begin()->first;
                    low[i - 1].erase( low[i - 1].begin() );

                    return std::make_pair(copy, i - 1);
                }
                return std::make_pair(6969696969,69696969);
            }

        private:
            constexpr static inline bool erase = true; // false uses more memory but is faster - 5%
            constexpr static inline bool smart_reserve = false; // true uses more memory but is faster - 10%

            std::vector<robin_hood::unordered_map<Pair, bool>> low;
            std::vector<std::pair<Pair, uint32_t>> high;

            constexpr static inline size_t increase_size = 8;
            constexpr static inline size_t decrease_size = 8;

            uint8_t increase_index = 0;
            std::array<pointer, increase_size> increase_cache {};

            uint8_t decrease_index = 0;
            std::array<pointer, decrease_size> decrease_cache {};

            size_t high_index;
            size_t low_index;
        };

        using map = robin_hood::unordered_node_map<Pair, std::pair<std::vector<Variable>, uint32_t>>;

        class maxmap
        {
        public:
            explicit maxmap(size_t amount) : map(amount / 7), queue(amount) {}

            void emplace(Pair pair, uint32_t index)
            {
                const auto iter = map.find(pair);

                if(iter == map.end())
                {
                    map.emplace(pair, std::make_pair(std::vector<uint32_t>{index}, 1));
                    queue.insert(pair);
                }
                else
                {
                    iter->second.first.push_back(index);
                    queue.increase(iter->first, iter->second.second);
                    iter->second.second += 1;
                }
            }

            auto pop() noexcept
            {
                const auto top = queue.pop();
                return map.find(top.first);
            }

            void decrease(Pair pair, Pair curr) noexcept
            {
                auto iter = map.find(pair);
                if(curr != iter->first and iter->second.second != 0)
                {
                    queue.decrease(iter->first, iter->second.second);
                    iter->second.second -= 1;
                }
            }

            [[nodiscard]] auto size() const noexcept
            {
                return map.size();
            }

        private:
            dot::map map;
            dot::queue queue;
        };
    }



    std::tuple<Settings, std::vector<Variable>, std::vector<Production>> compress(const std::vector<uint8_t>& bytes)
    {
        // all the lambda functions
        constexpr auto compose   = [](Variable lhs, Variable rhs) -> Pair { return (static_cast<Pair>(lhs) << 32u) + rhs; };
        constexpr auto decompose = [](Pair elem) { return std::array{static_cast<Variable>(elem >> 32u), static_cast<Variable>(elem)}; };

        constexpr auto not_tombstone = [](const auto elem){ return elem != Settings::end(); };


        constexpr size_t min_occurences = 8; // magic stuff


        Settings settings;
        dot::maxmap map(bytes.size());
        std::vector<Variable> string(bytes.begin(), bytes.end());
        std::vector<Production> productions;

        productions.reserve(bytes.size() / 100);

        size_t production_counter = 0;
        Pair last_pair = std::numeric_limits<Pair>::max();

        for(size_t i = 0; i < string.size() - 1; i++)
        {
            const auto pair = compose(string[i], string[i + 1]);

            // skip when we have a string of the same chars
            if(pair == last_pair)
            {
                last_pair = std::numeric_limits<Pair>::max();
                continue;
            }
            else last_pair = pair;
            map.emplace(pair, i);
        }

        while(true)
        {
            auto elem = map.pop();

            if(elem->second.second <= min_occurences) break;

            const auto pair = decompose(elem->first);

            // determine new value for production
            uint32_t new_value;
            if(Settings::is_reserved_rule(pair[0], pair[1]))
            {
                new_value = Settings::convert_to_reserved(pair[0], pair[1]);
            }
            else
            {
                new_value = settings.offset(production_counter);
                productions.emplace_back(pair);
                production_counter++;
            }

            // loop over all occurences
            for(auto index : elem->second.first)
            {
                const auto curr = string.begin() + index;

                // means we already changed the value to another production
                if(*curr != pair[0]) continue;

                const auto second = std::find_if(curr + 1, string.end(), not_tombstone);

                // means we already changed the value to another production
                if(second == string.end() or *second != pair[1]) continue;

                const auto next = std::find_if(second + 1, string.end(), not_tombstone);

                if(next != string.end()) map.decrease(compose(*second, *next), elem->first);

                const auto prev = std::find_if(std::reverse_iterator(curr), string.rend(), not_tombstone);

                if(prev != string.rend()) map.decrease(compose(*prev, *curr), elem->first);

                *curr = new_value;
                if(second != string.end()) *second = Settings::end();

                if(next != string.end() ) map.emplace(compose(*curr, *next), index);
                if(prev != string.rend()) map.emplace(compose(*prev, *curr), prev.base() - string.begin() - 1);
            }
            elem->second.first.clear();
        }

        // move away tombstones
        const auto end = std::copy_if(string.begin(), string.end(), string.begin(), not_tombstone);
        string.resize(end - string.begin());

//        std::cout << "possible productions per byte: " << float(map.size()) / float(bytes.size()) << '\n';
//        std::cout << "productions per byte: " << float(productions.size()) / float(bytes.size()) << '\n';
//        std::cout << "productions: " << productions.size() << '\n';
//
//        auto copy = productions;
//        std::sort(copy.begin(), copy.end());
//        const auto last = std::unique(copy.begin(), copy.end());
//        std::cout << "duplicate productions: " << copy.end() - last << '\n';

        return std::make_tuple(settings, std::move(string), std::move(productions));
    }
}












