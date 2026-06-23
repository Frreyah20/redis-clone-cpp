#include <gtest/gtest.h>

#include "../src/database/Database.h"

TEST(DatabaseTest, SetAndGet)
{
    Database db;
    db.set("name", "frreyah");
    EXPECT_EQ(db.get("name"),"frreyah");
}