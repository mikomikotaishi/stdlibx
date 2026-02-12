/**
 * @file sql_example.cpp
 * @brief Example usage of the JDBC-like ODBC wrapper.
 *
 * This file demonstrates how to use the SQL module to connect to databases
 * and execute queries in a JDBC-style manner.
 */

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
import stdx;
#else
import stdlibx;
#endif

#include <iostream>

#ifdef STDLIBX_NO_RESERVED_STD_NAMESPACE
using namespace stdx::sql;
#else
using namespace stdlibx::sql;
#endif

int main() {
    try {
        // Example 1: Connect using a connection string
        auto conn = DriverManager::getConnection(
            "Driver={SQL Server};Server=localhost;Database=testdb;Uid=user;Pwd=password;"
        );

        std::cout << "Connected successfully!" << std::endl;

        // Example 2: Create and execute a simple query
        {
            auto stmt = conn.createStatement();
            auto rs = stmt.executeQuery("SELECT id, name, age FROM users");

            std::cout << "\n=== Query Results ===" << std::endl;
            while (rs.next()) {
                auto id = rs.getInt(1);
                auto name = rs.getString(2);
                auto age = rs.getInt(3);

                std::cout << "ID: " << id.value_or(0)
                         << ", Name: " << name.value_or("NULL")
                         << ", Age: " << age.value_or(0) << std::endl;
            }
        }

        // Example 3: Execute an update
        {
            auto stmt = conn.createStatement();
            int rowsAffected = stmt.executeUpdate(
                "UPDATE users SET age = 30 WHERE name = 'John'"
            );
            std::cout << "\nRows updated: " << rowsAffected << std::endl;
        }

        // Example 4: Use prepared statements
        {
            auto pstmt = conn.prepareStatement(
                "INSERT INTO users (name, age, email) VALUES (?, ?, ?)"
            );

            pstmt.setString(1, "Alice");
            pstmt.setInt(2, 25);
            pstmt.setString(3, "alice@example.com");

            int rowsInserted = pstmt.executeUpdate();
            std::cout << "\nRows inserted: " << rowsInserted << std::endl;
        }

        // Example 5: Prepared statement with query
        {
            auto pstmt = conn.prepareStatement(
                "SELECT name, age FROM users WHERE age > ?"
            );

            pstmt.setInt(1, 20);

            auto rs = pstmt.executeQuery();
            std::cout << "\n=== Users over 20 ===" << std::endl;
            while (rs.next()) {
                auto name = rs.getString(1);
                auto age = rs.getInt(2);
                std::cout << "Name: " << name.value_or("NULL")
                         << ", Age: " << age.value_or(0) << std::endl;
            }
        }

        // Example 6: Transaction management
        conn.setAutoCommit(false);
        try {
            auto stmt = conn.createStatement();
            stmt.executeUpdate("UPDATE accounts SET balance = balance - 100 WHERE id = 1");
            stmt.executeUpdate("UPDATE accounts SET balance = balance + 100 WHERE id = 2");
            
            conn.commit();
            std::cout << "\nTransaction committed successfully!" << std::endl;
        } catch (const SQLException& e) {
            conn.rollback();
            std::cout << "\nTransaction rolled back: " << e.getMessage() << std::endl;
        }
        conn.setAutoCommit(true);

        // Example 7: Database-specific connections
        {
            // PostgreSQL
            auto pgConn = DriverManager::getPostgreSQLConnection(
                "localhost", "mydb", "user", "password", 5432
            );
            std::cout << "\nConnected to PostgreSQL!" << std::endl;
        }

        {
            // MySQL
            auto mysqlConn = DriverManager::getMySQLConnection(
                "localhost", "mydb", "user", "password", 3306
            );
            std::cout << "Connected to MySQL!" << std::endl;
        }

        // Example 8: ResultSet metadata
        {
            auto stmt = conn.createStatement();
            auto rs = stmt.executeQuery("SELECT * FROM users LIMIT 1");

            std::cout << "\n=== Column Information ===" << std::endl;
            int columnCount = rs.getColumnCount();
            for (int i = 1; i <= columnCount; ++i) {
                std::cout << "Column " << i << ": "
                         << rs.getColumnName(i) << std::endl;
            }
        }

        // Connection will be automatically closed when it goes out of scope
        conn.close();
        std::cout << "\nConnection closed." << std::endl;

    } catch (const SQLException& e) {
        std::cerr << "SQL Error: " << e.getMessage() << std::endl;
        std::cerr << "SQLSTATE: " << e.getSQLState() << std::endl;
        if (auto code = e.getErrorCode()) {
            std::cerr << "Error Code: " << code.value() << std::endl;
        }
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
