/**
 *    Copyright (C) 2018-present MongoDB, Inc.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the Server Side Public License, version 1,
 *    as published by MongoDB, Inc.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    Server Side Public License for more details.
 *
 *    You should have received a copy of the Server Side Public License
 *    along with this program. If not, see
 *    <http://www.mongodb.com/licensing/server-side-public-license>.
 *
 *    As a special exception, the copyright holders give permission to link the
 *    code of portions of this program with the OpenSSL library under certain
 *    conditions as described in each individual source file and distribute
 *    linked combinations including the program with the OpenSSL library. You
 *    must comply with the Server Side Public License in all respects for
 *    all of the code used other than as permitted herein. If you modify file(s)
 *    with this exception, you may extend this exception to your version of the
 *    file(s), but you are not obligated to do so. If you do not wish to do so,
 *    delete this exception statement from your version. If you delete this
 *    exception statement from all source files in the program, then also delete
 *    it in the license file.
 */

#pragma once

#include "mongo/db/session_killer.h"

/**
 * Mongod local kill session / transaction functionality library.
 */
namespace mongo {

/**
 * Kills all cursors, ops, and transactions on mongod for sessions matching 'matcher'.
 */
SessionKiller::Result killSessionsLocal(OperationContext* opCtx,
                                        const SessionKiller::Matcher& matcher,
                                        SessionKiller::UniformRandomBitGenerator* urbg);

/**
 * Matches only sessions which have unprepared multi-statement transactions, kills the sessions and
 * aborts the transactions.
 */
void killSessionsAbortUnpreparedTransactions(OperationContext* opCtx,
                                             const SessionKiller::Matcher& matcher,
                                             ErrorCodes::Error reason = ErrorCodes::Interrupted);

/**
 * Aborts any expired transactions.
 */
void killAllExpiredTransactions(OperationContext* opCtx);

/**
 * Run during shutdown to kill all in-progress transactions, including those in prepare.
 */
void killSessionsLocalShutdownAllTransactions(OperationContext* opCtx);

/**
 * Run during rollback to abort all in-progress prepared transactions.
 */
void killSessionsAbortAllPreparedTransactions(OperationContext* opCtx);

/**
 * Yields locks of prepared transactions.
 */
void yieldLocksForPreparedTransactions(OperationContext* opCtx);

}  // namespace mongo