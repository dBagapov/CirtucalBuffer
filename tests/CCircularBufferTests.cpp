#include "../lib/CCircularBuffer.h"

#include <gtest/gtest.h>

struct TestClassNew {
    int field;
    std::string str;

    TestClassNew() : field(7), str("ok") {};
};

TEST(CreateTests, EmptyTest) {

    CCircularBuffer<int> int_buffer;
    CCircularBuffer<std::string> string_buffer;
    CCircularBuffer<bool> bool_buffer;
    CCircularBuffer<TestClassNew> my_class_buffer;

    ASSERT_TRUE(int_buffer.empty());
    ASSERT_TRUE(string_buffer.empty());
    ASSERT_TRUE(bool_buffer.empty());
    ASSERT_TRUE(my_class_buffer.empty());
}

TEST(CreateTests, NoEmptyTest) {
    TestClassNew a;

    CCircularBuffer<int> int_buffer(4, 0);
    CCircularBuffer<std::string> string_buffer(2, "Hi");
    CCircularBuffer<bool> bool_buffer(3, true);
    CCircularBuffer<TestClassNew> my_class_buffer(2, a);

    ASSERT_FALSE(int_buffer.empty());
    ASSERT_FALSE(string_buffer.empty());
    ASSERT_FALSE(bool_buffer.empty());
    ASSERT_FALSE(my_class_buffer.empty());

    ASSERT_EQ(int_buffer.size(), 4);
    ASSERT_EQ(string_buffer.size(), 2);
    ASSERT_EQ(bool_buffer.size(), 3);
    ASSERT_EQ(my_class_buffer.size(), 2);

    ASSERT_EQ(int_buffer.max_size(), 4);
    ASSERT_EQ(string_buffer.max_size(), 2);

    std::vector<int> int_vector = {0, 0, 0, 0};
    ASSERT_TRUE(std::equal(int_buffer.begin(), int_buffer.end(),
                           int_vector.begin(), int_vector.end()));

    std::vector<std::string> string_vector = {"Hi", "Hi"};
    ASSERT_TRUE(std::equal(string_buffer.begin(), string_buffer.end(),
                           string_vector.begin(), string_vector.end()));

    std::vector<bool> bool_vector = {true, true, true};
    ASSERT_TRUE(std::equal(bool_buffer.begin(), bool_buffer.end(),
                           bool_vector.begin(), bool_vector.end()));
}

TEST(CopyTests, Copy) {
    CCircularBuffer<int> int_buffer(4, 0);
    CCircularBuffer<int> empty_buffer;
    CCircularBuffer<int> new_int_buffer(int_buffer);
    empty_buffer = int_buffer;

    ASSERT_TRUE(int_buffer == new_int_buffer);
    ASSERT_TRUE(int_buffer == empty_buffer);

    CCircularBuffer<std::string> string_buffer(2, "Hi");
    CCircularBuffer<std::string> empty_string_buffer;
    CCircularBuffer<std::string> new_string_buffer(string_buffer);
    empty_string_buffer = string_buffer;

    ASSERT_TRUE(string_buffer == new_string_buffer);
    ASSERT_TRUE(string_buffer == empty_string_buffer);
}

TEST(OperatorTests, Equal) {
    CCircularBuffer<int> buffer0(4, 0);
    CCircularBuffer<int> buffer1(4, 0);
    CCircularBuffer<int> buffer2(5, 1);
    CCircularBuffer<int> buffer3(5, 0);

    ASSERT_TRUE(buffer0 == buffer1);
    ASSERT_TRUE(buffer1 != buffer2);
    ASSERT_TRUE(buffer2 != buffer3);
    ASSERT_TRUE(buffer3 != buffer0);
}

TEST(AddElementTests, PushFront) {
    CCircularBuffer<int> buffer0(3);
    std::vector<int> vector = {3, 4, 5};

    buffer0.push_front(5);
    buffer0.push_front(4);
    buffer0.push_front(3);
    ASSERT_TRUE(std::equal(buffer0.begin(), buffer0.end(), vector.begin(), vector.end()));

    buffer0.push_front(2);
    buffer0.push_front(1);
    vector = {1,2,3};
    ASSERT_TRUE(std::equal(buffer0.begin(), buffer0.end(), vector.begin(), vector.end()));

    CCircularBuffer<int> buffer1(4, 1);
    buffer1.push_front(0);
    vector = {0, 1, 1, 1};
    ASSERT_TRUE(std::equal(buffer1.begin(), buffer1.end(), vector.begin(), vector.end()));
}

