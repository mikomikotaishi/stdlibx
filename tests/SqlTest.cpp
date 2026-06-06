#include <version>

import stdx;

#ifdef STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY
using stdx::collections::Vector;
using stdx::fs::Path;
using stdx::linq::Query;
using stdx::sql::Bindable;
using stdx::sql::Connection;
using stdx::sql::DriverManager;
using stdx::sql::PreparedStatement;
using stdx::sql::ResultSet;
using stdx::sql::SQLException;
using stdx::sql::Statement;
#endif

#ifdef __GNUC__
using namespace stdx::core;
using namespace stdx::literals;
#endif

#ifdef STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY
static_assert(Bindable<i32>);
static_assert(Bindable<i64>);
static_assert(Bindable<f64>);
static_assert(Bindable<bool>);
static_assert(Bindable<NullPointer>);
static_assert(Bindable<const char*>);
static_assert(Bindable<String>);
static_assert(Bindable<StringView>);
static_assert(!Bindable<Vector<i32>>);
static_assert(!Bindable<Path>);

struct TestContext {
    i32 passed = 0;
    i32 failed = 0;

    void check(StringView name, bool condition) {
        if (condition) {
            System::out.println("[PASS] {}", name);
            ++passed;
        } else {
            System::err.println("[FAIL] {}", name);
            ++failed;
        }
    }
};

/**
 * @brief Tests that attempting to connect with a bogus driver string throws SQLException,
 * rather than crashing or doing something else.
 * 
 * @param ctx TestContext to record pass/fail results.
 *
 * Negative-path tests: these run unconditionally because they don't need a
 * driver - they verify the API throws the right exception when the driver
 * stack can't satisfy the connection request.
 */
void test_bad_connection(TestContext& ctx) {
    bool threw = false;
    try {
        Connection c = DriverManager::connection(
            "Driver={ThisDriverDoesNotExist};Server=nope;"
        );
        // Unreachable in a sane environment, but if some test driver IS
        // installed under that name we just close it cleanly and count
        // the throw-check as a soft fail.
        c.close();
    } catch ([[maybe_unused]] const SQLException& e) {
        threw = true;
    }
    ctx.check("bogus driver connection throws SQLException", threw);
}

/**
 * @brief Tests that an empty connection string throws SQLException, 
 * rather than crashing or doing something else.
 * 
 * @param ctx TestContext to record pass/fail results.
 */
void test_empty_connection_string(TestContext& ctx) {
    bool threw = false;
    try {
        Connection c = DriverManager::connection("");
        c.close();
    } catch ([[maybe_unused]] const SQLException& e) {
        threw = true;
    }
    ctx.check("empty connection string throws SQLException", threw);
}

/**
 * @brief Attempts to open a live connection to a database file.
 * 
 * @param dbfile Path to a database file (e.g. SQLite) that the test can create and use.
 * Ignored if testing against a server-based database.
 * @return Optional<Connection> A live Connection if one could be established,
 * or nullopt if no driver could connect.
 *
 * Live-mode tests: opportunistically connect to whatever database is reachable.
 * We try a small list of connection strings and use the first that works; if
 * none connect, the live tests are skipped. The binding/CRUD code is driver-
 * agnostic (it all goes through ODBC), so any of these exercises it equally.
 *
 *   - Microsoft SQL Server, e.g. a local Docker container:
 *       docker run -e ACCEPT_EULA=Y -e MSSQL_SA_PASSWORD=Stdx#Passw0rd \
 *         -p 1433:1433 -d mcr.microsoft.com/mssql/server:2022-latest
 *     The password is read from STDX_MSSQL_PASSWORD (default below). Driver 18
 *     encrypts by default; against a dev container's self-signed certificate we
 *     opt out. MARS_Connection lets the connection hold >1 active result set.
 *   - SQLite (file-based), via libsqliteodbc under any of its common names.
 */
[[nodiscard]]
Optional<Connection> try_open_live(const Path& dbfile) noexcept {
    Vector<String> connection_strings;

    const Optional<StringView> mssql_password = Environment::get("STDX_MSSQL_PASSWORD");
    connection_strings.push_back(stdx::fmt::format(
        "Driver={{ODBC Driver 18 for SQL Server}};Server=127.0.0.1,1433;"
        "UID=sa;PWD={};Encrypt=no;TrustServerCertificate=yes;MARS_Connection=yes;",
        mssql_password.has_value() ? *mssql_password : StringView("Stdx#Passw0rd")
    ));

    static constexpr Array<StringView, 3> SQLITE_DRIVERS = {
        "SQLite3",
        "SQLite3 ODBC Driver",
        "SQLite",
    };
    for (StringView driver: SQLITE_DRIVERS) {
        connection_strings.push_back(stdx::fmt::format(
            "Driver={{{}}};Database={};", driver, dbfile.string()
        ));
    }

    for (const String& connection_string: connection_strings) {
        try {
            return DriverManager::connection(connection_string);
        } catch ([[maybe_unused]] const SQLException& e) {
            // Try the next candidate.
        }
    }
    return nullopt;
}

