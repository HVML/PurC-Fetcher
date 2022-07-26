/*
 *  Copyright (C) 2006-2017 Apple Inc. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#pragma once

#include <wtf/EnumTraits.h>

namespace PurCFetcher {

enum ExceptionCode {
    // DOMException error names (https://heycam.github.io/webidl/#idl-DOMException-error-names).
    // Those need to be kept in sync with the array in DOMException.cpp.
    IndexSizeError, // Deprecated. Use RangeError instead.
    HierarchyRequestError,
    WrongDocumentError,
    InvalidCharacterError,
    NoModificationAllowedError,
    NotFoundError,
    NotSupportedError,
    InUseAttributeError,
    InvalidStateError,
    SyntaxError,
    InvalidModificationError,
    NamespaceError,
    InvalidAccessError, // Deprecated. use NotAllowedError instead.
    TypeMismatchError, // Deprecated. Use TypeError instead.
    SecurityError,
    NetworkError,
    AbortError,
    URLMismatchError,
    QuotaExceededError,
    TimeoutError,
    InvalidNodeTypeError,
    DataCloneError,
    EncodingError,
    NotReadableError,
    UnknownError,
    ConstraintError,
    DataError,
    TransactionInactiveError,
    ReadonlyError,
    VersionError,
    OperationError,
    NotAllowedError,

    // Simple exceptions (https://heycam.github.io/webidl/#idl-exceptions).
    RangeError,
    TypeError,

    // Non-standard error.
    StackOverflowError,

    // Used to indicate to the bindings that a JS exception was thrown below and it should be propagated.
    ExistingExceptionError,
};

} // namespace PurCFetcher

namespace WTF {

template<> struct EnumTraits<PurCFetcher::ExceptionCode> {
    using values = EnumValues<
    PurCFetcher::ExceptionCode,
    PurCFetcher::ExceptionCode::IndexSizeError,
    PurCFetcher::ExceptionCode::HierarchyRequestError,
    PurCFetcher::ExceptionCode::WrongDocumentError,
    PurCFetcher::ExceptionCode::InvalidCharacterError,
    PurCFetcher::ExceptionCode::NoModificationAllowedError,
    PurCFetcher::ExceptionCode::NotFoundError,
    PurCFetcher::ExceptionCode::NotSupportedError,
    PurCFetcher::ExceptionCode::InUseAttributeError,
    PurCFetcher::ExceptionCode::InvalidStateError,
    PurCFetcher::ExceptionCode::SyntaxError,
    PurCFetcher::ExceptionCode::InvalidModificationError,
    PurCFetcher::ExceptionCode::NamespaceError,
    PurCFetcher::ExceptionCode::InvalidAccessError,
    PurCFetcher::ExceptionCode::TypeMismatchError,
    PurCFetcher::ExceptionCode::SecurityError,
    PurCFetcher::ExceptionCode::NetworkError,
    PurCFetcher::ExceptionCode::AbortError,
    PurCFetcher::ExceptionCode::URLMismatchError,
    PurCFetcher::ExceptionCode::QuotaExceededError,
    PurCFetcher::ExceptionCode::TimeoutError,
    PurCFetcher::ExceptionCode::InvalidNodeTypeError,
    PurCFetcher::ExceptionCode::DataCloneError,
    PurCFetcher::ExceptionCode::EncodingError,
    PurCFetcher::ExceptionCode::NotReadableError,
    PurCFetcher::ExceptionCode::UnknownError,
    PurCFetcher::ExceptionCode::ConstraintError,
    PurCFetcher::ExceptionCode::DataError,
    PurCFetcher::ExceptionCode::TransactionInactiveError,
    PurCFetcher::ExceptionCode::ReadonlyError,
    PurCFetcher::ExceptionCode::VersionError,
    PurCFetcher::ExceptionCode::OperationError,
    PurCFetcher::ExceptionCode::NotAllowedError,
    PurCFetcher::ExceptionCode::RangeError,
    PurCFetcher::ExceptionCode::TypeError,
    PurCFetcher::ExceptionCode::StackOverflowError,
    PurCFetcher::ExceptionCode::ExistingExceptionError
    >;
};

} // namespace WTF
