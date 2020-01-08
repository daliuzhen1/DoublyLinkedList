#include <memory>
#include <functional>

template<class ValueType>
class DoublyLinkedList {
    struct Node {
        Node* prev {nullptr};
        Node* next {nullptr};
        ValueType value;
    };
public:
    class Iterator {
        friend class DoublyLinkedList;
        public:
            using Node = DoublyLinkedList::Node;
            Iterator() = default;
            Iterator(Node* node_ptr) : m_node_ptr(node_ptr) {
            }

            Iterator operator++ () {
                if (!m_node_ptr)
                    throw std::runtime_error("bad Iterator");
                m_node_ptr = m_node_ptr->next;
                return *this;
            }

            Iterator operator-- () {
                if (!m_node_ptr)
                    throw std::runtime_error("bad Iterator");
                m_node_ptr = m_node_ptr->prev;
                return *this;       
            }

            friend bool operator == (const Iterator& l_it, const Iterator& r_it) {
                return l_it.m_node_ptr == r_it.m_node_ptr;
            }
            friend bool operator != (const Iterator& l_it, const Iterator& r_it) {
                return l_it.m_node_ptr != r_it.m_node_ptr;
            }

            ValueType operator*() const {
                return m_node_ptr->value;
            }

            ValueType* operator->() const {
                return &m_node_ptr->value;
            }
            operator bool() const {
                return m_node_ptr != nullptr && m_node_ptr->next != nullptr;
            }
        private:
            Node* m_node_ptr {nullptr};
    };

    using SizeType = uint64_t;
    explicit DoublyLinkedList() : m_head(new Node), m_tail(m_head) {
    }

    explicit DoublyLinkedList(const std::initializer_list<ValueType>& initial_list) : m_head(new Node), m_tail(m_head) {
        m_size = initial_list.size();
        auto prev_ptr = m_head;
        Node* current_ptr = nullptr;
        for (auto& value : initial_list) {
            current_ptr = new Node;
            current_ptr->value = value;
            current_ptr->prev = prev_ptr;
            prev_ptr->next = current_ptr;
            prev_ptr = current_ptr;
        }
        if (initial_list.size() > 0) {
            current_ptr->next = m_tail;
            m_tail->prev = current_ptr;
        }
    }
    ~DoublyLinkedList() {
        clear();
        delete m_head;
    }

    Iterator begin() const {
        return m_head->next == nullptr ? Iterator {m_head} : Iterator {m_head->next};
    }

    Iterator end() const {
        return Iterator {m_tail};
    }

    Iterator find(const ValueType& value) const {
        auto ptr = m_head->next;
        while (ptr && ptr != m_tail) {
            if (ptr->value == value) {
                return Iterator {ptr};
            }
            ptr = ptr->next;
        }
    }

    Iterator find(const ValueType& value, const std::function<bool(const ValueType&, const ValueType&)>& comp_fun) const {
        auto ptr = m_head->next;
        while (ptr && ptr != m_tail) {
            if (comp_fun(ptr->value, value)) {
                return Iterator {ptr};
            }
            ptr = ptr->next;
        }
    }

    bool empty() const {
        return m_size == 0;
    }

    SizeType size() const {
        return m_size;
    }

    Iterator insert(Iterator& it, ValueType value) {
        if (it) {
            auto ptr = new Node;
            ptr->value = value;
            ptr->prev = it.m_node_ptr;
            ptr->next = it.m_node_ptr->next;
            it.m_node_ptr->next->prev = ptr;
            it.m_node_ptr->next = ptr;
            m_size++;
            return ptr;
        }
        return nullptr;
    }

    Iterator emplace(Iterator& it, ValueType&& value) {
        if (it) {
            auto ptr = new Node;
            ptr->value = std::forward<ValueType>(value);
            ptr->prev = it.m_node_ptr->prev;
            it.m_node_ptr->prev->next = ptr;
            ptr->next = it.m_node_ptr->next;
            it.m_node_ptr->next->prev = ptr;
            m_size++;
            return ptr;
        }
        return nullptr;
    }

    void erase(Iterator& it) {
        if (it) {
            it.m_node_ptr->prev->next = it.m_node_ptr->next;
            it.m_node_ptr->next->prev = it.m_node_ptr->prev;
            delete it.m_node_ptr;
            it.m_node_ptr = nullptr;
            m_size--;
        }
    }

    void clear() {
        auto ptr = m_head->next;
        while (ptr) {
            auto next = ptr->next;
            delete ptr;
            ptr = nullptr;
            ptr = next;
        }
    }

private:
    SizeType m_size {0};
    Node* m_head {nullptr};
    Node* m_tail {nullptr};
};

