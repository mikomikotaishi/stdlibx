#pragma once

/**
 * @namespace stdx::data::csv
 * @brief CSV parsing, construction, and serialization (RFC 4180).
 */
export namespace stdx::data::csv {
    /**
     * @struct CsvDialect
     * @brief The lexical conventions of a particular CSV flavor.
     *
     * RFC 4180 fixes the comma, the double-quote, and CRLF, but real files vary:
     * TSV uses a tab, some European exports use ';', and many tools quote with a
     * different character. A dialect captures those knobs so one parser serves
     * them all. @ref has_header records whether the first record is a header row
     * rather than data; it does not change lexing, only how a @ref CsvDocument
     * interprets the rows it gets back.
     */
    struct CsvDialect {
        char delimiter = ','; ///< The field separator.
        char quote = '"'; ///< The quoting character; doubled inside a quoted field to mean itself.
        bool has_header = true; ///< Whether the first record names the columns.

        /**
         * @brief The RFC 4180 defaults: comma-separated, double-quoted, with a header.
         * @return The default dialect.
         */
        [[nodiscard]]
        static constexpr CsvDialect standard() noexcept {
            return CsvDialect{};
        }

        /**
         * @brief Tab-separated values, with a header.
         * @return A TSV dialect.
         */
        [[nodiscard]]
        static constexpr CsvDialect tsv() noexcept {
            return CsvDialect{.delimiter = '\t', .quote = '"', .has_header = true};
        }
    };

    /**
     * @class CsvRow
     * @brief One record: an ordered list of text fields.
     *
     * CSV has no types - every field is text - so a row is just its strings, in
     * column order. Typed reads happen a layer up, at the serde boundary, the
     * same way a command line is a list of strings until a parser gives them
     * meaning. A row carries an optional back-reference to its document's header
     * so a field can be fetched by column name as well as by index.
     */
    class CsvRow {
    private:
        Vector<String> _fields; ///< The fields, in column order.
        const Vector<String>* _header = nullptr; ///< The owning document's header, or nullptr if headerless.
    public:
        CsvRow() = default;

        explicit CsvRow(Vector<String> fields, const Vector<String>* header = nullptr) noexcept:
            _fields{Ops::move(fields)}, _header{header} {}

        /**
         * @brief The number of fields.
         * @return The field count.
         */
        [[nodiscard]]
        usize size() const noexcept {
            return _fields.size();
        }

        /**
         * @brief Whether the row has no fields.
         * @return true if empty, false otherwise.
         */
        [[nodiscard]]
        bool empty() const noexcept {
            return _fields.empty();
        }

        /**
         * @brief The fields, in column order.
         * @return A reference to the field vector.
         */
        [[nodiscard]]
        const Vector<String>& fields() const noexcept {
            return _fields;
        }

        [[nodiscard]]
        Vector<String>::const_iterator begin() const noexcept {
            return _fields.begin();
        }

        [[nodiscard]]
        Vector<String>::const_iterator end() const noexcept {
            return _fields.end();
        }

        /**
         * @brief Accesses a field by column index, without bounds checking.
         * @param index The 0-based column index.
         * @return A reference to the field.
         */
        [[nodiscard]]
        const String& operator[](usize index) const noexcept {
            return _fields[index];
        }

        /**
         * @brief Accesses a field by column index, checked.
         * @param index The 0-based column index.
         * @return A reference to the field.
         * @throws CsvTypeException if @p index is out of range.
         */
        [[nodiscard]]
        THROWS(CsvTypeException)
        const String& at(usize index) const {
            if (index >= _fields.size()) {
                throw CsvTypeException(Ops::fmt("field index {} out of range (row has {} fields)", index, _fields.size()));
            }
            return _fields[index];
        }

        /**
         * @brief Appends a field.
         * @param field The field text.
         */
        void push(String field) {
            _fields.push_back(Ops::move(field));
        }