TEST(AddElementTests, PushBack) {
    CCircularBuffer<int> buffer0(5);
    std::vector<int> vector0 = {3, 4, 5};

    buffer0.push_back(3);
    buffer0.push_back(4);
    buffer0.push_back(5);
    ASSERT_TRUE(std::equal(buffer0.begin(), buffer0.end(), vector0.begin(), vector0.end()));

    CCircularBuffer<int> buffer(3);
    buffer.push_back(3);
    buffer.push_back(4);
    buffer.push_back(5);
    buffer.push_back(6);
    buffer.push_back(7);

    std::vector<int> vector = {5, 6, 7};

    ASSERT_TRUE(std::equal(buffer.begin(), buffer.end(), vector.begin(), vector.end()));
}

TEST(AddElementTests, Insert) {
    CCircularBuffer<int> buffer(5);
    buffer.push_back(1);
    buffer.push_back(3);
    buffer.push_back(5);

    auto it = buffer.begin() + 1;
    buffer.insert(it, 2);
    it += 2;
    buffer.insert(it, 4);

    std::vector<int> vector = {1, 2, 3, 4, 5};
    ASSERT_TRUE(std::equal(buffer.begin(), buffer.end(), vector.begin(), vector.end()));

    it += 2;
    buffer.insert(it, 6);
    vector = {2, 3, 4, 5, 6};
    ASSERT_TRUE(std::equal(buffer.begin(), buffer.end(), vector.begin(), vector.end()));
}

TEST(DeleteElementTests, PopFront) {
    CCircularBuffer<int> buffer(5);
    buffer.pop_front();
    ASSERT_TRUE(buffer.empty());

    buffer.push_back(1);
    buffer.push_back(2);
    buffer.push_back(3);

    buffer.pop_front();
    ASSERT_EQ(buffer.front(), 2);
    buffer.pop_front();
    ASSERT_EQ(buffer.front(), 3);
    buffer.pop_front();
    ASSERT_TRUE(buffer.empty());
}

TEST(DeleteElementTests, PopBack) {
    CCircularBuffer<int> buffer(5);
    buffer.pop_back();
    ASSERT_TRUE(buffer.empty());

    buffer.push_back(1);
    buffer.push_back(2);
    buffer.push_back(3);

    buffer.pop_back();
    ASSERT_EQ(buffer.back(), 2);
    buffer.pop_back();
    ASSERT_EQ(buffer.back(), 1);
    buffer.pop_back();
    ASSERT_TRUE(buffer.empty());

}

TEST(DeleteElementTests, Erase) {
    CCircularBuffer<int> buffer(5);
    buffer.push_back(1);
    buffer.push_back(2);
    buffer.push_back(3);
    buffer.push_back(4);
    buffer.push_back(5);

    auto it1 = buffer.begin() + 1;
    it1 = buffer.erase(it1);

    auto it2 = buffer.end() - 2;
    it2 = buffer.erase(it2);

    std::vector<int> vector = {1, 3, 5};

    ASSERT_TRUE(std::equal(buffer.begin(), buffer.end(), vector.begin(), vector.end()));
    ASSERT_EQ(*it1, 3);
    ASSERT_EQ(*it2, 5);
}

TEST(DeleteElementTests, EraseArea) {
    CCircularBuffer<int> buffer(5);
    buffer.push_back(1);
    buffer.push_back(2);
    buffer.push_back(3);
    buffer.push_back(4);
    buffer.push_back(5);

    auto it1 = buffer.begin() + 1;
    auto it2 = buffer.end() - 1;
    it2 = buffer.erase(it1, it2);

    std::vector<int> vector = {1, 5};

    ASSERT_TRUE(std::equal(buffer.begin(), buffer.end(), vector.begin(), vector.end()));
    ASSERT_EQ(*it2, 5);
}

TEST(ClearTest, CLear) {
    CCircularBuffer<int> buffer(5, 3);
    buffer.clear();
    ASSERT_TRUE(buffer.empty());
}

TEST(STLTest, STL) {
    CCircularBuffer<int> buffer(6);
    buffer.push_back(2);
    buffer.push_back(6);
    buffer.push_back(3);
    buffer.push_back(4);
    buffer.push_back(1);
    buffer.push_back(5);

    std::sort(buffer.cbegin(), buffer.cend());

    std::vector<int> vector = {1, 2, 3, 4, 5, 6};

    ASSERT_TRUE(std::equal(buffer.begin(), buffer.end(), vector.begin(), vector.end()));
    ASSERT_TRUE(std::is_sorted(buffer.cbegin(), buffer.cend()));

    std::reverse(buffer.cbegin(), buffer.cend());
    std::reverse(vector.begin(), vector.end());

    ASSERT_TRUE(std::equal(buffer.begin(), buffer.end(), vector.begin(), vector.end()));
}


