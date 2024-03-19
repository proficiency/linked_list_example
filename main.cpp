#include <iostream>
#include <memory>
#include <gtest/gtest.h>

static constexpr size_t test_list_size = 1000u;

template <typename T = size_t>
class Linked_List
{
    class Node
    {
    public:
        Node(const T& data) : m_data(data), m_previous(nullptr), m_next(nullptr) {}

        T                     m_data;
        std::unique_ptr<Node> m_previous;
        std::unique_ptr<Node> m_next;
    };

    std::unique_ptr<Node> m_head;
    std::unique_ptr<Node> m_tail;
    size_t                m_size;

public:
    Linked_List() : m_head(nullptr), m_tail(nullptr), m_size(0) {}
    ~Linked_List() = default;

    void push_back(const T& data)
    {
        std::unique_ptr<Node> new_node = std::make_unique<Node>(data);

        if (!m_head)
        {
            m_head = std::move(new_node);
            m_tail = std::move(m_head);
        }

        else
        {
            new_node->m_previous = std::move(m_tail);
            m_tail->m_next       = std::move(new_node);
            m_tail               = std::move(m_tail->m_next);
        }

        m_size++;
    }

    void pop_back()
    {
        if (m_tail == nullptr)
            return;

        if (m_head == m_tail)
        {
            m_head = nullptr;
            m_tail = nullptr;
        }

        else
        {
            m_tail = std::move(m_tail->m_previous);

            if (m_tail != nullptr)
                m_tail->m_next = nullptr;
        }

        m_size--;
    }

    void clear()
    {
        while (m_head)
            m_head = std::move(m_head->m_next);

        m_tail.reset();
        m_size = 0;
    }

    inline size_t size() const
    {
        return m_size;
    }
};

class Linked_List_Tests : public testing::Test
{
protected:
    void SetUp() override
    {
        for (size_t i = 0u; i < test_list_size; ++i)
            m_list.push_back(i);
    }

    Linked_List<size_t> m_list;
};

TEST_F(Linked_List_Tests, list_test_populated)
{
    ASSERT_EQ(m_list.size(), test_list_size);
}

TEST_F(Linked_List_Tests, list_test_push_back)
{
    m_list.push_back(1);
    ASSERT_EQ(m_list.size(), test_list_size + 1u);
}

TEST_F(Linked_List_Tests, list_test_pop_back)
{
    m_list.pop_back();
    ASSERT_EQ(m_list.size(), test_list_size - 1u);
}

TEST_F(Linked_List_Tests, list_test_clear)
{
    m_list.clear();
    ASSERT_EQ(m_list.size(), 0u);
}

int main()
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
