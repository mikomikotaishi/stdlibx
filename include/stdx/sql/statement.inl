#pragma once

using stdx::collections::Vector;
using stdx::meta::DecayType;
using stdx::meta::IsSameValue;

#ifdef STDLIBX_EXTENSIONS_COMPILE_SQL_LIBRARY
/**
 * @namespace stdx::sql
 * @brief SQL database access over ODBC.
 */
export namespace stdx::sql {
    /**
     * @concept Bindable
     * @brief A type that can be bound as a SQL statement parameter.
     *
     * Satisfied by bool, nullptr (bound as SQL NULL), any integral or floating-
     * point type, and anything convertible to StringView. Mirrors the dispatch
     * performed by PreparedStatement::bind_one, so the variadic execute overloads
     * reject an unbindable argument at the call site rather than deep inside
     * instantiation.
     */
    template <typename T>
    concept Bindable =
        IsSameValue<DecayType<T>, bool>
        || IsSameValue<DecayType<T>, NullPointer>
        || Integral<DecayType<T>>
        || FloatingPoint<DecayType<T>>
        || ConvertibleTo<DecayType<T>, StringView>;

    /**
     * @class PreparedStatement
     * @brief Represents a precompiled SQL statement with parameter binding.
     *
     * Provides methods to bind parameters and execute parameterized SQL queries.
     */
    class PreparedStatement {
    private:
        Vector<Vector<ParamSlot>> _batch;
        Vector<ParamSlot> _params;
        String _sql;
        SQLHDBC _dbc;
        SQLHSTMT _stmt;
        bool _closed;

        /**
         * @brief Checks if the statement is closed.
         * @throws SQLException if the statement is closed.
         */
        THROWS(SQLException)
        void check_closed() const {
            if (_closed) {
                throw SQLException("PreparedStatement is closed");
            }
        }

        /**
         * @brief Validates a 1-based parameter index.
         * @param parameter_index The parameter index (1-based).
         * @throws SQLException if the index is out of range.
         */
        THROWS(SQLException)
        void check_index(i32 parameter_index) const {
            if (parameter_index < 1 || parameter_index > static_cast<i32>(_params.size())) {
                throw SQLException("Invalid parameter index");
            }
        }

        /**
         * @brief Binds a single value to a 1-based parameter index, dispatching on
         * the C++ type to the appropriate set_* overload.
         * @tparam T The deduced argument type.
         * @param parameter_index The parameter index (1-based).
         * @param value The value to bind.
         * @throws SQLException on error.
         *
         * Integral types are bound as INTEGER or BIGINT by width, floating-point as
         * DOUBLE, bool as BIT, anything convertible to StringView as VARCHAR, and
         * nullptr as SQL NULL.
         */
        template <typename T>
        THROWS(SQLException)
        void bind_one(i32 parameter_index, T&& value) {
            using V = DecayType<T>;
            if constexpr (IsSameValue<V, bool>) {
                set_boolean(parameter_index, value);
            } else if constexpr (IsSameValue<V, NullPointer>) {
                set_null(parameter_index);
            } else if constexpr (Integral<V>) {
                if constexpr (sizeof(V) > sizeof(i32)) {
                    set_long(parameter_index, static_cast<i64>(value));
                } else {
                    set_int(parameter_index, static_cast<i32>(value));
                }
            } else if constexpr (FloatingPoint<V>) {
                set_double(parameter_index, static_cast<f64>(value));
            } else if constexpr (ConvertibleTo<V, StringView>) {
                set_string(parameter_index, StringView(value));
            } else {
                static_assert(sizeof(V) == 0, "Unsupported SQL parameter type");
            }
        }

        /**
         * @brief Binds all arguments positionally (1-based), left to right.
         * @tparam Args The deduced argument types.
         * @param args The values to bind, one per parameter marker.
         * @throws SQLException if the argument count does not match the number of
         * parameter markers in the prepared statement.
         */
        template <Bindable... Args>
        THROWS(SQLException)
        void bind_all(Args&&... args) {
            if (sizeof...(Args) != _params.size()) {
                throw SQLException("Argument count does not match parameter count");
            }
            i32 index = 1;
            (bind_one(index++, Ops::forward<Args>(args)), ...);
        }

