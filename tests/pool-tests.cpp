#include <gtest/gtest.h>
#include "pool.h"

TEST(ObjectPool, CreatePool)
{
    pool_t<int> p;
    EXPECT_EQ(p.size(), 0);
}

TEST(ObjectPool, AddAquire)
{
    struct aux
    {
        int i{ 0 };
        double d{ 0.0 };
    };

    pool_t<aux> p;
    auto sp = std::move(*(p.add(1, 2.0)));

    EXPECT_EQ(p.size(), 1);

    auto aq = p.aquire();
    EXPECT_FALSE(aq);
    EXPECT_EQ(p.size(), 0);
}

TEST(ObjectPool, AddSimplePod)
{
    struct aux
    {
        int i{ 0 };
        double d{ 0.0 };
    };
    pool_t<aux> p;
    auto it = p.add(1, 2.0);
    EXPECT_EQ(p.size(), 1);
}

TEST(ObjectPool, AddSimpleConstructible)
{
    struct aux
    {
        aux(int i_, double d_) : i(i_), d(d_) {}
        int i{ 0 };
        double d{ 0.0 };
    };
    pool_t<aux> p;
    auto it = (p.add(1, 2.0));
    EXPECT_EQ(p.size(), 1);
}

TEST(ObjectPool, AquireSimple)
{
    struct aux
    {
        int i{ 0 };
        double d{ 0.0 };
    };

    pool_t<aux> p;
    p.add(1, 2.0);
    auto obj = std::move(p.aquire());
    EXPECT_EQ(obj->i, 1);
    EXPECT_EQ(obj->d, 2.0);
}

TEST(ObjectPool, AquireSimpleConstructible)
{
    struct aux
    {
        aux(int i_, double d_) : i(i_), d(d_) {}
        int i{ 0 };
        double d{ 0.0 };
    };

    pool_t<aux> p;
    p.add(1, 2.0);
    auto obj = std::move(p.aquire());
    EXPECT_EQ(obj->i, 1);
    EXPECT_EQ(obj->d, 2.0);
}

TEST(ObjectPool, PoolLifetime1)
{
    struct aux
    {
        int i{ 0 };
        double d{ 0.0 };
    };

    std::shared_ptr<aux> sp;

    {
        pool_t<aux> p;
        sp = std::move(*(p.add(1, 2.0)));
    }

    EXPECT_TRUE(sp);
}

TEST(ObjectPool, RemoveUnhealthy)
{
    struct aux
    {
        aux(int i_, double d_) : i(i_), d(d_) {}

        int i{ 0 };
        double d{ 0.0 };
    };

    pool_t<aux> p;
    auto sp1 = p.add(1, 2.0);
    auto sp2 = p.add(2, 3.0);

    p.aquire();
    auto obj = std::move(p.aquire());
    EXPECT_EQ(obj->i, 2);
    EXPECT_EQ(obj->d, 3.0);
}
