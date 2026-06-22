template<typename K, typename V>
HashTable<K, V>::HashTable(): size_(0), bucket_count_(8)
{
    buckets_.resize(bucket_count_, nullptr);
}

template<typename K, typename V>
size_t HashTable<K, V>::hash(const K& key) const
{
    return std::hash<K>{}(key) % bucket_count_;
}

template<typename K, typename V>
HashTable<K, V>::~HashTable()
{
    for (Node* head : buckets_)
    {
        while (head)
        {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
}

template<typename K, typename V>
void HashTable<K, V>::insert(const K& key, const V& value)
{
    if(loadFactor() > 0.75)
    {
        rehash(bucket_count_ * 2);
    }

    size_t index = hash(key);
    Node* current = buckets_[index];
    while (current)
    {
        if (current->key == key)
        {
            current->value = value;
            return;
        }
        current = current->next;
    }
    Node* new_node = new Node(key, value);
    new_node->next = buckets_[index];
    buckets_[index] = new_node;
    ++size_;
}

template<typename K, typename V>
bool HashTable<K, V>::contains(const K& key) const
{
    size_t index = hash(key);
    Node* current = buckets_[index];
    while (current)
    {
        if (current->key == key)
        {
            return true;
        }
        current = current->next;
    }
    return false;
}

template<typename K, typename V>
V* HashTable<K, V>::find(const K& key)
{
    size_t index = hash(key);
    Node* current = buckets_[index];
    while (current)
    {
        if (current->key == key)
        {
            return &current->value;
        }
        current = current->next;
    }
    return nullptr;
}

template<typename K, typename V>
bool HashTable<K, V>::erase(const K& key)
{
    size_t index = hash(key);
    Node* current = buckets_[index];
    Node* previous = nullptr;
    while (current)
    {
        if (current->key == key)
        {
            if (previous == nullptr)
            {
                buckets_[index] = current->next;
            }
            else
            {
                previous->next = current->next;
            }
            delete current;
            --size_;
            return true;
        }
        previous = current;
        current = current->next;
    }
    return false;
}

template<typename K, typename V>
size_t HashTable<K, V>::size() const
{
    return size_;
}

template<typename K, typename V>
void HashTable<K, V>::debugPrint() const
{
    for (size_t i = 0; i < bucket_count_; ++i)
    {
        std::cout << "Bucket " << i << ": ";
        Node* current = buckets_[i];
        while (current)
        {
            std::cout << "(" << current->key << ", " << current->value << ") -> ";
            current = current->next;
        }
        std::cout << "nullptr\n";
    }
}

template<typename K, typename V>
double HashTable<K,V>::loadFactor() const
{
    return static_cast<double>(size_) / bucket_count_;
}

template<typename K, typename V>
void HashTable<K,V>::rehash(size_t new_bucket_count)
{
    std::vector<Node*> old_buckets = buckets_;
    buckets_.clear();
    buckets_.resize(new_bucket_count,nullptr);
    std::cout << "Rehashing from " << bucket_count_ << " to " << new_bucket_count << std::endl;
    bucket_count_ = new_bucket_count;
    size_ = 0;
    for (Node* head : old_buckets)
    {
        while (head)
        {
            insert(head->key,head->value);
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
}

template<typename K, typename V>
size_t HashTable<K,V>::bucketCount() const
{
    return bucket_count_;
}

template<typename K, typename V>
std::vector<K> HashTable<K, V>::keys() const
{
    std::vector<K> result;
    for (Node* head : buckets_)
    {
        Node* current = head;
        while (current)
        {
            result.push_back(current->key);
            current = current->next;
        }
    }

    return result;
}

template<typename K, typename V>
std::vector<std::pair<K,V>> HashTable<K,V>::items() const
{
    std::vector<std::pair<K,V>> result;
    for(Node* head : buckets_)
    {
        Node* current = head;
        while(current)
        {
            result.push_back({current->key, current->value});
            current = current->next;
        }
    }
    return result;
}

