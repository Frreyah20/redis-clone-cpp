#include <gtest/gtest.h>
#include <thread>
#include <chrono>

#include "../src/database/Database.h"

TEST(DatabaseTest, SetAndGet)
{
    Database db;
    db.set("name", "frreyah");
    EXPECT_EQ(db.get("name"),"frreyah");
}

TEST(DatabaseTest, Exists)
{
    Database db;
    db.set("name", "frreyah");
    EXPECT_TRUE(db.exists("name"));
    EXPECT_FALSE(db.exists("missing"));
}

TEST(DatabaseTest, DeleteKey)
{
    Database db;
    db.set("name", "frreyah");
    EXPECT_TRUE(db.del("name"));
    EXPECT_FALSE(db.exists("name"));
}

TEST(DatabaseTest, ExpireKey)
{
    Database db;
    db.set("temp", "value");
    EXPECT_TRUE(db.expire("temp", 1));
}

TEST(DatabaseTest, TTLExpiration)
{
    Database db;
    db.set("temp", "value");
    db.expire("temp", 1);
    EXPECT_TRUE(db.exists("temp"));
    std::this_thread::sleep_for(std::chrono::seconds(2));
    EXPECT_FALSE(db.exists("temp"));
}