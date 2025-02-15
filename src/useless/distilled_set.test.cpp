#include <catch2/catch_test_macros.hpp>

#include <useless/distilled_set.hpp>

using namespace useless;

TEST_CASE("Low scores are distilled.") {
    using ScoreType = float;
    using KeyType = int;

    constexpr size_t CAPACITY {10};
    constexpr ScoreType BIG_SCORE {777.0f};
    constexpr ScoreType SMALL_SCORE {5.0f};

    DistilledSet<ScoreType, KeyType> set {CAPACITY};

    for (KeyType key = 100; key < 100 + CAPACITY; ++key) {
        set.insert(SMALL_SCORE, key);
    }
    for (KeyType key = 200; key < 200 + CAPACITY; ++key) {
        set.insert(BIG_SCORE, key);
    }
    for (KeyType key = 300; key < 300 + CAPACITY; ++key) {
        set.insert(SMALL_SCORE, key);
    }

    REQUIRE(set.capacity() == CAPACITY);
    REQUIRE(set.size() == CAPACITY);
    set.for_each([&](auto score, auto) {
        REQUIRE(score == BIG_SCORE);
    });
}

TEST_CASE("Scores are updated.") {
    using ScoreType = unsigned long long;
    using KeyType = int;

    constexpr size_t CAPACITY {20};
    constexpr ScoreType OLD_SCORE {42};
    constexpr ScoreType NEW_SCORE {27};

    DistilledSet<ScoreType, KeyType> set {CAPACITY};

    for (KeyType key = 0; key < CAPACITY; ++key) {
        set.insert(OLD_SCORE, key);
    }
    for (KeyType key = 0; key < CAPACITY / 2; ++key) {
        set.update(NEW_SCORE, key);
    }

    REQUIRE(set.capacity() == CAPACITY);
    REQUIRE(set.size() == CAPACITY);

    set.for_each(0, CAPACITY / 2, [&](auto score, auto) {
        REQUIRE(score == NEW_SCORE);
    });
    set.for_each(CAPACITY / 2, CAPACITY / 2, [&](auto score, auto) {
        REQUIRE(score == OLD_SCORE);
    });
}

TEST_CASE("Distilled set is iterated.") {
    using ScoreType = double;
    using KeyType = short;

    constexpr size_t CAPACITY {10};
    constexpr ScoreType SCORE {42.0};

    DistilledSet<ScoreType, KeyType> set {CAPACITY};

    for (KeyType key = 0; key < CAPACITY; ++key) {
        set.insert(SCORE, key);
    }

    REQUIRE(set.capacity() == CAPACITY);
    REQUIRE(set.size() == CAPACITY);

    KeyType target_key {0};
    set.for_each([&](auto score, auto key) {
        REQUIRE(score == SCORE);
        REQUIRE(key == target_key);
        target_key += 1;
    });
}

TEST_CASE("Distilled set cannot be iterated over capacity.") {
    constexpr size_t CAPACITY {10};

    DistilledSet<int, int> set {CAPACITY};

    for (int key = 0; key < CAPACITY; ++key) {
        set.insert(100, key);
    }

    REQUIRE(set.capacity() == CAPACITY);
    REQUIRE(set.size() == CAPACITY);

    set.for_each(CAPACITY, 10, [&](auto, auto) {
        FAIL();
    });

    int iterations {0};
    set.for_each(0, CAPACITY * 2, [&](auto, auto) {
        iterations += 1;
    });
    REQUIRE(iterations == CAPACITY);
}

TEST_CASE("Distilled set is erased.") {
    constexpr size_t CAPACITY {10};

    DistilledSet<int, int> set {CAPACITY};

    for (int key = 0; key < CAPACITY; ++key) {
        set.insert(100, key);
    }

    REQUIRE(set.capacity() == CAPACITY);
    REQUIRE(set.size() == CAPACITY);

    for (int key = 0; key < CAPACITY; ++key) {
        set.erase(key);
    }

    REQUIRE(set.capacity() == CAPACITY);
    REQUIRE(set.size() == 0);
}