/**
 * @brief Tests that we can connect to a live database and perform basic CRUD operations.
 * 
 * @param ctx TestContext to record pass/fail results.
 * @param c A live Connection to a test database.
 * The test creates and drops its own table, so the schema doesn't matter.
 */
void test_crud(TestContext& ctx, Connection& c) {
    // DDL goes through a plain Statement - there are no values to bind. The
    // VARCHAR(255) column type is portable across SQL Server and SQLite.
    Statement ddl = c.create_statement();
    (void)ddl.execute("DROP TABLE IF EXISTS stdx_test_users;");
    (void)ddl.execute(
        "CREATE TABLE stdx_test_users ("
        "  id INTEGER PRIMARY KEY,"
        "  name VARCHAR(255) NOT NULL,"
        "  age INTEGER"
        ");"
    );

    // Variadic parameter binding: prepare once, then execute with values.
    // Each argument is bound positionally to its `?` marker and dispatched on
    // its C++ type (i32 -> INTEGER, const char* -> VARCHAR), so it is sent as
    // data, never spliced into SQL.
    PreparedStatement insert = c.prepare_statement(
        "INSERT INTO stdx_test_users (id, name, age) VALUES (?, ?, ?);"
    );
    const i32 inserted_alice = insert.execute_update(1, "Alice", 30);
    const i32 inserted_bob = insert.execute_update(2, "Bob", 25);
    ctx.check("INSERT Alice returns 1 row affected", inserted_alice == 1);
    ctx.check("INSERT Bob returns 1 row affected", inserted_bob == 1);

    // Variadic execute_query: a parameterised point lookup.
    {
        PreparedStatement by_id = c.prepare_statement(
            "SELECT id, name, age FROM stdx_test_users WHERE id = ?;"
        );
        ResultSet rs = by_id.execute_query(1);
        ctx.check("parameterised SELECT finds a row", rs.next());
        const Optional<String> name = rs.get_string(2);
        const Optional<i32> age = rs.get_int(3);
        ctx.check("looked-up name == \"Alice\"", name.has_value() && *name == "Alice");
        ctx.check("looked-up age == 30", age.has_value() && *age == 30);
    }

    // Full ordered scan via a plain Statement (no parameters needed).
    {
        Statement scan = c.create_statement();
        ResultSet rs = scan.execute_query(
            "SELECT id, name, age FROM stdx_test_users ORDER BY id ASC;"
        );

        bool got_alice = rs.next();
        ctx.check("first SELECT row available", got_alice);
        if (got_alice) {
            const Optional<i32> id = rs.get_int(1);
            const Optional<String> name = rs.get_string(2);
            const Optional<i32> age = rs.get_int(3);
            ctx.check("row 1 id == 1", id.has_value() && *id == 1);
            ctx.check("row 1 name == \"Alice\"", name.has_value() && *name == "Alice");
            ctx.check("row 1 age == 30", age.has_value() && *age == 30);
        }

        bool got_bob = rs.next();
        ctx.check("second SELECT row available", got_bob);
        if (got_bob) {
            const Optional<i32> id = rs.get_int(1);
            const Optional<String> name = rs.get_string(2);
            const Optional<i32> age = rs.get_int(3);
            ctx.check("row 2 id == 2", id.has_value() && *id == 2);
            ctx.check("row 2 name == \"Bob\"", name.has_value() && *name == "Bob");
            ctx.check("row 2 age == 25", age.has_value() && *age == 25);
        }

        ctx.check("third row absent (end of result set)", !rs.next());
    }

    // Parameterised UPDATE: bump every age by a bound amount.
    PreparedStatement bump = c.prepare_statement(
        "UPDATE stdx_test_users SET age = age + ? WHERE age >= ?;"
    );
    const i32 updated = bump.execute_update(1, 0);
    ctx.check("UPDATE affects 2 rows", updated == 2);

    (void)ddl.execute("DROP TABLE stdx_test_users;");
}

/**
 * @brief Tests that parameter binding is safe against SQL injection.
 * 
 * @param ctx TestContext to record pass/fail results.
 * @param c A live Connection to a test database. The test creates
 * and drops its own table, so the schema doesn't matter.
 *
 * Proves the variadic binding path is injection-safe: a malicious value bound
 * as a parameter is stored as literal data, never executed as SQL.
 */
