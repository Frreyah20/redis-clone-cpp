#include <gtest/gtest.h>
#include <thread>
#include <chrono>

#include "../src/database/Database.h"
#include "../src/persistence/PersistenceManager.h"
#include "../src/parser/CommandParser.h"
#include <cstdio>

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

TEST(PersistenceTest, SaveAndLoadSnapshot)
{
    Database db;
    PersistenceManager pm;
    db.set("name", "frreyah");
    db.set("role", "quant");
    EXPECT_TRUE(pm.saveSnapshot(db, "test.rdb"));
    Database loadedDb;
    EXPECT_TRUE(pm.loadSnapshot(loadedDb, "test.rdb"));
    EXPECT_EQ(loadedDb.get("name"), "frreyah");
    EXPECT_EQ(loadedDb.get("role"), "quant");
    std::remove("test.rdb");
}

TEST(CommandParserTest, Ping)
{
    Database db;
    PersistenceManager pm;
    CommandParser parser;
    EXPECT_EQ(parser.execute("PING", db, pm),"PONG\n");
}

TEST(CommandParserTest, SetCommand)
{
    Database db;
    PersistenceManager pm;
    CommandParser parser;
    EXPECT_EQ(parser.execute("SET name frreyah",db,pm),"OK\n");
    EXPECT_EQ(db.get("name"),"frreyah");
}

TEST(CommandParserTest, GetCommand)
{
    Database db;
    PersistenceManager pm;
    CommandParser parser;
    db.set("name", "frreyah");
    EXPECT_EQ(parser.execute("GET name",db,pm),"frreyah\n");
}