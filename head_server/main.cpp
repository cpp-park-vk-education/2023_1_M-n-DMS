#include <head.hpp>

#include <iostream>

// Define a fixture for the tests
class DatabaseTest : public ::testing::Test {
protected:
    // Create a test database and connect to it
    void SetUp() override {
        db = new Database("testdb", "testuser", "testpassword", "localhost", "5432");
    }

    // Disconnect from the test database and delete the object
    void TearDown() override {
        delete db;
    }

    Database* db;
};

// Test the ExecuteQuery method
TEST_F(DatabaseTest, ExecuteQuery) {
    // Execute a query that should return a result
    int result = db->ExecuteQuery("SELECT * FROM test_table WHERE id = 1");

    // Check that the query returned 0
    EXPECT_EQ(result, 0);

    // Execute a broken query
    result = db->ExecuteQuery("sdfsjfsofsdf");

    // Check that the query returned 1
    EXPECT_EQ(result, 1);
}

// Test the ExecuteCommand method
TEST_F(DatabaseTest, ExecuteCommand) {
    // Insert a row into the test table
    int result = db->ExecuteCommand("INSERT INTO test_table VALUES (1, 'test')");

    // Check that the insert succeeded
    EXPECT_EQ(result, 0);

    // Delete the row from the test table
    result = db->ExecuteCommand("DELETE FROM test_table WHERE id = 1");

    // Check that the delete succeeded
    EXPECT_EQ(result, 0);
}

// Test the GetResult method
TEST_F(DatabaseTest, GetResult) {
    // Execute a query that should return a result
    db->ExecuteQuery("SELECT * FROM test_table WHERE id = 1");

    // Get the result of the query
    int result = db->GetResult(); // pqxx::result result = db->GetResult();

    // Check that the result is not empty
    EXPECT_EQ(result, 1); // EXPECT_FALSE(result.empty());
}

// Run the tests
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
