#include <memory>
#include <list>
#include <mutex>

namespace detail
{
    // wrapper to extend container object lifetime
    template <typename T>
    struct shared_pool_t : public std::enable_shared_from_this<shared_pool_t<T>>
    {
        std::mutex cs;
        std::list<std::unique_ptr<T>> data;
    };

    // finalizer for post-aquire routine
    template <typename T>
    struct finalizer_t
    {
        using shared_pool_type = shared_pool_t<T>;
        finalizer_t(std::weak_ptr<shared_pool_type> wp)
            : _wp(wp) {}

        void operator()(T* ptr)
        {
            if (auto pool = _wp.lock())
            {
                pool->data.push_back(std::move(std::unique_ptr<T>(ptr)));
            }
        }

        std::weak_ptr<shared_pool_type> _wp;
    };
}

template <typename T>
class pool_t final
{
public:
    using shared_pool_type = detail::shared_pool_t<T>;
    using value_type = std::unique_ptr<T, detail::finalizer_t<T>>;

    pool_t()
        : _obj_pool(std::make_shared<shared_pool_type>())
    {
    }

    template <typename...Args>
    typename std::list<std::unique_ptr<T>>::iterator add(Args...args)
    {
        // memory leak is possble in case of std::bad_alloc
        if constexpr (std::is_constructible_v<T, Args...>)
        {
            // try constructor to create T, no deleter
            std::unique_ptr<T> sp(new T(args...));
            return add(std::move(sp));
        }
        else
        {
            // try designated initialization to construct T, no deleter
            std::unique_ptr<T> sp(new T{ args... });
            return add(std::move(sp));
        }
    }

    typename std::list<std::unique_ptr<T>>::iterator add(std::unique_ptr<T> sp)
    {
        return _obj_pool->data.emplace(_obj_pool->data.end(), std::move(sp));
    }

    value_type aquire()
    {
        std::weak_ptr<shared_pool_type> wp(_obj_pool);

        std::lock_guard<std::mutex> lock(_obj_pool->cs);
        if (_obj_pool->data.empty())
            return value_type(nullptr, detail::finalizer_t<T>(wp));

        // make a copy of the element in the list
        value_type sp(_obj_pool->data.front().release(), detail::finalizer_t<T>(wp));

        // remove first element from the list
        _obj_pool->data.pop_front();

        return sp;
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> lock(_obj_pool->cs);
        return _obj_pool->data.size();
    }

private:
    std::shared_ptr<shared_pool_type> _obj_pool;
};