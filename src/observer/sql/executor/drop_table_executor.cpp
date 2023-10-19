/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2023/6/14.
//

#include "sql/executor/drop_table_executor.h"

#include "session/session.h"
#include "common/log/log.h"
#include "storage/table/table.h"
#include "sql/stmt/drop_table_stmt.h"
#include "event/sql_event.h"
#include "event/session_event.h"
#include "storage/db/db.h"


RC DropTableExecutor::execute(SQLStageEvent *sql_event)
{
  RC rc = RC::SUCCESS;
  Stmt *stmt = sql_event->stmt();
  SessionEvent *session_event = sql_event->session_event();
  Session *session = session_event->session();
  ASSERT(stmt->type() == StmtType::DROP_TABLE, 
         "drop table executor can not run this command: %d", static_cast<int>(stmt->type()));


  //RC rc = session->get_current_db()->create_table(table_name, attribute_count, create_table_stmt->attr_infos().data());

  
  DropTableStmt *drop_table_stmt = static_cast<DropTableStmt *>(stmt);

  const char *table_name = drop_table_stmt->table_name().c_str();
  Table *table=session->get_current_db()->find_table(table_name);
  if(table==nullptr){
    LOG_WARN("Can not drop not-exist table %s.", table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }
  
  rc = session->get_current_db()->drop_table(table,table_name);
  
  return rc;
}