void test_sql_injection_safe(TestContext& ctx, Connection& c) {
    Statement ddl = c.create_statement();
    (void)ddl.execute("DROP TABLE IF EXISTS stdx_inj;");
    (void)ddl.execute(
        "CREATE TABLE stdx_inj (id INTEGER PRIMARY KEY, name VARCHAR(255));"
    );

    // The classic "Little Bobby Tables" payload, supplied as a bound parameter.
    static constexpr StringView payload = "Robert'); DROP TABLE stdx_inj;--";
    {
        PreparedStatement insert = c.prepare_statement(
            "INSERT INTO stdx_inj (id, name) VALUES (?, ?);"
        );
        const i32 affected = insert.execute_update(1, payload);
        ctx.check("parameterised insert affects 1 row", affected == 1);
    }

    {
        Statement query = c.create_statement();
        ResultSet rs = query.execute_query("SELECT name FROM stdx_inj WHERE id = 1;");
        const bool found = rs.next();
        // 1. The table still exists - the embedded DROP never ran.
        ctx.check("table survived injection attempt", found);
        if (found) {
            // 2. The payload round-trips verbatim - treated as data, not SQL.
            const Optional<String> name = rs.get_string(1);
            ctx.check(
                "injection payload stored verbatim as data",
                name.has_value() && *name == payload
            );
        }
    }

    (void)ddl.execute("DROP TABLE stdx_inj;");
}

/**
 * @brief Tests the Connection-level convenience methods for executing SQL with optional parameters.
 * 
 * @param ctx TestContext to record pass/fail results.
 * @param c A live Connection to a test database.
 * The test creates and drops its own table, so the schema doesn't matter.
 *
 * Exercises the Connection-level convenience: query()/execute() with optional
 * bound parameters and no statement object to manage. Also verifies that the
 * ResultSet from query() owns its handle and stays valid after the one-shot
 * statement is gone (ASan/UBSan would catch a dangling borrow here).
 */
void test_connection_convenience(TestContext& ctx, Connection& c) {
    // execute() with no parameters runs DDL directly (SQLExecDirect path).
    (void)c.execute("DROP TABLE IF EXISTS stdx_conv;");
    (void)c.execute("CREATE TABLE stdx_conv (id INTEGER PRIMARY KEY, name VARCHAR(255));");

    // execute() with bound parameters - no PreparedStatement to juggle.
    const i32 n1 = c.execute("INSERT INTO stdx_conv (id, name) VALUES (?, ?);", 1, "Ada");
    const i32 n2 = c.execute("INSERT INTO stdx_conv (id, name) VALUES (?, ?);", 2, "Linus");
    ctx.check("conn.execute INSERT affects 1 row (Ada)", n1 == 1);
    ctx.check("conn.execute INSERT affects 1 row (Linus)", n2 == 1);

    // query() with a bound parameter. The returned ResultSet outlives the
    // internal statement that produced it.
    {
        ResultSet rs = c.query("SELECT name FROM stdx_conv WHERE id = ?;", 2);
        ctx.check("conn.query finds bound row", rs.next());
        const Optional<String> name = rs.get_string(1);
        ctx.check("conn.query row name == \"Linus\"", name.has_value() && *name == "Linus");
    }

    // query() with no parameters.
    {
        ResultSet rs = c.query("SELECT COUNT(*) FROM stdx_conv;");
        ctx.check("conn.query (no params) returns a row", rs.next());
        const Optional<i32> count = rs.get_int(1);
        ctx.check("conn.query COUNT(*) == 2", count.has_value() && *count == 2);
    }

    (void)c.execute("DROP TABLE stdx_conv;");
}

/**
 * @brief Tests that ResultSet can be used as a single-pass input range.
 * 
 * @param ctx TestContext to record pass/fail results.
 * @param c A live Connection to a test database.
 * The test creates and drops its own table, so the schema doesn't matter.
 *
 * Exercises ResultSet as a single-pass input range: a range-based for loop and
 * a stdx::linq::Query pipeline (filter + project) over the live rows.
 */
void test_result_set_range(TestContext& ctx, Connection& c) {
    (void)c.execute("DROP TABLE IF EXISTS stdx_range;");
    (void)c.execute("CREATE TABLE stdx_range (id INTEGER PRIMARY KEY, name VARCHAR(255));");
    (void)c.execute("INSERT INTO stdx_range (id, name) VALUES (?, ?);", 1, "Grace");
    (void)c.execute("INSERT INTO stdx_range (id, name) VALUES (?, ?);", 2, "Edsger");
    (void)c.execute("INSERT INTO stdx_range (id, name) VALUES (?, ?);", 3, "Barbara");

    // Range-based for over an owning ResultSet temporary.
    {
        i32 rows = 0;
        i32 id_sum = 0;
        for (ResultSet::Row row: c.query("SELECT id, name FROM stdx_range ORDER BY id ASC;")) {
            ++rows;
            id_sum += row.get<i32>(1).value_or(0);
        }
        ctx.check("range-for visited all 3 rows", rows == 3);
        ctx.check("range-for summed ids (1+2+3 == 6)", id_sum == 6);
    }

    // LINQ over the live result set: filter (id >= 2) then project the name.
    {
        ResultSet rs = c.query("SELECT id, name FROM stdx_range ORDER BY id ASC;");
        Vector<String> names = Query(rs)
            .where([](ResultSet::Row row) -> bool {
                return row.get<i32>(1).value_or(0) >= 2;
            })
            .select([](ResultSet::Row row) -> String {
                return row.get<String>(2).value_or("");
            })
            .to<Vector<String>>();
        ctx.check("LINQ where+select kept 2 rows", names.size() == 2);
        ctx.check(
            "LINQ first projected name == \"Edsger\"",
            names.size() >= 1 && names[0] == "Edsger"
        );
        ctx.check(
            "LINQ second projected name == \"Barbara\"",
            names.size() >= 2 && names[1] == "Barbara"
        );
    }

    (void)c.execute("DROP TABLE stdx_range;");
}