        /**
         * @brief The index of a column by its header name.
         * @param name The column name.
         * @return The 0-based index, or an empty Optional if there is no header
         * or no such column.
         */
        [[nodiscard]]
        Optional<usize> column_index(StringView name) const noexcept {
            if (_header == nullptr) {
                return nullopt;
            }
            for (usize i = 0; i < _header->size(); ++i) {
                if ((*_header)[i] == name) {
                    return i;
                }
            }
            return nullopt;
        }

        /**
         * @brief A field by column name, if present.
         * @param name The column name.
         * @return A pointer to the field, or nullptr if the column is absent or
         * the row has no field at that position.
         */
        [[nodiscard]]
        const String* find(StringView name) const noexcept {
            const Optional<usize> index = column_index(name);
            if (!index.has_value() || *index >= _fields.size()) {
                return nullptr;
            }
            return &_fields[*index];
        }

        /**
         * @brief A field by column name, checked.
         * @param name The column name.
         * @return A reference to the field.
         * @throws CsvTypeException if the column is absent.
         */
        [[nodiscard]]
        THROWS(CsvTypeException)
        const String& at(StringView name) const {
            const String* field = find(name);
            if (field == nullptr) {
                throw CsvTypeException(Ops::fmt("no such column: {}", name));
            }
            return *field;
        }

        [[nodiscard]]
        bool operator==(const CsvRow& other) const noexcept {
            return _fields == other._fields;
        }

        friend class CsvDocument;
    };

    /**
     * @class CsvDocument
     * @brief A parsed CSV document: an optional header plus a sequence of rows.
     *
     * When the dialect has a header, the first record is split off into
     * @ref header and the remaining records become data @ref rows, each able to
     * resolve a field by column name. Headerless documents keep every record as
     * a data row and have no column-name lookup. The document owns the header
     * vector; rows hold a pointer into it, so a document must outlive the rows
     * handed out from it.
     */
    class CsvDocument {
    private:
        Vector<String> _header; ///< The column names, or empty if headerless.
        bool _has_header = false; ///< Whether @ref _header is meaningful.
        Vector<CsvRow> _rows; ///< The data rows (excludes the header record).

        /**
         * @internal
         * @brief Rebinds every row's header back-pointer to this document's header.
         *
         * Row header pointers go stale whenever the document moves or the header
         * vector reallocates, so construction and any header mutation re-point
         * them. Done here in one place rather than scattered across the mutators.
         */
        void rebind() noexcept {
            const Vector<String>* header = _has_header ? &_header : nullptr;
            for (CsvRow& row: _rows) {
                row._header = header;
            }
        }
    public:
        CsvDocument() = default;

        CsvDocument(Vector<String> header, bool has_header, Vector<CsvRow> rows) noexcept:
            _header{Ops::move(header)}, _has_header{has_header}, _rows{Ops::move(rows)} {
            rebind();
        }

        CsvDocument(const CsvDocument& other):
            _header{other._header}, _has_header{other._has_header}, _rows{other._rows} {
            rebind();
        }

        CsvDocument(CsvDocument&& other) noexcept:
            _header{Ops::move(other._header)}, _has_header{other._has_header}, _rows{Ops::move(other._rows)} {
            rebind();
        }

        CsvDocument& operator=(CsvDocument other) noexcept {
            swap(*this, other);
            rebind();
            return *this;
        }

        friend void swap(CsvDocument& a, CsvDocument& b) noexcept {
            Ops::swap(a._header, b._header);
            Ops::swap(a._has_header, b._has_header);
            Ops::swap(a._rows, b._rows);
        }

        /**
         * @brief Whether the document carries a header row.
         * @return true if a header is present, false otherwise.
         */
        [[nodiscard]]
        bool has_header() const noexcept {
            return _has_header;
        }

        /**
         * @brief The column names.
         * @return A reference to the header vector (empty if headerless).
         */
        [[nodiscard]]
        const Vector<String>& header() const noexcept {
            return _header;
        }

        /**
         * @brief The number of data rows (excluding any header).
         * @return The row count.
         */
        [[nodiscard]]
        usize size() const noexcept {
            return _rows.size();
        }

        /**
         * @brief Whether there are no data rows.
         * @return true if empty, false otherwise.
         */
        [[nodiscard]]
        bool empty() const noexcept {
            return _rows.empty();
        }