        /**
         * @internal
         * @brief Constructs a PreparedStatement from an ODBC connection handle.
         * @param dbc The ODBC connection handle.
         * @param sql The SQL statement with parameter placeholders (?).
         * @throws SQLException if statement preparation fails.
         *
         * This constructor is private and used by Connection.
         */
        THROWS(SQLException)
        PreparedStatement(SQLHDBC dbc, StringView sql):
            _sql{sql}, _dbc{dbc}, _stmt{nullptr}, _closed{false} {
            SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &_stmt);
            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_DBC, dbc, "Failed to allocate statement handle");
            }

            ret = SQLPrepare(
                this->_stmt,
                reinterpret_cast<SQLCHAR*>(const_cast<char*>(this->_sql.c_str())),
                SQL_NTS
            );

            if (!SQL_SUCCEEDED(ret)) {
                SQLException ex(SQL_HANDLE_STMT, this->_stmt, "Failed to prepare statement");
                SQLFreeHandle(SQL_HANDLE_STMT, _stmt);
                throw ex;
            }

            SQLSMALLINT param_count;
            SQLNumParams(this->_stmt, &param_count);

            _params.resize(param_count);
        }

        /**
         * @internal
         * @brief Relinquishes the statement handle without freeing it.
         * Used by Connection::query to transfer handle ownership to the
         * ResultSet it returns; this PreparedStatement then destructs
         * harmlessly.
         */
        void release_handle() noexcept {
            _stmt = nullptr;
            _closed = true;
        }

        friend class Connection;
    public:
        /**
         * @brief Move constructor.
         */
        PreparedStatement(PreparedStatement&& other) noexcept:
            _dbc{other._dbc}, _stmt{other._stmt},
            _params{Ops::move(other._params)},
            _batch{Ops::move(other._batch)},
            _sql{Ops::move(other._sql)}, _closed{other._closed} {
            other._stmt = nullptr;
            other._closed = true;
        }

        /**
         * @brief Move assignment operator.
         */
        PreparedStatement& operator=(PreparedStatement&& other) noexcept {
            if (this != &other) {
                close();
                _dbc = other._dbc;
                _stmt = other._stmt;
                _params = Ops::move(other._params);
                _batch = Ops::move(other._batch);
                _sql = Ops::move(other._sql);
                _closed = other._closed;
                other._stmt = nullptr;
                other._closed = true;
            }
            return *this;
        }

        PreparedStatement(const PreparedStatement&) = DELETE_METHOD("PreparedStatement is not copyable.");
        PreparedStatement& operator=(const PreparedStatement&) = DELETE_METHOD("PreparedStatement is not copyable.");

        /**
         * @brief Destructor.
         */
        ~PreparedStatement() {
            close();
        }

        /**
         * @brief Binds an integer parameter.
         * @param parameter_index The parameter index (1-based).
         * @param value The integer value to bind.
         * @throws SQLException on error.
         */
        THROWS(SQLException)
        void set_int(i32 parameter_index, i32 value) {
            check_closed();
            check_index(parameter_index);

            ParamSlot& slot = _params[parameter_index - 1];
            slot.value = value;
            slot.indicator = 0;

            SQLRETURN ret = SQLBindParameter(
                _stmt, parameter_index, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                0, 0, &Ops::get<i32>(slot.value),
                0, &slot.indicator
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to bind integer parameter");
            }
        }

        /**
         * @brief Binds a long integer parameter.
         * @param parameter_index The parameter index (1-based).
         * @param value The long integer value to bind.
         * @throws SQLException on error.
         */
        THROWS(SQLException)
        void set_long(i32 parameter_index, i64 value) {
            check_closed();
            check_index(parameter_index);

            ParamSlot& slot = _params[parameter_index - 1];
            slot.value = value;
            slot.indicator = 0;

            SQLRETURN ret = SQLBindParameter(
                _stmt, parameter_index, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT,
                0, 0, &Ops::get<i64>(slot.value),
                0, &slot.indicator
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to bind long parameter");
            }
        }

        /**
         * @brief Binds a double parameter.
         * @param parameter_index The parameter index (1-based).
         * @param value The double value to bind.
         * @throws SQLException on error.
         */
        THROWS(SQLException)
        void set_double(i32 parameter_index, f64 value) {
            check_closed();
            check_index(parameter_index);

            ParamSlot& slot = _params[parameter_index - 1];
            slot.value = value;
            slot.indicator = 0;

            SQLRETURN ret = SQLBindParameter(
                _stmt, parameter_index, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE,
                0, 0, &Ops::get<f64>(slot.value),
                0, &slot.indicator
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to bind double parameter");
            }
        }

        /**
         * @brief Binds a string parameter.
         * @param parameter_index The parameter index (1-based).
         * @param value The string value to bind.
         * @throws SQLException on error.
         */
        THROWS(SQLException)
        void set_string(i32 parameter_index, StringView value) {
            check_closed();
            check_index(parameter_index);

            ParamSlot& slot = _params[parameter_index - 1];
            slot.value = String(value);
            const String& str = Ops::get<String>(slot.value);
            slot.indicator = static_cast<SQLLEN>(str.size());

            SQLRETURN ret = SQLBindParameter(
                _stmt, parameter_index, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                str.size(), 0, const_cast<char*>(str.c_str()),
                str.size(), &slot.indicator
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to bind string parameter");
            }
        }

        /**
         * @brief Binds a boolean parameter.
         * @param parameter_index The parameter index (1-based).
         * @param value The boolean value to bind.
         * @throws SQLException on error.
         */
        THROWS(SQLException)
        void set_boolean(i32 parameter_index, bool value) {
            check_closed();
            check_index(parameter_index);

            ParamSlot& slot = _params[parameter_index - 1];
            slot.value = static_cast<u8>(value ? 1 : 0);
            slot.indicator = 0;

            SQLRETURN ret = SQLBindParameter(
                _stmt, parameter_index, SQL_PARAM_INPUT, SQL_C_BIT, SQL_BIT, 0, 0,
                &Ops::get<u8>(slot.value), 0, &slot.indicator
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to bind boolean parameter");
            }
        }

        /**
         * @brief Sets a parameter to NULL.
         * @param parameter_index The parameter index (1-based).
         * @throws SQLException on error.
         */
        THROWS(SQLException)
        void set_null(i32 parameter_index) {
            check_closed();
            check_index(parameter_index);

            ParamSlot& slot = _params[parameter_index - 1];
            slot.value = Monostate{};
            slot.indicator = SQL_NULL_DATA;

            SQLRETURN ret = SQLBindParameter(
                _stmt, parameter_index, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                0, 0, nullptr, 0, &slot.indicator
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to bind NULL parameter");
            }
        }

        /**
         * @brief Executes the prepared query and returns a result set.
         * @return A ResultSet containing the query results.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        ResultSet execute_query() {
            check_closed();

            SQLRETURN ret = SQLExecute(_stmt);

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to execute prepared query");
            }

            return ResultSet(_stmt);
        }

        /**
         * @brief Executes the prepared update/insert/delete statement.
         * @return The number of rows affected.
         * @throws SQLException on error.
         */
        THROWS(SQLException)
        i32 execute_update() {
            check_closed();

            SQLRETURN ret = SQLExecute(_stmt);

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to execute prepared update");
            }

            SQLLEN row_count;
            ret = SQLRowCount(_stmt, &row_count);

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to get row count");
            }

            return static_cast<i32>(row_count);
        }

        /**
         * @brief Binds the given arguments to the parameter markers in order, then
         * executes the prepared query.
         * @tparam Args The deduced argument types (at least one).
         * @param args One value per parameter marker, in order.
         * @return A ResultSet containing the query results.
         * @throws SQLException on error.
         *
         * Each argument is bound positionally to the corresponding `?` marker,
         * dispatching on its C++ type. Because values are sent as bound parameters,
         * they are never interpreted as SQL - this is the injection-safe path.
         */
        template <Bindable... Args>
            requires (sizeof...(Args) >= 1)
        [[nodiscard]]
        THROWS(SQLException)
        ResultSet execute_query(Args&&... args) {
            bind_all(Ops::forward<Args>(args)...);
            return execute_query();
        }

        /**
         * @brief Binds the given arguments to the parameter markers in order, then
         * executes the prepared update/insert/delete statement.
         * @tparam Args The deduced argument types (at least one).
         * @param args One value per parameter marker, in order.
         * @return The number of rows affected.
         * @throws SQLException on error.
         */
        template <Bindable... Args>
            requires (sizeof...(Args) >= 1)
        THROWS(SQLException)
        i32 execute_update(Args&&... args) {
            bind_all(Ops::forward<Args>(args)...);
            return execute_update();
        }

        /**
         * @brief Adds the current set of parameters to the batch.
         * @throws SQLException if the statement is closed.
         *
         * Saves a snapshot of the current parameter values. After calling this,
         * new parameters can be set for the next batch entry. Call execute_batch()
         * to execute all accumulated entries.
         */
        THROWS(SQLException)
        void add_batch() {
            check_closed();
            _batch.push_back(_params);
        }

        /**
         * @brief Executes all batched parameter sets.
         * @return A vector of row counts, one per batch entry.
         * @throws SQLException on error (execution stops at the first failure).
         *
         * Iterates over each snapshot added via add_batch(), rebinds parameters,
         * and executes the statement. Returns the row counts for each execution.
         * The batch is cleared after execution.
         */
        THROWS(SQLException)
        Vector<i32> execute_batch() {
            check_closed();

            Vector<i32> results;
            results.reserve(_batch.size());

            for (Vector<ParamSlot>& entry: _batch) {
                SQLFreeStmt(_stmt, SQL_RESET_PARAMS);

                for (i32 i = 0; i < static_cast<i32>(entry.size()); ++i) {
                    ParamSlot& slot = entry[i];
                    i32 param_index = i + 1;

                    Ops::visit([&](auto& val) -> void {
                        if constexpr (IsSameValue<DecayType<decltype(val)>, Monostate>) {
                            slot.indicator = SQL_NULL_DATA;
                            SQLBindParameter(
                                _stmt, param_index, SQL_PARAM_INPUT,
                                SQL_C_CHAR, SQL_VARCHAR, 0, 0,
                                nullptr, 0, &slot.indicator
                            );
                        } else if constexpr (IsSameValue<DecayType<decltype(val)>, i32>) {
                            SQLBindParameter(
                                _stmt, param_index, SQL_PARAM_INPUT,
                                SQL_C_SLONG, SQL_INTEGER, 0, 0,
                                &val, 0, &slot.indicator
                            );
                        } else if constexpr (IsSameValue<DecayType<decltype(val)>, i64>) {
                            SQLBindParameter(
                                _stmt, param_index, SQL_PARAM_INPUT,
                                SQL_C_SBIGINT, SQL_BIGINT, 0, 0,
                                &val, 0, &slot.indicator
                            );
                        } else if constexpr (IsSameValue<DecayType<decltype(val)>, f64>) {
                            SQLBindParameter(
                                _stmt, param_index, SQL_PARAM_INPUT,
                                SQL_C_DOUBLE, SQL_DOUBLE, 0, 0,
                                &val, 0, &slot.indicator
                            );
                        } else if constexpr (IsSameValue<DecayType<decltype(val)>, String>) {
                            slot.indicator = static_cast<SQLLEN>(val.size());
                            SQLBindParameter(
                                _stmt, param_index, SQL_PARAM_INPUT,
                                SQL_C_CHAR, SQL_VARCHAR,
                                val.size(), 0, const_cast<char*>(val.c_str()),
                                val.size(), &slot.indicator
                            );
                        } else if constexpr (IsSameValue<DecayType<decltype(val)>, u8>) {
                            SQLBindParameter(
                                _stmt, param_index, SQL_PARAM_INPUT,
                                SQL_C_BIT, SQL_BIT, 0, 0,
                                &val, 0, &slot.indicator
                            );
                        }
                    }, slot.value);
                }

                SQLRETURN ret = SQLExecute(_stmt);
                if (!SQL_SUCCEEDED(ret)) {
                    clear_batch();
                    throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to execute batch entry");
                }

                SQLLEN row_count;
                SQLRowCount(_stmt, &row_count);
                results.push_back(static_cast<i32>(row_count));
            }

            clear_batch();
            return results;
        }

        /**
         * @brief Clears all accumulated batch entries.
         */
        void clear_batch() noexcept {
            _batch.clear();
        }

        /**
         * @brief Clears all parameter bindings.
         * @throws SQLException if the statement is closed.
         */
        THROWS(SQLException)
        void clear_parameters() {
            check_closed();
            SQLFreeStmt(_stmt, SQL_RESET_PARAMS);
        }

        /**
         * @brief Closes the statement.
         */
        void close() noexcept {
            if (!_closed && _stmt != nullptr) {
                SQLFreeHandle(SQL_HANDLE_STMT, _stmt);
                _stmt = nullptr;
                _closed = true;
            }
        }

        /**
         * @brief Checks if the statement is closed.
         * @return true if closed, false otherwise.
         */
        [[nodiscard]]
        bool is_closed() const noexcept {
            return _closed;
        }
    };

    /**
     * @class CallableStatement
     * @brief Represents a callable statement for executing stored procedures.
     *
     * Extends PreparedStatement semantics with support for registering and
     * retrieving OUT parameters. Uses the ODBC `{call procedure_name(?, ?)}` syntax.
     */
    class CallableStatement {
    private:
        SQLHDBC _dbc;
        SQLHSTMT _stmt;
        Vector<ParamSlot> _params;
        Vector<SQLSMALLINT> _param_directions;
        String _sql;
        bool _closed;

        THROWS(SQLException)
        void check_closed() const {
            if (_closed) {
                throw SQLException("CallableStatement is closed");
            }
        }

        THROWS(SQLException)
        void check_index(i32 parameter_index) const {
            if (parameter_index < 1 || parameter_index > static_cast<i32>(_params.size())) {
                throw SQLException("Invalid parameter index");
            }
        }

        /**
         * @internal
         * @brief Constructs a CallableStatement.
         * @param dbc The ODBC connection handle.
         * @param procedure_call The ODBC call syntax, e.g. "{call my_proc(?, ?)}".
         * @throws SQLException if statement preparation fails.
         *
         * This constructor is private and used by Connection.
         */
        THROWS(SQLException)
        CallableStatement(SQLHDBC dbc, StringView procedure_call):
            _dbc{dbc}, _stmt{nullptr}, _params{}, _param_directions{},
            _sql{procedure_call}, _closed{false} {
            SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &_stmt);
            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_DBC, dbc, "Failed to allocate statement handle");
            }

            ret = SQLPrepare(
                this->_stmt,
                reinterpret_cast<SQLCHAR*>(const_cast<char*>(this->_sql.c_str())),
                SQL_NTS
            );

            if (!SQL_SUCCEEDED(ret)) {
                SQLException ex(SQL_HANDLE_STMT, this->_stmt, "Failed to prepare callable statement");
                SQLFreeHandle(SQL_HANDLE_STMT, _stmt);
                throw ex;
            }

            SQLSMALLINT param_count;
            SQLNumParams(this->_stmt, &param_count);

            _params.resize(param_count);
            _param_directions.resize(param_count, SQL_PARAM_INPUT);
        }

        friend class Connection;

    public:
        /**
         * @brief Move constructor.
         */
        CallableStatement(CallableStatement&& other) noexcept:
            _dbc{other._dbc}, _stmt{other._stmt},
            _params{Ops::move(other._params)},
            _param_directions{Ops::move(other._param_directions)},
            _sql{Ops::move(other._sql)}, _closed{other._closed} {
            other._stmt = nullptr;
            other._closed = true;
        }

        /**
         * @brief Move assignment operator.
         */
        CallableStatement& operator=(CallableStatement&& other) noexcept {
            if (this != &other) {
                close();
                _dbc = other._dbc;
                _stmt = other._stmt;
                _params = Ops::move(other._params);
                _param_directions = Ops::move(other._param_directions);
                _sql = Ops::move(other._sql);
                _closed = other._closed;
                other._stmt = nullptr;
                other._closed = true;
            }
            return *this;
        }

        CallableStatement(const CallableStatement&) = DELETE_METHOD("CallableStatement is not copyable.");
        CallableStatement& operator=(const CallableStatement&) = DELETE_METHOD("CallableStatement is not copyable.");

        ~CallableStatement() {
            close();
        }

        /**
         * @brief Registers a parameter as an OUT parameter.
         * @param parameter_index The parameter index (1-based).
         * @param type The SQL type of the OUT parameter.
         * @throws SQLException on error.
         *
         * Must be called before execute() for each OUT or INOUT parameter.
         * The SQL type tells the driver what to expect from the procedure.
         */
        THROWS(SQLException)
        void register_out_parameter(i32 parameter_index, SQLType type) {
            check_closed();
            check_index(parameter_index);

            _param_directions[parameter_index - 1] = SQL_PARAM_OUTPUT;
            ParamSlot& slot = _params[parameter_index - 1];
            slot.indicator = 0;

            SQLSMALLINT c_type = SQL_C_CHAR;
            SQLULEN col_size = 256;

            switch (type) {
                case SQLType::INTEGER:
                    slot.value = i32{0};
                    c_type = SQL_C_SLONG;
                    col_size = 0;
                    break;
                case SQLType::BIGINT:
                    slot.value = i64{0};
                    c_type = SQL_C_SBIGINT;
                    col_size = 0;
                    break;
                case SQLType::DOUBLE:
                case SQLType::FLOAT:
                    slot.value = f64{0.0};
                    c_type = SQL_C_DOUBLE;
                    col_size = 0;
                    break;
                case SQLType::BIT:
                    slot.value = u8{0};
                    c_type = SQL_C_BIT;
                    col_size = 0;
                    break;
                default:
                    slot.value = String(256, '\0');
                    c_type = SQL_C_CHAR;
                    col_size = 256;
                    slot.indicator = SQL_NTS;
                    break;
            }

            void* value_ptr = Ops::visit([](auto& val) -> void* {
                if constexpr (IsSameValue<DecayType<decltype(val)>, Monostate>) {
                    return nullptr;
                } else if constexpr (IsSameValue<DecayType<decltype(val)>, String>) {
                    return val.data();
                } else {
                    return &val;
                }
            }, slot.value);

            SQLLEN buffer_len = 0;
            if (holds_alternative<String>(slot.value)) {
                buffer_len = static_cast<SQLLEN>(Ops::get<String>(slot.value).size());
            }

            SQLRETURN ret = SQLBindParameter(
                _stmt, parameter_index, SQL_PARAM_OUTPUT,
                c_type, static_cast<SQLSMALLINT>(static_cast<i32>(type)),
                col_size, 0, value_ptr, buffer_len, &slot.indicator
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to register OUT parameter");
            }
        }

        /**
         * @brief Sets an integer IN parameter.
         * @param parameter_index The parameter index (1-based).
         * @param value The integer value to bind.
         * @throws SQLException on error.
         */
        THROWS(SQLException)
        void set_int(i32 parameter_index, i32 value) {
            check_closed();
            check_index(parameter_index);

            ParamSlot& slot = _params[parameter_index - 1];
            slot.value = value;
            slot.indicator = 0;
            _param_directions[parameter_index - 1] = SQL_PARAM_INPUT;

            SQLRETURN ret = SQLBindParameter(
                _stmt, parameter_index, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                0, 0, &Ops::get<i32>(slot.value), 0, &slot.indicator
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to bind integer parameter");
            }
        }

        /**
         * @brief Sets a string IN parameter.
         * @param parameter_index The parameter index (1-based).
         * @param value The string value to bind.
         * @throws SQLException on error.
         */
        THROWS(SQLException)
        void set_string(i32 parameter_index, StringView value) {
            check_closed();
            check_index(parameter_index);

            ParamSlot& slot = _params[parameter_index - 1];
            slot.value = String(value);
            const String& str = Ops::get<String>(slot.value);
            slot.indicator = static_cast<SQLLEN>(str.size());
            _param_directions[parameter_index - 1] = SQL_PARAM_INPUT;

            SQLRETURN ret = SQLBindParameter(
                _stmt, parameter_index, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                str.size(), 0, const_cast<char*>(str.c_str()),
                str.size(), &slot.indicator
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to bind string parameter");
            }
        }

        /**
         * @brief Executes the callable statement.
         * @return true if the result is a ResultSet, false otherwise.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        bool execute() {
            check_closed();

            SQLRETURN ret = SQLExecute(_stmt);
            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to execute callable statement");
            }

            SQLSMALLINT col_count;
            ret = SQLNumResultCols(_stmt, &col_count);
            return SQL_SUCCEEDED(ret) && col_count > 0;
        }

        /**
         * @brief Gets the result set from the last execute() call.
         * @return A ResultSet containing the query results.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        ResultSet result_set() {
            check_closed();
            return ResultSet(_stmt);
        }

        /**
         * @brief Gets an integer OUT parameter value.
         * @param parameter_index The parameter index (1-based).
         * @return The integer value.
         * @throws SQLException if the parameter is not an integer OUT parameter.
         */
        [[nodiscard]]
        THROWS(SQLException)
        i32 get_int(i32 parameter_index) const {
            check_closed();
            check_index(parameter_index);

            const ParamSlot& slot = _params[parameter_index - 1];
            if (slot.indicator == SQL_NULL_DATA) {
                throw SQLException("OUT parameter is NULL");
            }
            return Ops::get<i32>(slot.value);
        }

        /**
         * @brief Gets a long OUT parameter value.
         * @param parameter_index The parameter index (1-based).
         * @return The long value.
         * @throws SQLException if the parameter is not a long OUT parameter.
         */
        [[nodiscard]]
        THROWS(SQLException)
        i64 get_long(i32 parameter_index) const {
            check_closed();
            check_index(parameter_index);

            const ParamSlot& slot = _params[parameter_index - 1];
            if (slot.indicator == SQL_NULL_DATA) {
                throw SQLException("OUT parameter is NULL");
            }
            return Ops::get<i64>(slot.value);
        }

        /**
         * @brief Gets a double OUT parameter value.
         * @param parameter_index The parameter index (1-based).
         * @return The double value.
         * @throws SQLException if the parameter is not a double OUT parameter.
         */
        [[nodiscard]]
        THROWS(SQLException)
        f64 get_double(i32 parameter_index) const {
            check_closed();
            check_index(parameter_index);

            const ParamSlot& slot = _params[parameter_index - 1];
            if (slot.indicator == SQL_NULL_DATA) {
                throw SQLException("OUT parameter is NULL");
            }
            return Ops::get<f64>(slot.value);
        }

        /**
         * @brief Gets a string OUT parameter value.
         * @param parameter_index The parameter index (1-based).
         * @return The string value.
         * @throws SQLException if the parameter is not a string OUT parameter.
         */
        [[nodiscard]]
        THROWS(SQLException)
        String get_string(i32 parameter_index) const {
            check_closed();
            check_index(parameter_index);

            const ParamSlot& slot = _params[parameter_index - 1];
            if (slot.indicator == SQL_NULL_DATA) {
                throw SQLException("OUT parameter is NULL");
            }
            return Ops::get<String>(slot.value);
        }

        /**
         * @brief Checks if the last OUT parameter read was NULL.
         * @param parameter_index The parameter index (1-based).
         * @return true if the parameter value is NULL.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        bool was_null(i32 parameter_index) const {
            check_closed();
            check_index(parameter_index);
            return _params[parameter_index - 1].indicator == SQL_NULL_DATA;
        }

        /**
         * @brief Closes the statement.
         */
        void close() noexcept {
            if (!_closed && _stmt != nullptr) {
                SQLFreeHandle(SQL_HANDLE_STMT, _stmt);
                _stmt = nullptr;
                _closed = true;
            }
        }

        /**
         * @brief Checks if the statement is closed.
         * @return true if closed, false otherwise.
         */
        [[nodiscard]]
        bool is_closed() const noexcept {
            return _closed;
        }
    };

    /**
     * @class Statement
     * @brief Represents a SQL statement to be executed.
     *
     * Provides methods to execute SQL queries and updates.
     */
    class Statement {
    private:
        SQLHDBC _dbc;
        SQLHSTMT _stmt;
        ResultSetType _rs_type;
        bool closed;

        /**
         * @brief Checks if the statement is closed.
         * @throws SQLException if the statement is closed.
         */
        THROWS(SQLException)
        void check_closed() const {
            if (closed) {
                throw SQLException("Statement is closed");
            }
        }

        /**
         * @internal
         * @brief Constructs a Statement from an ODBC connection handle.
         * @param dbc The ODBC connection handle.
         * @throws SQLException if statement allocation fails.
         *
         * This constructor is private and used by Connection.
         */
        THROWS(SQLException)
        explicit Statement(SQLHDBC dbc):
            _dbc{dbc}, _stmt{nullptr}, _rs_type{ResultSetType::FORWARD_ONLY}, closed{false} {
            SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, this->_dbc, &this->_stmt);
            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_DBC, this->_dbc, "Failed to allocate statement handle");
            }
        }

        /**
         * @internal
         * @brief Relinquishes the statement handle without freeing it.
         *
         * Used by Connection::query to transfer handle ownership to the
         * ResultSet it returns; this Statement then destructs harmlessly.
         */
        void release_handle() noexcept {
            _stmt = nullptr;
            closed = true;
        }

        friend class Connection;

    public:
        /**
         * @brief Move constructor.
         */
        Statement(Statement&& other) noexcept:
            _dbc{other._dbc}, _stmt{other._stmt}, _rs_type{other._rs_type}, closed{other.closed} {
            other._stmt = nullptr;
            other.closed = true;
        }

        /**
         * @brief Move assignment operator.
         */
        Statement& operator=(Statement&& other) noexcept {
            if (this != &other) {
                close();
                _dbc = other._dbc;
                _stmt = other._stmt;
                _rs_type = other._rs_type;
                closed = other.closed;
                other._stmt = nullptr;
                other.closed = true;
            }
            return *this;
        }

        Statement(const Statement&) = DELETE_METHOD("Statement is not copyable.");
        Statement& operator=(const Statement&) = DELETE_METHOD("Statement is not copyable.");

        /**
         * @brief Destructor.
         */
        ~Statement() {
            close();
        }

        /**
         * @brief Executes a SQL query and returns a result set.
         * @param sql The SQL query to execute.
         * @return A ResultSet containing the query results.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        ResultSet execute_query(StringView sql) {
            check_closed();

            String sql_str(sql);
            SQLRETURN ret = SQLExecDirect(
                _stmt,
                reinterpret_cast<SQLCHAR*>(sql_str.data()),
                static_cast<SQLINTEGER>(sql_str.size())
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to execute query");
            }

            return ResultSet(_stmt, _rs_type != ResultSetType::FORWARD_ONLY);
        }

        /**
         * @brief Executes a SQL update/insert/delete statement.
         * @param sql The SQL statement to execute.
         * @return The number of rows affected.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        i32 execute_update(StringView sql) {
            check_closed();

            String sql_str(sql);
            SQLRETURN ret = SQLExecDirect(
                _stmt,
                reinterpret_cast<SQLCHAR*>(sql_str.data()),
                static_cast<SQLINTEGER>(sql_str.size())
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to execute update");
            }

            SQLLEN row_count;
            ret = SQLRowCount(_stmt, &row_count);

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to get row count");
            }

            return static_cast<i32>(row_count);
        }

        /**
         * @brief Executes any SQL statement.
         * @param sql The SQL statement to execute.
         * @return true if the result is a ResultSet, false otherwise.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        bool execute(StringView sql) {
            check_closed();

            String sql_str(sql);
            SQLRETURN ret = SQLExecDirect(
                _stmt,
                reinterpret_cast<SQLCHAR*>(sql_str.data()),
                static_cast<SQLINTEGER>(sql_str.size())
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to execute statement");
            }

            SQLSMALLINT column_count;
            ret = SQLNumResultCols(_stmt, &column_count);

            return SQL_SUCCEEDED(ret) && column_count > 0;
        }

        /**
         * @brief Gets the result set from the last execute() call.
         * @return A ResultSet containing the query results.
         * @throws SQLException if no result set is available.
         */
        [[nodiscard]]
        THROWS(SQLException)
        ResultSet result_set() const {
            check_closed();
            return ResultSet(_stmt, _rs_type != ResultSetType::FORWARD_ONLY);
        }

        /**
         * @brief Gets the update count from the last execute_update() call.
         *
         * @return The number of rows affected.
         * @throws SQLException on error.
         */
        [[nodiscard]]
        THROWS(SQLException)
        i32 update_count() {
            check_closed();

            SQLLEN row_count;
            SQLRETURN ret = SQLRowCount(_stmt, &row_count);

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to get row count");
            }

            return static_cast<i32>(row_count);
        }

        /**
         * @brief Sets the result set type (scrollability) for subsequent queries.
         * @param type The desired result set type.
         * @throws SQLException on error.
         *
         * Must be called before execute_query(). Sets the ODBC cursor type
         * on the underlying statement handle.
         */
        THROWS(SQLException)
        void set_result_set_type(ResultSetType type) {
            check_closed();

            SQLRETURN ret = SQLSetStmtAttr(
                _stmt, SQL_ATTR_CURSOR_TYPE,
                reinterpret_cast<SQLPOINTER>(static_cast<SQLLEN>(static_cast<i32>(type))), 0
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to set cursor type");
            }

            _rs_type = type;
        }

        /**
         * @brief Sets the query timeout in seconds.
         * @param seconds The timeout in seconds.
         * @throws SQLException on error.
         *
         * If the query does not complete within the specified time, it is cancelled.
         * A value of 0 means no timeout (the default).
         */
        THROWS(SQLException)
        void set_query_timeout(i32 seconds) {
            check_closed();

            SQLRETURN ret = SQLSetStmtAttr(
                _stmt, SQL_ATTR_QUERY_TIMEOUT,
                reinterpret_cast<SQLPOINTER>(static_cast<SQLLEN>(seconds)), 0
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to set query timeout");
            }
        }

        /**
         * @brief Sets the maximum number of rows to return from a query.
         * @param max The maximum number of rows.
         * @throws SQLException on error.
         *
         * A value of 0 means no limit (the default).
         */
        THROWS(SQLException)
        void set_max_rows(i32 max) {
            check_closed();

            SQLRETURN ret = SQLSetStmtAttr(
                _stmt, SQL_ATTR_MAX_ROWS,
                reinterpret_cast<SQLPOINTER>(static_cast<SQLLEN>(max)), 0
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to set max rows");
            }
        }

        /**
         * @brief Sets the number of rows to fetch at a time (fetch size hint).
         * @param size The fetch size.
         * @throws SQLException on error.
         *
         * This is a hint to the driver for performance optimization.
         * A value of 0 means use the driver default.
         */
        THROWS(SQLException)
        void set_fetch_size(i32 size) {
            check_closed();

            SQLRETURN ret = SQLSetStmtAttr(
                _stmt, SQL_ATTR_ROW_ARRAY_SIZE,
                reinterpret_cast<SQLPOINTER>(static_cast<SQLLEN>(size)), 0
            );

            if (!SQL_SUCCEEDED(ret)) {
                throw_sql_exception(SQL_HANDLE_STMT, _stmt, "Failed to set fetch size");
            }
        }

        /**
         * @brief Closes the statement.
         */
        void close() noexcept {
            if (!closed && _stmt != nullptr) {
                SQLFreeHandle(SQL_HANDLE_STMT, _stmt);
                _stmt = nullptr;
                closed = true;
            }
        }

        /**
         * @brief Checks if the statement is closed.
         * @return true if closed, false otherwise.
         */
        [[nodiscard]]
        bool is_closed() const noexcept {
            return closed;
        }
    };
}
#endif