#ifdef __cpp_impl_reflection
/**
 * @struct UserRow
 * @brief A plain reflectable row type; members map to columns by position.
 */
struct UserRow {
    i32 id;
    String name;
    i32 age;
};

/**
 * @brief Tests that query<T>() can map rows to a reflectable struct T.
 * 
 * @param ctx TestContext to record pass/fail results.
 * @param c A live Connection to a test database.
 * The test creates and drops its own table, so the schema doesn't matter.
 *
 * Exercises reflection-based row mapping: query<T>() fills each T's members
 * from the result columns in declaration order. The test verifies that two
 * rows come back with the expected data, proving that the mapping is correct
 * and that the ResultSet remains valid after query<T>() returns (i.e. it owns
 * its handle, not the internal Statement).
 */
void test_reflection_row_mapping(TestContext& ctx, Connection& c) {
    (void)c.execute("DROP TABLE IF EXISTS stdx_refl;");
    (void)c.execute(
        "CREATE TABLE stdx_refl (id INTEGER PRIMARY KEY, name VARCHAR(255), age INTEGER);"
    );
    (void)c.execute("INSERT INTO stdx_refl (id, name, age) VALUES (?, ?, ?);", 1, "Alan", 41);
    (void)c.execute("INSERT INTO stdx_refl (id, name, age) VALUES (?, ?, ?);", 2, "Grace", 85);

    Vector<UserRow> users = c.query<UserRow>(
        "SELECT id, name, age FROM stdx_refl ORDER BY id ASC;"
    );
    ctx.check("query<T> mapped 2 rows", users.size() == 2);
    if (users.size() >= 2) {
        ctx.check(
            "row 0 mapped to {1, Alan, 41}",
            users[0].id == 1 && users[0].name == "Alan" && users[0].age == 41
        );
        ctx.check(
            "row 1 mapped to {2, Grace, 85}",
            users[1].id == 2 && users[1].name == "Grace" && users[1].age == 85
        );
    }

    (void)c.execute("DROP TABLE stdx_refl;");
}
#endif


/**
 * @brief Best-effort removal: ignore any failure (e.g. file doesn't exist).
 * 
 * @param p Path to remove.
 */
void try_remove(const Path& p) noexcept {
    try {
        (void)stdx::fs::remove(p);
    } catch (...) {
        // Nothing useful to do - we're cleaning up.
    }
}
#endif

int main() {
    #ifdef STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY
    TestContext ctx;
    test_bad_connection(ctx);
    test_empty_connection_string(ctx);

    // Live mode: probe for any reachable database (SQL Server, then SQLite).
    const Path dbfile{"/tmp/stdx_sqltest.db"};
    // Remove any leftover file from a previous run so CREATE TABLE on a stale
    // schema doesn't confuse the live tests.
    try_remove(dbfile);

    Optional<Connection> conn = try_open_live(dbfile);
    if (conn.has_value()) {
        System::out.println("[live] Database reachable; running CRUD + injection tests.");
        try {
            test_crud(ctx, *conn);
            test_sql_injection_safe(ctx, *conn);
            test_connection_convenience(ctx, *conn);
            test_result_set_range(ctx, *conn);
            #ifdef __cpp_impl_reflection
            test_reflection_row_mapping(ctx, *conn);
            #endif
            conn->close();
        } catch (const SQLException& e) {
            ctx.check(
                stdx::fmt::format("live path raised: {}", e.what()),
                false
            );
        }
        try_remove(dbfile);
    } else {
        System::out.println(
            "[live] No database reachable - skipping CRUD + injection tests."
        );
    }

    System::out.println(
        "\nResults: {} passed, {} failed", ctx.passed, ctx.failed
    );
    return ctx.failed == 0 ? 0 : 1;
    #else
    System::out.println("[test] Test disabled (enable with STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY).");
    #endif
}
