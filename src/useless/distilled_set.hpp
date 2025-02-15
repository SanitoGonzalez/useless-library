#pragma once

#include <functional>
#include <set>
#include <unordered_map>


namespace useless {
template<typename ScoreType, typename KeyType>
class DistilledSet {
    using ForEachFunction = std::function<void(ScoreType, KeyType)>;

public:
    explicit DistilledSet(size_t capacity);

    void insert(ScoreType score, KeyType key);

    // Score only increases
    void update(ScoreType score, KeyType key);

    void erase(KeyType key);

    void for_each(const ForEachFunction& func);
    void for_each(size_t offset, size_t n, const ForEachFunction& func);

    size_t capacity() const { return _capacity; }
    size_t size() const { return _set.size(); }

private:
    size_t _capacity;
    std::multiset<std::pair<ScoreType, KeyType>> _set {};
    std::unordered_map<KeyType, typename decltype(_set)::iterator> _hash {};
};


template <typename ScoreType, typename KeyType>
DistilledSet<ScoreType, KeyType>::DistilledSet(const size_t capacity)
    : _capacity{capacity} {}

template <typename ScoreType, typename KeyType>
void DistilledSet<ScoreType, KeyType>::insert(const ScoreType score, const KeyType key) {
    if (_capacity == 0) return;

    if (_set.size() < _capacity) {
        _hash[key] = _set.insert({score, key});
        return;
    }

    const auto& [lowest_score, lowest_key] {*_set.begin()};
    if ( score <= lowest_score) return;

    _hash.erase(_set.erase(_set.begin())->second);
    _hash[key] = _set.insert({score, key});
}

template <typename ScoreType, typename KeyType>
void DistilledSet<ScoreType, KeyType>::update(const ScoreType score, const KeyType key) {
    if (!_hash.contains(key)) return;

    _set.erase(_hash[key]);
    _hash[key] = _set.insert({score, key});
}

template <typename ScoreType, typename KeyType>
void DistilledSet<ScoreType, KeyType>::erase(KeyType key) {
    if (!_hash.contains(key)) return;

    _set.erase(_hash[key]);
    _hash.erase(key);
}

template <typename ScoreType, typename KeyType>
void DistilledSet<ScoreType, KeyType>::for_each(const ForEachFunction& func) {
    for (auto iter {_set.rbegin()}; iter != _set.rend(); ++iter) {
        const auto& [score, key] {*iter};
        func(score, key);
    }
}

template <typename ScoreType, typename KeyType>
void DistilledSet<ScoreType, KeyType>::for_each(
    const size_t offset, size_t n, const ForEachFunction& func) {
    if (offset >= _set.size()) return;

    auto iter {_set.rbegin()};
    std::advance(iter, offset);
    n = std::min(n, _set.size() - offset);

    for (size_t i {0}; i < n; ++i, ++iter) {
        const auto& [score, key] {*iter};
        func(score, key);
    }
}
}

