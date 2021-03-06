/*************************************************************************
 *
 * REALM CONFIDENTIAL
 * __________________
 *
 *  [2011] - [2012] Realm Inc
 *  All Rights Reserved.
 *
 * NOTICE:  All information contained herein is, and remains
 * the property of Realm Incorporated and its suppliers,
 * if any.  The intellectual and technical concepts contained
 * herein are proprietary to Realm Incorporated
 * and its suppliers and may be covered by U.S. and Foreign Patents,
 * patents in process, and are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from Realm Incorporated.
 *
 **************************************************************************/

#ifndef REALM_EXCEPTIONS_HPP
#define REALM_EXCEPTIONS_HPP

#include <stdexcept>

#include <realm/util/features.h>

namespace realm {

/// Thrown by various functions to indicate that a specified table does not
/// exist.
class NoSuchTable: public std::exception {
public:
    const char* what() const REALM_NOEXCEPT_OR_NOTHROW override;
};


/// Thrown by various functions to indicate that a specified table name is
/// already in use.
class TableNameInUse: public std::exception {
public:
    const char* what() const REALM_NOEXCEPT_OR_NOTHROW override;
};


// Thrown by functions that require a table to **not** be the target of link
// columns, unless those link columns are part of the table itself.
class CrossTableLinkTarget: public std::exception {
public:
    const char* what() const REALM_NOEXCEPT_OR_NOTHROW override;
};


/// Thrown by various functions to indicate that the dynamic type of a table
/// does not match a particular other table type (dynamic or static).
class DescriptorMismatch: public std::exception {
public:
    const char* what() const REALM_NOEXCEPT_OR_NOTHROW override;
};


/// The \c LogicError exception class is intended to be thrown only when
/// applications (or bindings) violate rules that are stated (or ought to have
/// been stated) in the documentation of the public API, and only in cases
/// where the violation could have been easily and efficiently predicted by the
/// application. In other words, this exception class is for the cases where
/// the error is due to incorrect use of the public API.
///
/// This class is not supposed to be caught by applications. It is not even
/// supposed to be considered part of the public API, and therefore the
/// documentation of the public API should **not** mention the \c LogicError
/// exception class by name. Note how this contrasts with other exception
/// classes, such as \c NoSuchTable, which are part of the public API, and are
/// supposed to be mentioned in the documentation by name. The \c LogicError
/// exception is part of Realm's private API.
///
/// In other words, the \c LogicError class should exclusively be used in
/// replacement (or in addition to) asserts (debug or not) in order to
/// guarantee program interruption, while still allowing for complete
/// test-cases to be written and run.
///
/// To this effect, the special `CHECK_LOGIC_ERROR()` macro is provided as a
/// test framework plugin to allow unit tests to check that the functions in
/// the public API do throw \c LogicError when rules are violated.
///
/// The reason behind hiding this class from the public API is to prevent users
/// from getting used to the idea that "Undefined Behaviour" equates a specific
/// exception being thrown. The whole point of properly documenting "Undefined
/// Behaviour" cases is to help the user know what the limits are, without
/// constraining the database to handle every and any use-case thrown at it.
///
/// FIXME: This exception class should probably be moved to the `_impl`
/// namespace, in order to avoid some confusion.
class LogicError: public std::exception {
public:
    enum ErrorKind {
        string_too_big,
        binary_too_big,
        table_name_too_long,
        column_name_too_long,
        table_index_out_of_range,
        row_index_out_of_range,
        column_index_out_of_range,
        bad_version,

        /// Indicates that an argument has a value that is illegal in combination
        /// with another argument, or with the state of an involved object.
        illegal_combination,

        /// Indicates a data type mismatch, such as when `Table::find_pkey_int()` is
        /// called and the type of the primary key is not `type_Int`.
        type_mismatch,

        /// Indicates that an involved table is of the wrong kind, i.e., if it is a
        /// subtable, and the function requires a root table.
        wrong_kind_of_table,

        /// Indicates that an involved accessor is was detached, i.e., was not
        /// attached to an underlying object.
        detached_accessor,

        // Indicates that an involved column lacks a search index.
        no_search_index,

        // Indicates that an involved table lacks a primary key.
        no_primary_key,

        // Indicates that an attempt was made to add a primary key to a table that
        // already had a primary key.
        has_primary_key,

        /// Indicates that a modification to a column was attempted that cannot
        /// be done because the column is the primary key of the table.
        is_primary_key,

        /// Indicates that a modification was attempted that would have produced a
        /// duplicate primary value.
        unique_constraint_violation,

        /// User attempted to insert null in non-nullable column
        column_not_nullable,

        /// Group::open() is called on a group accessor that is already in the
        /// attached state. Or Group::open() or Group::commit() is called on a
        /// group accessor that is managed by a SharedGroup object.
        wrong_group_state,

        /// No active transaction on a particular SharedGroup object (e.g.,
        /// SharedGroup::commit()), or the active transaction on the SharedGroup
        /// object is of the wrong type (read/write), or an attampt was made to
        /// initiate a new transaction while one is already in progress on the
        /// same SharedGroup object.
        wrong_transact_state
    };

    LogicError(ErrorKind message);

    const char* what() const REALM_NOEXCEPT_OR_NOTHROW override;
    ErrorKind kind() const REALM_NOEXCEPT_OR_NOTHROW;
private:
    ErrorKind m_kind;
};




// Implementation:

inline const char* NoSuchTable::what() const REALM_NOEXCEPT_OR_NOTHROW
{
    return "No such table exists";
}

inline const char* TableNameInUse::what() const REALM_NOEXCEPT_OR_NOTHROW
{
    return "The specified table name is already in use";
}

inline const char* CrossTableLinkTarget::what() const REALM_NOEXCEPT_OR_NOTHROW
{
    return "Table is target of cross-table link columns";
}

inline const char* DescriptorMismatch::what() const REALM_NOEXCEPT_OR_NOTHROW
{
    return "Table descriptor mismatch";
}

inline LogicError::LogicError(LogicError::ErrorKind kind):
    m_kind(kind)
{
}

inline LogicError::ErrorKind LogicError::kind() const REALM_NOEXCEPT_OR_NOTHROW
{
    return m_kind;
}


} // namespace realm

#endif // REALM_EXCEPTIONS_HPP