        /**
         * @brief The data rows.
         * @return A reference to the row vector.
         */
        [[nodiscard]]
        const Vector<CsvRow>& rows() const noexcept {
            return _rows;
        }

        [[nodiscard]]
        Vector<CsvRow>::const_iterator begin() const noexcept {
            return _rows.begin();
        }

        [[nodiscard]]
        Vector<CsvRow>::const_iterator end() const noexcept {
            return _rows.end();
        }

        /**
         * @brief A data row by index, checked.
         * @param index The 0-based row index.
         * @return A reference to the row.
         * @throws CsvTypeException if @p index is out of range.
         */
        [[nodiscard]]
        THROWS(CsvTypeException)
        const CsvRow& row(usize index) const {
            if (index >= _rows.size()) {
                throw CsvTypeException(Ops::fmt("row index {} out of range (document has {} rows)", index, _rows.size()));
            }
            return _rows[index];
        }

        /**
         * @brief Appends a data row, binding it to this document's header.
         * @param r The row to append.
         */
        void push(CsvRow r) {
            _rows.push_back(Ops::move(r));
            rebind();
        }

        /**
         * @brief Parses CSV text into a document.
         * @param text The document.
         * @param dialect The lexical conventions to parse under.
         * @return The parsed document.
         * @throws CsvParseException if @p text is not well-formed CSV.
         */
        [[nodiscard]]
        THROWS(CsvParseException)
        static CsvDocument parse(StringView text, const CsvDialect& dialect = CsvDialect::standard());

        /**
         * @brief Parses CSV text, reporting failure as an empty Optional.
         * @param text The document.
         * @param dialect The lexical conventions to parse under.
         * @return The parsed document, or an empty Optional on malformed input.
         */
        [[nodiscard]]
        static Optional<CsvDocument> try_parse(StringView text, const CsvDialect& dialect = CsvDialect::standard()) noexcept;

        /**
         * @brief Serializes the document back to CSV text.
         * @param dialect The lexical conventions to write under.
         * @return The serialized document, header first if present.
         */
        [[nodiscard]]
        String to_string(const CsvDialect& dialect = CsvDialect::standard()) const;

        #ifdef __cpp_lib_reflection
        /**
         * @brief Builds a CSV document from a sequence of reflectable rows.
         * @tparam T The row struct type.
         * @param rows The rows to serialize.
         * @return A document whose header is T's field names and whose rows are
         * the serialized elements.
         *
         * Reads public data members only; a struct with private or protected
         * ones must be marked {@code [[=Reflectable]]}, or serialized with the
         * AllowPrivate overload. Defined only where the toolchain provides
         * C++26 reflection.
         */
        template <typename T>
        [[nodiscard]]
        static CsvDocument serialize(const Vector<T>& rows);

        /**
         * @brief Like serialize(const Vector<T>&), but also reads private and
         * protected data members, of T and every struct flattened into it.
         * @see stdx::data::AllowPrivate
         */
        template <typename T>
        [[nodiscard]]
        static CsvDocument serialize(const Vector<T>& rows, AllowPrivateTag);

        /**
         * @brief Reconstructs a sequence of reflectable rows from this document.
         * @tparam T The row struct type.
         * @return The reconstructed rows.
         * @throws CsvTypeException if a field is missing or will not convert.
         *
         * Columns bind to fields by header name (honoring CsvPropertyName), so
         * column order need not match declaration order. Defined only where the
         * toolchain provides C++26 reflection.
         */
        template <typename T>
        [[nodiscard]]
        THROWS(CsvTypeException)
        Vector<T> deserialize() const;

        /**
         * @brief Like deserialize(), but also writes private and protected data
         * members, of T and every struct flattened into it.
         * @see stdx::data::AllowPrivate
         */
        template <typename T>
        [[nodiscard]]
        THROWS(CsvTypeException)
        Vector<T> deserialize(AllowPrivateTag) const;
        #endif

        [[nodiscard]]
        bool operator==(const CsvDocument& other) const noexcept {
            return _has_header == other._has_header && _header == other._header && _rows == other._rows;
        }
    };
}
