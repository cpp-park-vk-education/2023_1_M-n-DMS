#include <head.hpp>

#include <iostream>

// // Define a fixture for the tests
// class DatabaseTest : public ::testing::Test {
// protected:
//     // Create a test database and connect to it
//     void SetUp() override {
//         db = new Database("testdb", "testuser", "testpassword", "localhost", "5432");
//     }

//     // Disconnect from the test database and delete the object
//     void TearDown() override {
//         delete db;
//     }

//     Database* db;
// };

// // Test the ExecuteQuery method
// TEST_F(DatabaseTest, ExecuteQuery) {
//     // Execute a query that should return a result
//     int result = db->ExecuteQuery("SELECT * FROM test_table WHERE id = 1");

//     // Check that the query returned 0
//     EXPECT_EQ(result, 0);

//     // Execute a broken query
//     result = db->ExecuteQuery("sdfsjfsofsdf");

//     // Check that the query returned 1
//     EXPECT_EQ(result, 1);
// }

// // Test the ExecuteCommand method
// TEST_F(DatabaseTest, ExecuteCommand) {
//     // Insert a row into the test table
//     int result = db->ExecuteCommand("INSERT INTO test_table VALUES (1, 'test')");

//     // Check that the insert succeeded
//     EXPECT_EQ(result, 0);

//     // Delete the row from the test table
//     result = db->ExecuteCommand("DELETE FROM test_table WHERE id = 1");

//     // Check that the delete succeeded
//     EXPECT_EQ(result, 0);
// }

// // Test the GetResult method
// TEST_F(DatabaseTest, GetResult) {
//     // Execute a query that should return a result
//     db->ExecuteQuery("SELECT * FROM test_table WHERE id = 1");

//     // Get the result of the query
//     int result = db->GetResult(); // pqxx::result result = db->GetResult();

//     // Check that the result is not empty
//     EXPECT_EQ(result, 1); // EXPECT_FALSE(result.empty());
// }


// // Define a fixture for the tests
// class HandlerTest : public ::testing::Test {
// protected:
//     // Create a test handler and connect to it
//     void SetUp() override {
//         boost::asio::io_context io_context;
//         handler = new Handler("1", "1", "1", "1", "1", io_context);
//     }

//     // Disconnect from the test handler and delete the object
//     void TearDown() override {
//         delete handler;
//     }

//     Handler* handler;
// };

// // Test the MakeDecision method
// TEST_F(HandlerTest, MakeDecision) {
//     // Call the MakeDecision method
//     int decision = handler->MakeDecision();

//     // Check that the decision is valid
//     EXPECT_TRUE(decision == 0 || decision == 1);
// }

// // Test the GetID method
// TEST_F(HandlerTest, GetID) {
//     // Call the GetID method
//     int id = handler->GetID();

//     // Check that the ID is greater than or equal to 0
//     EXPECT_GE(id, 0);
// }

// // Test the WriteInDB method
// TEST_F(HandlerTest, WriteInDB) {
//     // Call the WriteInDB method
//     int rows = handler->WriteInDB();

//     // Check that the write succeeded
//     EXPECT_EQ(rows, 1);
// }

// // Test the FindInDB method
// TEST_F(HandlerTest, FindInDB) {
//     // Call the FindInDB method
//     std::string result = handler->FindInDB();

//     // Check that the result is not empty
//     EXPECT_FALSE(result.empty());
// }


// // Define a fixture for the tests
// class HeadServerTest : public ::testing::Test {
// protected:
//     // Create a test head server and connect to it
//     void SetUp() override {
//         boost::asio::io_context io_context;
//         server = new HeadServer("1", "1", "1", "1", "1", io_context);
//     }

//     // Disconnect from the test head server and delete the object
//     void TearDown() override {
//         delete server;
//     }

//     HeadServer* server;
// };

// // Test the Listen method
// TEST_F(HeadServerTest, Listen) {
//     // Call the Listen method
//     int result = server->Listen();

//     // Check that the result is valid
//     EXPECT_TRUE(result == 0 || result == 1);
// }

// // Test the SetAcceptor method
// TEST_F(HeadServerTest, SetAcceptor) {
//     // Call the SetAcceptor method
//     int result = server->SetAcceptor(1234);

//     // Check that the result is valid
//     EXPECT_TRUE(result == 0 || result == 1);
// }

// // Run the tests
// int main(int argc, char** argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }

int main() {
    HeadServer server(8080);
    server.Listen();
    return 0;
}
